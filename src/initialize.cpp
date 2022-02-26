#include "main.h"
#include "api.h"

pros::Motor chassisLF(chassisLFPort, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassisLM(chassisLMPort, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassisLB(chassisLBPort, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassisRF(chassisRFPort, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassisRM(chassisRMPort, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassisRB(chassisRBPort, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor frontLift(frontLiftPort, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor intake(intakePort, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);


pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut piston1(CLAW_FRONT, true);
pros::ADIDigitalOut piston2(CLAW_BACK, false);

pros::ADIAnalogIn lineSensor(lineSensorPort);
// pros::ADIDigitalOut piston3(WING_LEFT, false);
// pros::ADIDigitalOut piston4(WING_RIGHT, false);

pros::Imu imu(imuPort);


//pros::ADIDigitalIn bumper(bumperPort);

