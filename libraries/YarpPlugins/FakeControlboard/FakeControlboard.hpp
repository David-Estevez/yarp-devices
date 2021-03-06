// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __FAKE_CONTROLBOARD_HPP__
#define __FAKE_CONTROLBOARD_HPP__

#include <vector>

#include <yarp/os/RateThread.h>
#include <yarp/os/Semaphore.h>

#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/IControlLimits2.h>

#define DEFAULT_AXES 5

#define DEFAULT_EXTRA_ROBOT "none"
#define DEFAULT_EXTERN_OBJ "none"
#define DEFAULT_GEN_ENC_RAW_EXPOSED 0.0174532925199433  // Ratio, 0.0174532925199433 is pi/180 (raw/exp)<->(rad/deg)
#define DEFAULT_GEN_INIT_POS 0  // Exposed.
#define DEFAULT_GEN_JOINT_TOL 0.25  // Exposed.
#define DEFAULT_GEN_MAX_LIMIT 180.0  // Exposed.
#define DEFAULT_GEN_MIN_LIMIT -180.0  // Exposed.
#define DEFAULT_GEN_REF_SPEED 7.5  // Exposed.
#define DEFAULT_GEN_VEL_RAW_EXPOSED 0.0174532925199433  // Ratio, 0.0174532925199433 is pi/180 (raw/exp)<->(rad/deg)
#define DEFAULT_JMC_MS 20  // [ms]
#define DEFAULT_MODE_POS_VEL 0  // 0=Position, 1=Velocity.

