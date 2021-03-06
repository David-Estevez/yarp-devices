// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * @ingroup yarp_devices_cuiAbsolute
 * \defgroup yarp_devices_picFirmware PIC-Firmware
 *
 * @brief Firmware that allows communication with the PIC and the publication of position messages. 
 *
 * @section yarp_devices_firmware_legal Legal
 *
 * Copyright: 2016 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/people/r-de-santos">Raul de Santos Rico</a>
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * @section cuiAbsolute_install Installation
 *
 * You need to install MPLAB IDE v8.92 and MPLAB C Compiler for PIC18 MCUs. <br>
 * Open the proyect with MPLAB, open "main.c" file and change the value of "canId" variable (You can see <a class="el" href="http://robots.uc3m.es/index.php/CuiAbsolute_Documentation">correspondence</a>) <br>
 * Also, to increase the time between sending messages,  you can modify "sendDelay" value too (see code documentation in main.c). 
 * Then, build all. <br>
 * Finally, select programmer "MPLAB ICD 2", disconnect CAN-BUS connections of the PIC, connect and program it. <br> 
 *
 * @section cuiAbsolute_running Running (assuming correct installation)
 *
 *
 * To test Cui Aboslute encoders are working properly, you need to start the program named "testCuiAbsolute" and see that the test passed correctly. <br>
 * Previously, you have to manually change the ID of Cui that you want to test in the code (example: #define CAN_ID 124), compile and reinstall. <br>
 * The testCuiAbsolute application can be edited at:
 * tests/testCuiAbsolute.cpp
\verbatim
[on terminal] testCuiAbsolute
\endverbatim
 * And then, in the end of the test you should see this:
\verbatim
[----------] 3 tests from CuiAbsoluteTest (5082 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test case ran. (5082 ms total)
[  PASSED  ] 3 tests.
\endverbatim
 *
 * @section cuiAbsolute_modify Modify
 *
 * This file can be edited at
 * firmware/CuiAbsolute/Pic_source/main.cpp
 *
 */

#include <p18F2580.h>
#include "ECAN.h"
#include "ECAN.c"
#include "timers.h"
#include <delays.h>
#include <spi.h>
#include <stdlib.h>
#include <stdio.h>

#pragma config OSC = HS, FCMEN = OFF, IESO = OFF
#pragma config PWRT = OFF, BOREN = OFF, BORV = 1
#pragma config WDT = OFF, WDTPS = 32768, DEBUG = ON
#pragma config PBADEN = OFF, LPT1OSC = OFF, MCLRE = ON
#pragma config STVREN = OFF, LVP = OFF, XINST = OFF


/***** Variables configurables (CAN_ID & ENCODER PULSES & SEND_DELAY) *******************************************************/

// CAN ID (Ver correspondecia en http://robots.uc3m.es/index.php/CuiAbsolute_Documentation)
unsigned long canId = 498; //508 (codo 124) 492 (pierna izquierda 108)

// ENCODER PULSES (Valor compuesto por el factor de pulsos-por-slot (normalmente 4) y numero total de slots del encoder (actualmente: 4 * 1024))
double encoderPulses = 4096;

/* SEND DELAY (Valor que utilizar� Delay10TCYx en el env�o. Valor recomendado de 1 a 100)
 * El byte 3 (data[2]) que recibir� el PIC (valor comprendido entre [0-255]) se multiplicar� por el tiempo que
 * tarde en ejecutar el delay marcado por la funci�n Delay10TCYx(sendDelay)
 * A tener en cuenta:
	- La velocidad de ejecuci�n de cada ciclo de instrucci�n son 0.8 microsegundos
        - Delay10TCYx(i) -> 10.Tcy.i genera una demora de 10 ciclos de instrucciones * i . Por tanto Delay10TCYc(1) equivale a 8 microsegundos (10 ciclos de reloj) */
BYTE sendDelay = 1; //Default: 1 
/***********************************************************************************************************/

// -- Inicializaci�n de variables para el env�o
int aux, message[2];
int i=0;
double degrees;
double div = encoderPulses/360.0; // resultado dividir 4096/360 = 11.38
BYTE x, y;
ECAN_TX_MSG_FLAGS txFlags = ECAN_TX_PRIORITY_3 & ECAN_TX_STD_FRAME & ECAN_TX_NO_RTR_FRAME;
int stop_flag=0; // -- flag para saber cuando se ha recibido un stop

// -- Variables que almacenar�n los datos a recibir:
unsigned long picId;
BYTE data[8];
BYTE dataLen;
ECAN_RX_MSG_FLAGS rxflags;

// -- Prototipos de funciones
void send(void);
void cleanData(void);



// -- Funcion principal
void main(void)
{
    SSPCON1bits.SSPEN=1;
    OpenSPI(SPI_FOSC_4, MODE_00, SMPEND);

    TRISCbits.TRISC2=0;     // CS, out
    TRISCbits.TRISC3=0;     // SCL, out
    TRISCbits.TRISC5=0;     // SDO, out
    TRISCbits.TRISC4=1;     // SDI  in
    LATCbits.LATC2=1;       //Disable del encoder al inicio

    OSCCON=0b11110000;       //Primary oscillator.

    // Manual (page 15): ECAN.def file must be set up correctly.
    ECANInitialize();

    // Manual (page 38): Must be in Config mode to change many of settings.
    ECANSetOperationMode(ECAN_OP_MODE_CONFIG);

    /* Manual (page 1):
    ECAN provides three modes of operation � Mode 0, Mode 1 and Mode 2. Mode 0 is fully backward compat-
    ible with the legacy CAN module. Applications developed for the legacy CAN module would continue to
    work without any change using ECAN. Mode 1 is the Enhanced Legacy mode with increased buffers and fil-
    ters. Mode 2 has the same resources as Mode 1, but with a hardware managed receive FIFO. Given its fea-
    tures and flexibility, ECAN would prove useful to many CAN-based applications. */
    ECANSetFunctionalMode(ECAN_MODE_0);

    // Manual (page 41): Enable double-buffering
    ECANSetRXB0DblBuffer(ECAN_DBL_BUFFER_MODE_DISABLE);

    /* Manual (page 44):
    This macro sets the value for a mask register. There are a total of two macros, one for each mask.
    For example, for mask RXM0, use ECANSetRXM0Value, for RXM1, use ECANSetRXM1Value and so on
    ECAN_MSG_STD Standard type. 11-bit of value will be used.
    ECAN_MSG_XTD Extended type. 29-bit of value will be used.*/
    ECANSetRXM0Value(-1, ECAN_MSG_STD);

    // Manual (page 42): RXB0 will receive all valid messages
    ECANSetRxBnRxMode(RXB0, ECAN_RECEIVE_ALL_VALID); //ECAN_RECEIVE_ALL_VALID (funciona) ECAN_RECEIVE_STANDARD (inestable, se queda pillado) ECAN_RECEIVE_ALL (no funciona, el mensaje de STOP lo ignora)

    // Manual (page 36): This macro sets the CAN bus Wake-up Filter mode. Specifies that the low-pass filter be disabled
    ECANSetFilterMode(ECAN_FILTER_MODE_DISABLE);

    // Manual (page 38): Return to Normal mode to communicate.
    ECANSetOperationMode(ECAN_OP_MODE_NORMAL);


    /* Mensaje compuesto por 3 bytes
    -- data[0]: (Byte 1)
    	* start (0x01)
    	* stop  (0x02)
    -- data[1]: (Byte 2)
    	* mode1 (0x01): publicaci�n permanente		(necesita indicar velocidad de publicaci�n)
    	* mode2 (0x02): publicaci�n por petici�n	(no necesita indicar velocidad de publicaci�n)
    -- data[2]: (Byte 3)
    	* (0 - 255)
    	A tener en cuenta:
    		- La frecuencia de oscilaci�n se encuentra definida como Fosc = 5Mhz (20/4)
    		- La velocidad de ejecuci�n de cada ciclo de instrucci�n son 0.8 microsegundos
    		- Delay10TCYx(i) -> 10.Tcy.i genera una demora de 10 ciclos de instrucciones * i . Por tanto Delay10TCYc(1) equivale a 8 microsegundos (10 ciclos de reloj)
    */

    while(1)
    {
        // --------------- Recibo!!! ------------------
        if(ECANReceiveMessage(&picId, data, &dataLen, &rxflags))
        {
            // ----------- Checkeamos ID Driver -----------
            if(picId == canId-384)  		// -- (canId = picId + 0x180)
            {
                if(data[0]==0x01 && data[1]==0x01 && data[3]==0x00 && data[4]==0x00 && data[5]==0x00 && data[6]==0x00 && data[7]==0x00 )    // -- comienza publicaci�n (start) modo permanente : if(data[0]==0x01 && data[1]==0x01)
                {
                    // -- mientras no mande un Stop, sigue publicando
                    while(!stop_flag)
                    {
                        send();	// -- envia
                        for( i=0; (i<= data[2]) && (!stop_flag) ; i++ )  // DELAY: data[2] recibir� un valor comprendido en [0 - 255]
                        {
                            Delay10TCYx(sendDelay);
                            ECANReceiveMessage(&picId, data, &dataLen, &rxflags);
                            if((data[0]==0x02 && data[1]==0x01 && data[3]==0x00 && data[4]==0x00 && data[5]==0x00 && data[6]==0x00 && data[7]==0x00) && (picId == canId-384)) 
                            	stop_flag=1;
                        }
                    }
                    cleanData(); // -- Se para, limpia las variables
                    stop_flag=0;
                }
                if(data[0]==0x01 && data[1]==0x02 && data[2]==0x00 && data[3]==0x00 && data[4]==0x00 && data[5]==0x00 && data[6]==0x00 && data[7]==0x00)  	// -- publica por pulling (petici�n)
                {
                    send();		 // Manda una �nico mensaje
                    cleanData(); // Limpia las variables
                }
            } // if(picId == canId-384)
        } // if(ECANReceiveMessage(&picId, data, &dataLen, &flags))
    } // while
} // main


// -- Funcion de envio de mensajes de posicion del Cui:
void send()
{
    // - Manual: [1. The host issues the command, 0x10. The data read in at this time will be 0xa5 or 0x00 since this is the first SPI transfer]
    LATCbits.LATC2=0;			
    WriteSPI (0b00010000);      //Solicitaci�n de posici�n (comando 0x10) 
    Delay10TCYx(4);             // Used 4 to fix Known error message (37 b6 ff c4) or (3c 13 fe c4). Old delay: 3 ( 3 = 6us)
    y=SSPBUF;                   //Recoge los datos del SSPBUF que provienen del encoder (MISO)
    LATCbits.LATC2=1;			

	// - Manual: [2. The host waits a minimum of 5 �s then sends a �nop_a5� command: 0x00]
    LATCbits.LATC2=0;			
    WriteSPI (0b00000000);      //Comando 0x00 -> nop_a5
    Delay10TCYx(4);             // Used 4 to fix Known error message (37 b6 ff c4) or (3c 13 fe c4). Old delay: 3 ( 3 = 6us)
    y=SSPBUF;                   //Recoge los datos del SSPBUF que provienen del encoder (MISO)
    LATCbits.LATC2=1;

	// - Manual: [3. The response to the �nop_a5� is either 0xa5 or an echo of the command, 0x10.]
    while(y==0b10100101)  		// - Manual: [a. If it is 0xa5, it will go back to step 2.]											
    {
	    Delay10TCYx(3); 
        LATCbits.LATC2=0;
        WriteSPI (0b00000000);  // Comando 0x00 -> nop_a5)
        Delay10TCYx(4); 		// Used 4 to fix Known error message (37 b6 ff c4) or (3c 13 fe c4). Old delay: 3 ( 3 = 6us)
        y=SSPBUF;				// Recoge los datos del SSPBUF que provienen del encoder (MISO)
        LATCbits.LATC2=1;
    }

	// - Manual: [b. Otherwise it will go to step 4.] (echo of the command, 0x10.)
	// - Manual: [4. The host waits a minimum of 5 �s then sends �nop_a5�, the data read is the high byte of the position.]
    LATCbits.LATC2=0;			
    WriteSPI(0b00000000);		//Comando 0x00 -> nop_a5
    Delay10TCYx(4);				// Used 4 to fix Known error message (37 b6 ff c4) or (3c 13 fe c4). Old delay: 3 ( 3 = 6us)
    y=SSPBUF;					//Recoge los datos del SSPBUF que provienen del encoder (MISO)
    message[0]=y;				//Se almacena en el primer elemento del array que se enviar� por CAN
    LATCbits.LATC2=1;
    
    // - Manual: [5. The host waits a minimum of 5 �s then sends �nop_a5�, the data read is the low byte of the position.]
    LATCbits.LATC2=0;
    WriteSPI(0b00000000);       //Espera para captar la parte baja del mensaje
    Delay10TCYx(4);             // Used 4 to fix Known error message (37 b6 ff c4) or (3c 13 fe c4). Old delay: 3 ( 3 = 6us)
    y=SSPBUF;                   //Recoge los datos del SSPBUF que provienen del encoder (MISO)
    message[1]=y;               //Se almacena en el segundo elemento del array que se enviar� por CAN

	// - Manual: [6. The host waits a minimum of 5 �s before sending another SPI command.]
    LATCbits.LATC2=1;
    aux=(message[0]<<8)+message[1];  //Se rota el byte alto de la posici�n 8 bits (2^8=256) y se suma al bajo
    degrees = aux / div;            //Se divide para obtener la realacci�n en grados

    x=0;
    while( !x )
    {
        x=ECANSendMessage(canId, &degrees, sizeof(degrees), txFlags);
    }
}

// -- Funcion para limpiar los datos recibidos por el PIC
void cleanData()
{
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x00;
}
