/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convienent for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"

/**
 * You should add more #includes here
 */
//#include "okapi/api.hpp"
//#include "pros/api_legacy.h"
#include "EZ-Template/api.hpp"
#include "autons.hpp"
#include "pros/apix.h"


// More includes here...
//
// . . .

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
// using namespace pros;
// using namespace pros::literals;
// using namespace okapi;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif
const int chassisLFPort = 8;
const int chassisLMPort = 7;
const int chassisLBPort = 6;
const int chassisRFPort = 20;
const int chassisRMPort = 19;
const int chassisRBPort = 18;

const int frontLiftPort = 3;
const int intakePort = 17;

//sensor ports
const int imuPort = 5;
const char bumperPort = 'E';
const std::uint8_t lineSensorPort = 'C';

//tracking wheel encoder ports
const std::uint8_t trackingLeftPort = 'E';
const std::uint8_t trackingRightPort = 'F';
const std::uint8_t trackingBackPort = 'G';

//piston ports
const std::uint8_t CLAW_FRONT = 'A';
const std::uint8_t CLAW_BACK = 'B';
// const std::uint8_t WING_LEFT = 'C';
// const std::uint8_t WING_RIGHT = 'D';

//motors
extern pros::Motor chassisLF;
extern pros::Motor chassisLM;
extern pros::Motor chassisLB;
extern pros::Motor chassisRF;
extern pros::Motor chassisRM;
extern pros::Motor chassisRB;

extern pros::Motor intake;
extern pros::Motor frontLift;

//pistons
extern pros::ADIDigitalOut piston1;
extern pros::ADIDigitalOut piston2;
// extern pros::ADIDigitalOut piston3;
// extern pros::ADIDigitalOut piston4;

//sensors
extern pros::Imu imu;
extern pros::ADIDigitalIn bumper;
extern pros::ADIAnalogIn lineSensor;


extern pros::Controller master;

#ifdef __cplusplus
extern "C" {
#endif

//basic funct
void chassisDrive(int x, int y);
void frontliftMove(int d);
void moveFrontLift(int abs, int voltage);
void manualLift(int volt);
void liftmove(int volt, int time);
void moveClamp(bool c);
void moveBackClamp(bool a);
void moveWings(bool l);
void moveIntake(int a);
void stopIntake();
void wait(int time);

//PIDs
void turnP(int angle, int voltmax);
void driveP(int left, int right, int voltmax);
void driveQ(int voltmax);
void drive(int left, int right, int voltmax);
void armP(int degrees, int voltmax);
void turnQ(int voltmax);
void turn(int degrees, int voltmax);

//chassis funct
void chassisTurn(double degree);
void chassisDriveDistance(int unit, int speed);
void chassisDriveUntilDistance(int stop_distance, int speed);
void chassisDriveUntilLevel();
void setChassisBreakMode(const pros::motor_brake_mode_e_t mode);


#ifdef __cplusplus
}
#endif

#endif  // _PROS_MAIN_H_