namespace roboticslab
{

/**
 * @ingroup TeoYarp
 * \defgroup FakeControlboard
 *
 * @brief Contains teo::FakeControlboard.
 *
 * @section FakeControlboard_install Installation
 *
 * The plugin is compiled when ENABLE_TeoYarp_FakeControlboard is activated (not default). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 */

/**
 * @ingroup FakeControlboard
 * @brief Implements the YARP_dev IPositionControl, IVelocityControl, IEncodersTimed, etc.
 * interface class member functions.
 */
class FakeControlboard : public yarp::dev::DeviceDriver,
                         public yarp::dev::IPositionControl2,
                         public yarp::dev::IVelocityControl2,
                         public yarp::dev::IEncodersTimed,
                         public yarp::dev::IControlLimits2,
                         public yarp::dev::IControlMode2,
                         public yarp::dev::ITorqueControl,
                         public yarp::os::RateThread
{
public:

    // Set the Thread Rate in the class constructor
    FakeControlboard() : RateThread(DEFAULT_JMC_MS) {}  // In ms

// ------- IPositionControl declarations. Implementation in IPositionControlImpl.cpp -------

    /**
     * Get the number of controlled axes. This command asks the number of controlled
     * axes for the current physical interface.
     * @param ax pointer to storage
     * @return true/false.
     */
    virtual bool getAxes(int *ax);

    /**
     * Set new reference point for a single axis.
     * @param j joint number
     * @param ref specifies the new ref point
     * @return true/false on success/failure
     */
    virtual bool positionMove(int j, double ref);

    /**
     * Set new reference point for all axes.
     * @param refs array, new reference points.
     * @return true/false on success/failure
     */
    virtual bool positionMove(const double *refs);

    /**
     * Set relative position. The command is relative to the
     * current position of the axis.
     * @param j joint axis number
     * @param delta relative command
     * @return true/false on success/failure
     */
    virtual bool relativeMove(int j, double delta);

    /**
     * Set relative position, all joints.
     * @param deltas pointer to the relative commands
     * @return true/false on success/failure
     */
    virtual bool relativeMove(const double *deltas);

    /**
     * Check if the current trajectory is terminated. Non blocking.
     * @return true if the trajectory is terminated, false otherwise
     */
    virtual bool checkMotionDone(int j, bool *flag);

    /**
     * Check if the current trajectory is terminated. Non blocking.
     * @return true if the trajectory is terminated, false otherwise
     */
    virtual bool checkMotionDone(bool *flag);

    /**
     * Set reference speed for a joint, this is the speed used during the
     * interpolation of the trajectory.
     * @param j joint number
     * @param sp speed value
     * @return true/false upon success/failure
     */
    virtual bool setRefSpeed(int j, double sp);

    /**
     * Set reference speed on all joints. These values are used during the
     * interpolation of the trajectory.
     * @param spds pointer to the array of speed values.
     * @return true/false upon success/failure
     */
    virtual bool setRefSpeeds(const double *spds);

    /**
     * Set reference acceleration for a joint. This value is used during the
     * trajectory generation.
     * @param j joint number
     * @param acc acceleration value
     * @return true/false upon success/failure
     */
    virtual bool setRefAcceleration(int j, double acc);

    /**
     * Set reference acceleration on all joints. This is the valure that is
     * used during the generation of the trajectory.
     * @param accs pointer to the array of acceleration values
     * @return true/false upon success/failure
     */
    virtual bool setRefAccelerations(const double *accs);

    /**
     * Get reference speed for a joint. Returns the speed used to
     * generate the trajectory profile.
     * @param j joint number
     * @param ref pointer to storage for the return value
     * @return true/false on success or failure
     */
    virtual bool getRefSpeed(int j, double *ref);

    /**
     * Get reference speed of all joints. These are the  values used during the
     * interpolation of the trajectory.
     * @param spds pointer to the array that will store the speed values.
     */
    virtual bool getRefSpeeds(double *spds);

    /**
     * Get reference acceleration for a joint. Returns the acceleration used to
     * generate the trajectory profile.
     * @param j joint number
     * @param acc pointer to storage for the return value
     * @return true/false on success/failure
     */
    virtual bool getRefAcceleration(int j, double *acc);

    /**
     * Get reference acceleration of all joints. These are the values used during the
     * interpolation of the trajectory.
     * @param accs pointer to the array that will store the acceleration values.
     * @return true/false on success or failure
     */
    virtual bool getRefAccelerations(double *accs);

    /**
     * Stop motion, single joint
     * @param j joint number
     * @return true/false on success/failure
     */
    virtual bool stop(int j);

    /**
     * Stop motion, multiple joints
     * @return true/false on success/failure
     */
    virtual bool stop();

// ------- IPositionControl2 declarations. Implementation in IPositionControl2Impl.cpp -------

    /**
     * Set new reference point for a subset of joints.
     * @param joints pointer to the array of joint numbers
     * @param refs   pointer to the array specifing the new reference points
     * @return true/false on success/failure
     */
    virtual bool positionMove(const int n_joint, const int *joints, const double *refs);

    /**
     * Set relative position for a subset of joints.
     * @param joints pointer to the array of joint numbers
     * @param deltas pointer to the array of relative commands
     * @return true/false on success/failure
     */
    virtual bool relativeMove(const int n_joint, const int *joints, const double *deltas);

    /**
     * Check if the current trajectory is terminated. Non blocking.
     * @param joints pointer to the array of joint numbers
     * @param flags  pointer to return value (logical "and" of all set of joints)
     * @return true/false if network communication went well.
     */
    virtual bool checkMotionDone(const int n_joint, const int *joints, bool *flags);

    /**
     * Set reference speed on all joints. These values are used during the
     * interpolation of the trajectory.
     * @param joints pointer to the array of joint numbers
     * @param spds   pointer to the array with speed values.
     * @return true/false upon success/failure
     */
    virtual bool setRefSpeeds(const int n_joint, const int *joints, const double *spds);

    /**
     * Set reference acceleration on all joints. This is the valure that is
     * used during the generation of the trajectory.
     * @param joints pointer to the array of joint numbers
     * @param accs   pointer to the array with acceleration values
     * @return true/false upon success/failure
     */
    virtual bool setRefAccelerations(const int n_joint, const int *joints, const double *accs);

    /**
     * Get reference speed of all joints. These are the  values used during the
     * interpolation of the trajectory.
     * @param joints pointer to the array of joint numbers
     * @param spds   pointer to the array that will store the speed values.
     * @return true/false upon success/failure
     */
    virtual bool getRefSpeeds(const int n_joint, const int *joints, double *spds);

    /**
     * Get reference acceleration for a joint. Returns the acceleration used to
     * generate the trajectory profile.
     * @param joints pointer to the array of joint numbers
     * @param accs   pointer to the array that will store the acceleration values
     * @return true/false on success/failure
     */
    virtual bool getRefAccelerations(const int n_joint, const int *joints, double *accs);

    /**
     * Stop motion for subset of joints
     * @param joints pointer to the array of joint numbers
     * @return true/false on success/failure
     */
    virtual bool stop(const int n_joint, const int *joints);

    /**
     * Get the last position reference for the specified axis.
     *  This is the dual of PositionMove and shall return only values sent using
     *  IPositionControl interface.
     *  If other interfaces like IPositionDirect are implemented by the device, this call
     *  must ignore their values, i.e. this call must never return a reference sent using
     *  IPositionDirect::SetPosition
     * @param ref last reference sent using PositionMove functions
     * @return true/false on success/failure
     */
    virtual bool getTargetPosition(const int joint, double *ref);

    /**
     * Get the last position reference for all axes.
     *  This is the dual of PositionMove and shall return only values sent using
     *  IPositionControl interface.
     *  If other interfaces like IPositionDirect are implemented by the device, this call
     *  must ignore their values, i.e. this call must never return a reference sent using
     *  IPositionDirect::SetPosition
     * @param ref last reference sent using PositionMove functions
     * @return true/false on success/failure
     */
    virtual bool getTargetPositions(double *refs);

    /**
     * Get the last position reference for the specified group of axes.
     *  This is the dual of PositionMove and shall return only values sent using
     *  IPositionControl interface.
     *  If other interfaces like IPositionDirect are implemented by the device, this call
     *  must ignore their values, i.e. this call must never return a reference sent using
     *  IPositionDirect::SetPosition
     * @param ref last reference sent using PositionMove functions
     * @return true/false on success/failure
     */
    virtual bool getTargetPositions(const int n_joint, const int *joints, double *refs);

//  ---------- IEncodersTimed Declarations. Implementation in IEncoderImpl.cpp ----------

    /**
     * Reset encoder, single joint. Set the encoder value to zero
     * @param j encoder number
     * @return true/false
     */
    virtual bool resetEncoder(int j);

    /**
     * Reset encoders. Set the encoders value to zero
     * @return true/false
     */
    virtual bool resetEncoders();

    /**
     * Set the value of the encoder for a given joint.
     * @param j encoder number
     * @param val new value
     * @return true/false
     */
    virtual bool setEncoder(int j, double val);

    /**
     * Set the value of all encoders.
     * @param vals pointer to the new values
     * @return true/false
     */
    virtual bool setEncoders(const double *vals);

    /**
     * Read the value of an encoder.
     * @param j encoder number
     * @param v pointer to storage for the return value
     * @return true/false, upon success/failure (you knew it, uh?)
     */
    virtual bool getEncoder(int j, double *v);

    /**
     * Read the position of all axes.
     * @param encs pointer to the array that will contain the output
     * @return true/false on success/failure
     */
    virtual bool getEncoders(double *encs);

    /**
     * Read the instantaneous speed of an axis.
     * @param j axis number
     * @param sp pointer to storage for the output
     * @return true if successful, false ... otherwise.
     */
    virtual bool getEncoderSpeed(int j, double *sp);

    /**
     * Read the instantaneous speed of all axes.
     * @param spds pointer to storage for the output values
     * @return guess what? (true/false on success or failure).
     */
    virtual bool getEncoderSpeeds(double *spds);

    /**
     * Read the instantaneous acceleration of an axis.
     * @param j axis number
     * @param spds pointer to the array that will contain the output
     */
    virtual bool getEncoderAcceleration(int j, double *spds);

    /**
     * Read the instantaneous acceleration of all axes.
     * @param accs pointer to the array that will contain the output
     * @return true if all goes well, false if anything bad happens.
     */
    virtual bool getEncoderAccelerations(double *accs);

    /**
     * Read the instantaneous acceleration of all axes.
     * @param encs pointer to the array that will contain the output
     * @param time pointer to the array that will contain individual timestamps
     * @return true if all goes well, false if anything bad happens.
     */
    virtual bool getEncodersTimed(double *encs, double *time);

    /**
     * Read the instantaneous acceleration of all axes.
     * @param j axis index
     * @param encs encoder value (pointer to)
     * @param time corresponding timestamp (pointer to)
     * @return true if all goes well, false if anything bad happens.
     */
    virtual bool getEncoderTimed(int j, double *encs, double *time);

//  --------- IVelocityControl Declarations. Implementation in IVelocityControlImpl.cpp ---------

    /**
     * Start motion at a given speed, single joint.
     * @param j joint number
     * @param sp speed value
     * @return bool/false upone success/failure
     */
    virtual bool velocityMove(int j, double sp);

    /**
     * Start motion at a given speed, multiple joints.
     * @param sp pointer to the array containing the new speed values
     * @return true/false upon success/failure
     */
    virtual bool velocityMove(const double *sp);

//  --------- IVelocityControl2 Declarations. Implementation in IVelocityControl2Impl.cpp ---------

    /**
     * Start motion at a given speed for a subset of joints.
     * @param n_joint how many joints this command is referring to
     * @param joints of joints controlled. The size of this array is n_joints
     * @param spds pointer to the array containing the new speed values, one value for each joint, the size of the array is n_joints.
     * The first value will be the new reference fot the joint joints[0].
     *          for example:
     *          n_joint  3
     *          joints   0  2  4
     *          spds    10 30 40
     * @return true/false on success/failure
     */
    virtual bool velocityMove(const int n_joint, const int *joints, const double *spds);

    /**
     * Get the last reference speed set by velocityMove for single joint.
     * @param j joint number
     * @param vel returns the requested reference.
     * @return true/false on success/failure
     */
    virtual bool getRefVelocity(const int joint, double *vel);

    /**
     * Get the last reference speed set by velocityMove for all joints.
     * @param vels pointer to the array containing the new speed values, one value for each joint
     * @return true/false on success/failure
     */
    virtual bool getRefVelocities(double *vels);

    /**
     * Get the last reference speed set by velocityMove for a group of joints.
     * @param n_joint how many joints this command is referring to
     * @param joints of joints controlled. The size of this array is n_joints
     * @param vels pointer to the array containing the requested values, one value for each joint.
     *  The size of the array is n_joints.
     * @return true/false on success/failure
     */
    virtual bool getRefVelocities(const int n_joint, const int *joints, double *vels);

//  --------- IControlLimits Declarations. Implementation in IControlLimitsImpl.cpp ---------

    /**
     * Set the software limits for a particular axis, the behavior of the
     * control card when these limits are exceeded, depends on the implementation.
     * @param axis joint number (why am I telling you this)
     * @param min the value of the lower limit
     * @param max the value of the upper limit
     * @return true or false on success or failure
     */
    virtual bool setLimits(int axis, double min, double max);

    /**
     * Get the software limits for a particular axis.
     * @param axis joint number (again... why am I telling you this)
     * @param pointer to store the value of the lower limit
     * @param pointer to store the value of the upper limit
     * @return true if everything goes fine, false otherwise.
     */
    virtual bool getLimits(int axis, double *min, double *max);

//  --------- IControlLimits2 Declarations. Implementation in IControlLimits2Impl.cpp ---------

    /**
     * Set the software speed limits for a particular axis, the behavior of the
     * control card when these limits are exceeded, depends on the implementation.
     * @param axis joint number
     * @param min the value of the lower limit
     * @param max the value of the upper limit
     * @return true or false on success or failure
     */
    virtual bool setVelLimits(int axis, double min, double max);

    /**
     * Get the software speed limits for a particular axis.
     * @param axis joint number
     * @param min pointer to store the value of the lower limit
     * @param max pointer to store the value of the upper limit
     * @return true if everything goes fine, false otherwise.
     */
    virtual bool getVelLimits(int axis, double *min, double *max);

//  --------- IControlMode Declarations. Implementation in IControlModeImpl.cpp ---------

    /**
     * Get the current control mode.
     * @param j joint number
     * @param mode a vocab of the current control mode for joint j.
     * @return true/false success failure.
     */
    virtual bool getControlMode(int j, int *mode);

    /**
     * Get the current control mode (multiple joints).
     * @param modes a vector containing vocabs for the current control modes of the joints.
     * @return true/false success failure.
     */
    virtual bool getControlModes(int *modes);

//  --------- IControlMode2 Declarations. Implementation in IControlMode2Impl.cpp ---------

    /**
     * Get the current control mode for a subset of axes.
     * @param n_joints how many joints this command is referring to
     * @param joints list of joint numbers, the size of this array is n_joints
     * @param modes array containing the new controlmodes, one value for each joint, the size is n_joints.
     *          The first value will be the new reference fot the joint joints[0].
     *          for example:
     *          n_joint  3
     *          joints   0  2  4
     *          modes    VOCAB_CM_POSITION VOCAB_CM_VELOCITY VOCAB_CM_POSITION
     * @return true/false success failure.
     */
    virtual bool getControlModes(const int n_joint, const int *joints, int *modes);

    /**
     * Set the current control mode.
     * @param j joint number
     * @param mode a vocab of the desired control mode for joint j.
     * @return true if the new controlMode was successfully set, false if the message was not received or
     *         the joint was unable to switch to the desired controlMode
     *         (e.g. the joint is on a fault condition or the desired mode is not implemented).
     */
    virtual bool setControlMode(const int j, const int mode);

    /**
     * Set the current control mode for a subset of axes.
     * @param n_joints how many joints this command is referring to
     * @param joints list of joint numbers, the size of this array is n_joints
     * @param modes array containing the new controlmodes, one value for each joint, the size is n_joints.
     *          The first value will be the new reference fot the joint joints[0].
     *          for example:
     *          n_joint  3
     *          joints   0  2  4
     *          modes    VOCAB_CM_POSITION VOCAB_CM_VELOCITY VOCAB_CM_POSITION
     * @return true if the new controlMode was successfully set, false if the message was not received or
     *         the joint was unable to switch to the desired controlMode
     *         (e.g. the joint is on a fault condition or the desired mode is not implemented).
     */
    virtual bool setControlModes(const int n_joint, const int *joints, int *modes);

    /**
     * Set the current control mode (multiple joints).
     * @param modes a vector containing vocabs for the desired control modes of the joints.
     * @return true if the new controlMode was successfully set, false if the message was not received or
     *         the joint was unable to switch to the desired controlMode
     *         (e.g. the joint is on a fault condition or the desired mode is not implemented).
     */
    virtual bool setControlModes(int *modes);

// -------- ITorqueControl declarations. Implementation in ITorqueControlImpl.cpp --------

    /**
     * Get the reference value of the torque for all joints.
     * This is NOT the feedback (see getTorques instead).
     * @param t pointer to the array of torque values
     * @return true/false on success/failure
     */
    virtual bool getRefTorques(double *t);

    /**
     * Get the reference value of the torque for a given joint.
     * This is NOT the feedback (see getTorque instead).
     * @param j joint number
     * @param t the returned reference torque of joint j
     * @return true/false on success/failure
     */
    virtual bool getRefTorque(int j, double *t);

    /**
     * Set the reference value of the torque for all joints.
     * @param t pointer to the array of torque values
     * @return true/false on success/failure
     */
    virtual bool setRefTorques(const double *t);

    /**
     * Set the reference value of the torque for a given joint.
     * @param j joint number
     * @param t new value
     * @return true/false on success/failure
     */
    virtual bool setRefTorque(int j, double t);

    /** Set new torque reference for a subset of joints.
     * @param joints pointer to the array of joint numbers
     * @param refs   pointer to the array specifing the new torque reference
     * @return true/false on success/failure
     */
    virtual bool setRefTorques(const int n_joint, const int *joints, const double *t);

    /** Get a subset of motor parameters (bemf, ktau etc) useful for torque control.
     * @param j joint number
     * @param params a struct containing the motor parameters to be retrieved
     * @return true/false on success/failure
     */
    virtual bool getMotorTorqueParams(int j,  yarp::dev::MotorTorqueParameters *params);

    /** Set a subset of motor parameters (bemf, ktau etc) useful for torque control.
     * @param j joint number
     * @param params a struct containing the motor parameters to be set
     * @return true/false on success/failure
     */
    virtual bool setMotorTorqueParams(int j, const yarp::dev::MotorTorqueParameters params);

    /**
     * Get the value of the torque on a given joint (this is the
     * feedback if you have a torque sensor).
     * @param j joint number
     * @param t pointer to the result value
     * @return true/false on success/failure
     */
    virtual bool getTorque(int j, double *t);

    /**
     * Get the value of the torque for all joints (this is
     * the feedback if you have torque sensors).
     * @param t pointer to the array that will store the output
     * @return true/false on success/failure
     */
    virtual bool getTorques(double *t);

    /**
     * Get the full scale of the torque sensor of a given joint
     * @param j joint number
     * @param min minimum torque of the joint j
     * @param max maximum torque of the joint j
     * @return true/false on success/failure
     */
    virtual bool getTorqueRange(int j, double *min, double *max);

    /**
     * Get the full scale of the torque sensors of all joints
     * @param min pointer to the array that will store minimum torques of the joints
     * @param max pointer to the array that will store maximum torques of the joints
     * @return true/false on success/failure
     */
    virtual bool getTorqueRanges(double *min, double *max);

#if YARP_VERSION_MAJOR != 3
    /**
     * Set the back-efm compensation gain for a given joint.
     * @param j joint number
     * @param bemf the returned bemf gain of joint j
     * @return true/false on success/failure
     */
    virtual bool getBemfParam(int j, double *bemf);

    /**
     * Set the back-efm compensation gain for a given joint.
     * @param j joint number
     * @param bemf new value
     * @return true/false on success/failure
     */
    virtual bool setBemfParam(int j, double bemf);
#endif // YARP_VERSION_MAJOR != 3

// -------- DeviceDriver declarations. Implementation in DeviceDriverImpl.cpp --------

    /**
     * Open the DeviceDriver.
     * @param config is a list of parameters for the device.
     * Which parameters are effective for your device can vary.
     * See \ref dev_examples "device invocation examples".
     * If there is no example for your device,
     * you can run the "yarpdev" program with the verbose flag
     * set to probe what parameters the device is checking.
     * If that fails too,
     * you'll need to read the source code (please nag one of the
     * yarp developers to add documentation for your device).
     * @return true/false upon success/failure
     */
    virtual bool open(yarp::os::Searchable& config);

    /**
     * Close the DeviceDriver.
     * @return true/false on success/failure.
     */
    virtual bool close();

// -------- RateThread declarations. Implementation in RateThreadImpl.cpp --------

    /**
     * Initialization method. The thread executes this function
     * when it starts and before "run". This is a good place to
     * perform initialization tasks that need to be done by the
     * thread itself (device drivers initialization, memory
     * allocation etc). If the function returns false the thread
     * quits and never calls "run". The return value of threadInit()
     * is notified to the class and passed as a parameter
     * to afterStart(). Note that afterStart() is called by the
     * same thread that is executing the "start" method.
     */
    bool threadInit();

    /**
     * Loop function. This is the thread itself.
     */
    void run();

// ----- Shared Area Funcion declarations. Implementation in SharedArea.cpp -----

    void setEncRaw(const int index, const double position);
    void setEncsRaw(const std::vector<double> & positions);

    double getEncRaw(const int index);
    std::vector<double> getEncsRaw();

    double getEncExposed(const int index);
    std::vector<double> getEncsExposed();

// ------------------------------- Private -------------------------------------

private:

    enum jmc_state { NOT_MOVING, POSITION_MOVE, RELATIVE_MOVE, VELOCITY_MOVE };
    enum jmc_mode { POSITION_MODE, VELOCITY_MODE };

    bool setPositionMode(int j);
    bool setVelocityMode(int j);
    bool setTorqueMode(int j);

    // General Joint Motion Controller parameters //
    unsigned int axes;
    double jmcMs;
    jmc_mode modePosVel;
    double lastTime;

    yarp::os::Semaphore encRawMutex;  // SharedArea

    std::vector<jmc_state> jointStatus;

    std::vector<double> encRaw;
    std::vector<double> encRawExposed;  // For conversion.
    std::vector<double> initPos;  // Exposed.
    std::vector<double> jointTol;  // Exposed.
    std::vector<double> maxLimit;  // Exposed.
    std::vector<double> minLimit;  // Exposed.
    std::vector<double> refAcc;  // Exposed.
    std::vector<double> refSpeed;  // Exposed.
    std::vector<double> targetExposed;  // Exposed.
    std::vector<double> velRawExposed;  // For conversion.
    std::vector<double> velRaw;
};

}  // namespace roboticslab

#endif  // __FAKE_CONTROLBOARD_HPP__
