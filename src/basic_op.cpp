#include "main.h"
#include <fstream>

using namespace std;


const int chass_power_threshold = 3;
const int arm_voltage = 127;

void setChassisBreakMode(const pros::motor_brake_mode_e_t mode) {
    chassisLF.set_brake_mode(mode);
    chassisLM.set_brake_mode(mode);
    chassisLB.set_brake_mode(mode);
    chassisRF.set_brake_mode(mode);
    chassisRM.set_brake_mode(mode);
    chassisRB.set_brake_mode(mode);
}

void chassisDrive(int left_power, int right_power) {
    if (abs(left_power) < chass_power_threshold) {
        left_power = 0;
    }
    if (abs(right_power) < chass_power_threshold) {
        right_power = 0;
    }

    chassisLF.move(left_power);
    chassisLM.move(left_power);
    chassisLB.move(left_power);
    chassisRF.move(right_power);
    chassisRM.move(right_power);
    chassisRB.move(right_power);
}

void moveIntake(int a) {
    intake.move_velocity(a);
}

bool testFrontArm = true;
int armStage = 0;
void frontliftMove(int direction) {
    if (direction > 0) {
        testFrontArm = true;
        if(armStage == 0) {
            frontLift.move_absolute(-620, -arm_voltage);
            //moveIntake(-200);
            armStage=1;
        } else {
            frontLift.move_absolute(-900, -arm_voltage);
            //moveIntake(-200);
        }

    } else if (direction < 0) {
        testFrontArm = false;
        armStage=0;
        frontLift.move_absolute(0, arm_voltage);
    } else if (!(direction > 0) && testFrontArm && !(armStage == 0)){
        frontLift.move(0);
        //moveIntake(0);
    }

    //std::cout << pros::millis() << "Arm Stage: " << armStage << std::endl;
}

void liftmove(int volt, int time) {
    frontLift.move(volt);
    pros::delay(time);
    frontLift.move(0);
    frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}
void moveFrontLift(int abs, int voltage) {
    frontLift.move_absolute(abs, voltage);
    
}

void manualLift(int deg) {
    frontLift.move(deg);
}

void moveClamp(bool close) {
    if (close) {
        piston1.set_value(1);
    } else {
        piston1.set_value(0);
    }
}

void moveBackClamp(bool a) {
    if(a) {
        piston2.set_value(1);
    } else {
        piston2.set_value(0);
    }
}

// void moveWings(bool l) {
//     if(l) {
//         piston3.set_value(1);
//         piston4.set_value(1);
//     } else {
//         piston3.set_value(0);
//         piston4.set_value(0);
//     }
// }



void stopIntake() {
    intake.move_velocity(0);
}

void chassisRotate(int speed, bool clockwise) {
    int speed_modifier = clockwise ? 1 : -1;
    chassisDrive(speed_modifier * speed, -1 * speed_modifier * speed);
}

int calculate_rotate_speed(int delta) {
    if (std::abs(delta) < 30) {
        return 50;
    }
    return 60;
}

int calculate_rotation_threshold(int degree) {
    degree = abs(degree);
    if (degree >= 90) {
        return 7;
    } else if (degree >= 60) {
        return 6;
    } else if (degree >= 45) {
        return 5;
    } else if (degree >= 30) {
        return 3;
    } 

    return 2;
}

void chassisTurn(double degree) {

    //sstd::cout << "chassis turn " << degree << std::endl;
    setChassisBreakMode(pros::E_MOTOR_BRAKE_BRAKE);
    double chass_rotate_degree_threshold = calculate_rotation_threshold(degree);

    imu.tare_heading();
    double current_pos = imu.get_heading();
    std::cout << pros::millis() << "imu rotation: " << current_pos << std::endl;
    double target_pos = (int)(current_pos + degree);
    double delta = degree;
    double last_delta = std::abs(delta);

    while (std::abs(delta) > chass_rotate_degree_threshold) {
        int speed = calculate_rotate_speed(delta);
        chassisRotate(speed, delta > 0);
        pros::delay(20);
        
        current_pos = imu.get_heading();
        delta = (int)(target_pos - current_pos) % 360;
        last_delta = std::abs(delta);
    }
    cout << "break out from the loop" << endl;
    chassisDrive(0, 0);
}

double averageChassisMotorPosition() {
    auto l_pos = chassisLF.get_position();
    auto r_pos = chassisRF.get_position(); 

        // cout << "left position: " << l_pos
        //     << ", right position: " << r_pos
        //     << endl;

    return (l_pos + r_pos) / 2;
}

void wait(int time) {
    pros::delay(time);
}

void chassisDriveDistance(int unit, int speed) {
    setChassisBreakMode(pros::E_MOTOR_BRAKE_COAST);
    int direction = (speed/-speed)*-1;
    chassisLF.tare_position();
    chassisRF.tare_position();
    chassisDrive(speed * direction, speed * direction);
    pros::delay(200);
    while (true) {
        chassisDrive(speed * direction, speed * direction);
        pros::delay(20);
        auto pos = averageChassisMotorPosition();
        if ((abs(pos) - unit) >= 400) {
            break;
        }
    }
    while (true) {
        chassisDrive(30 * direction, 30 * direction);
        pros::delay(20);
        auto pos = averageChassisMotorPosition();
        if (abs(pos) >= unit) {
            break;
        }
    }
    chassisDrive(0, 0);
}


void chassis_drive_until_level() {

    const int pitch_threshold = 10;
    setChassisBreakMode(pros::E_MOTOR_BRAKE_BRAKE);
    bool is_chassis_on_bridge = false;
    bool is_chassis_leveled = false;
    double max_pitch = 0;
    while (!is_chassis_on_bridge) {
        chassisDrive(-80, -80);
        pros::delay(20);

        auto pitch = imu.get_pitch();
        max_pitch = max(max_pitch, abs(pitch));
        if (abs(pitch) > pitch_threshold) {
            is_chassis_on_bridge = true;
        }
    }
    pros::delay(700);
    while (!is_chassis_leveled) {
        pros::delay(20);
        auto pitch = imu.get_pitch();
        max_pitch = max(max_pitch, abs(pitch));
        if (abs(pitch) < max_pitch - 1) {
            is_chassis_leveled = true;
        }
    }
    chassisDrive(0, 0);
    pros::delay(100);
    chassisDrive(50, 50);
    chassisDrive(0, 0);
    pros::delay(1000);
    cout << "final pitch: " << imu.get_pitch() << endl;
}

void driveP(int targetLeft, int targetRight, int voltageMax=115) {
    chassisLB.tare_position(); // reset base encoders
    chassisRB.tare_position();

    bool debugLog = false; //debuging

	float kp = 0.15;
	float acc = 5;
	float kpTurn = 0.7;
	float accTurn = 4;

	// the untouchables
	float voltageLeft = 0;
	float voltageRight = 0;
	int errorLeft;
	int errorRight;
	int voltageCap = 0;
	int signLeft;
	int signRight;
	int errorCurrent = 0;
	int errorLast = 0;
	int sameErrCycles = 0;
	int same0ErrCycles = 0;
	int startTime = pros::millis();
	targetLeft = targetLeft + chassisLB.get_position();
	targetRight = targetRight + chassisRB.get_position();

	while(autonomous){
		errorLeft = targetLeft - chassisLB.get_position(); // error is target minus actual value
        errorRight = targetRight - chassisRB.get_position();
		errorCurrent = (abs(errorRight) + abs(errorLeft)) / 2;

		signLeft = errorLeft / abs(errorLeft); // + or - 1
		signRight = errorRight / abs(errorRight);

		if(signLeft == signRight){
			voltageLeft = errorLeft * kp; // intended voltage is error times constant
			voltageRight = errorRight * kp;
			voltageCap = voltageCap + acc;  // slew rate
		}
		else{
			voltageLeft = errorLeft * kpTurn; // same logic with different turn value
			voltageRight = errorRight * kpTurn;
			voltageCap = voltageCap + accTurn;  // turn slew rate
		}
		
		if(voltageCap > voltageMax) voltageCap = voltageMax; // voltageCap cannot exceed 115

		if(abs(voltageLeft) > voltageCap) voltageLeft = voltageCap * signLeft; // limit the voltage
		if(abs(voltageRight) > voltageCap) voltageRight = voltageCap * signRight;// ditto

		// set the motors to the intended speed
		//chassis->getModel()->tank(voltageLeft/127, voltageRight/127);
        chassisLF.move(voltageLeft); 
        chassisLM.move(voltageLeft);
        chassisLB.move(voltageLeft);
        chassisRF.move(voltageRight);
        chassisRM.move(voltageRight);
        chassisRB.move(voltageRight);

		// timeout utility
		if (errorLast == errorCurrent) {
			if (errorCurrent <= 2) same0ErrCycles +=1; // less than 2 ticks is "0" error
			sameErrCycles += 1;
		}
		else {
			sameErrCycles = 0;
			same0ErrCycles = 0;
		}

		// exit paramaters
		if ((errorLast < 5 and errorCurrent < 5) or sameErrCycles >= 20) { // allowing for smol error or exit if we stay the same err for .4 second
			chassisLF.move_velocity(0); 
            chassisLM.move_velocity(0);
		    chassisLB.move_velocity(0);
		    chassisRF.move_velocity(0);
            chassisRM.move_velocity(0);
		    chassisRB.move_velocity(0);
			std::cout << pros::millis() << "task complete with error " << errorCurrent << " in " << (pros::millis() - startTime) << "ms" << std::endl;
			return;
		}
		
		// debug
		if (debugLog) {
			std::cout << pros::millis() << "error  " << errorCurrent << std::endl;
			std::cout << pros::millis() << "errorLeft  " << errorLeft << std::endl;
			std::cout << pros::millis() << "errorRight  " << errorRight << std::endl;
			std::cout << pros::millis() << "voltageLeft  " << voltageLeft << std::endl;
			std::cout << pros::millis() << "voltageRight  " << voltageRight << std::endl;
		}

		// nothing goes after this
		errorLast = errorCurrent;
		pros::delay(20);
	}
}



int voltageCap = 100; // voltageCap limits the change in velocity and must be global
int targetLeft;
int targetRight;
int targetTurn;
int targetTurnRelative;

void driveQ(int voltageMax) {
  chassisLB.tare_position(); // reset base encoders
  chassisRB.tare_position();
  int errorLeft;
  int errorRight;
 
  // the touchables ;)))))))) touch me uwu :):):)
  float kp = 0.15;
  float acc = 3.5;
  float kpTurn = 0.7;
  float accTurn = 4;

  // the untouchables
  int voltageLeft = 0;
  int voltageRight = 0;
  int signLeft;
  int signRight;
  int errorCurrent = 0;
  int errorLast = 0;
  int sameErrCycles = 0;
  int same0ErrCycles = 0;
  pros::delay(20); // dunno

  while(autonomous){
    errorLeft = targetLeft - chassisLB.get_position(); // error is target minus actual value
    errorRight = targetRight - chassisRB.get_position();
	errorCurrent = (abs(errorRight) + abs(errorLeft)) / 2;

    signLeft = errorLeft / abs(errorLeft); // + or - 1
    signRight = errorRight / abs(errorRight);

    if(signLeft == signRight){
      voltageLeft = errorLeft * kp; // intended voltage is error times constant
      voltageRight = errorRight * kp;
	  voltageCap = voltageCap + acc;  // slew rate
    }
    else{
      voltageLeft = errorLeft * kpTurn; // same logic with different turn value
      voltageRight = errorRight * kpTurn;
	  voltageCap = voltageCap + accTurn;  // turn slew rate
    }
    
    if(voltageCap > voltageMax){
      voltageCap = voltageMax; // voltageCap cannot exceed 115
    }

    if(abs(voltageLeft) > voltageCap){ // limit the voltage
      voltageLeft = voltageCap * signLeft;
    }

    if(abs(voltageRight) > voltageCap){ // ditto
      voltageRight = voltageCap * signRight;
    }

	// set the motors to the intended speed
    chassisLF.move(voltageLeft); 
    chassisLM.move(voltageLeft);
    chassisLB.move(voltageLeft);
    chassisRF.move(voltageRight);
    chassisRM.move(voltageRight);
    chassisRB.move(voltageRight);

	// timeout utility
	if (errorLast == errorCurrent) {
		if (errorCurrent < 2) {
			same0ErrCycles +=1;
		}
		sameErrCycles += 1;
	}
	else {
		sameErrCycles = 0;
		same0ErrCycles = 0;
	}

	// exit paramaters
	if ((errorLast < 4 and errorCurrent < 4) or sameErrCycles >= 10) { // allowing for smol error or exit if we stay the same err for .2 second
		chassisLF.move_velocity(0); 
        chassisLM.move_velocity(0);
		chassisLB.move_velocity(0);
		chassisRF.move_velocity(0);
        chassisRM.move_velocity(0);
		chassisRB.move_velocity(0);
		std::cout << "task complete with error " << errorCurrent << std::endl;
		return;
	}
	
	// debug
	if (sameErrCycles == 0) {
		std::cout << "error  " << errorCurrent << std::endl;
		std::cout << "voltageLeft  " << voltageLeft << std::endl;
	}

	// nothing goes after this
	errorLast = errorCurrent;
    pros::delay(20);
  }
}

void drive(int left, int right, int voltageMax=115){
  targetLeft = left;
  targetRight = right;
  voltageCap = 0; //reset velocity cap for slew rate
  driveQ(voltageMax);
}

void turnP(int targetTurn, int voltageMax) {
    imu.tare_rotation();
 	float kp = 1.6;
	float ki = 0.8;
	float kd = 0.45;

	// the untouchables
	int voltageCap = 0;
	float voltage = 0;
	float errorCurrent;
	float errorLast;
	int errorCurrentInt;
	int errorLastInt;
	int sameErrCycles = 0;
	int same0ErrCycles = 0;
	int p;
	float i;
	int d;
	int sign;
	float error;
	int startTime = pros::millis();

	while(autonomous) {
		error = targetTurn - imu.get_rotation();
		errorCurrent = abs(error);
		errorCurrentInt = errorCurrent;
		sign = targetTurn / abs(targetTurn); // -1 or 1

		p = (error * kp);
		if (abs(error) < 10) { // if we are in range for I to be desireable
			i = ((i + error) * ki);
		}
		else
			i = 0;
		d = (error - errorLast) * kd;
		
		voltage = p + i + d;

		if(abs(voltage) > voltageMax) voltage = voltageMax * sign;

		// set the motors to the intended speed
        chassisLF.move(voltage); 
        chassisLM.move(voltage);
        chassisLB.move(voltage);
        chassisRF.move(-voltage);
        chassisRM.move(-voltage);
        chassisRB.move(-voltage);
		// timeout utility
		if (errorLastInt == errorCurrentInt) {
			if (errorLast <= 2 and errorCurrent <= 2) { // saying that error less than 2 counts as 0
				same0ErrCycles +=1;
			}
			sameErrCycles += 1;
		}
		else {
			sameErrCycles = 0;
			same0ErrCycles = 0;
		}

		// exit paramaters
		if (same0ErrCycles >= 5 or sameErrCycles >= 60) { // allowing for smol error or exit if we stay the same err for .6 second
            chassisLF.move_velocity(0); 
            chassisLM.move_velocity(0);
    		chassisLB.move_velocity(0);
    		chassisRF.move_velocity(0);
            chassisRM.move_velocity(0);
	    	chassisRB.move_velocity(0);			
            std::cout << pros::millis() << "task complete with error " << errorCurrent << " in " << (pros::millis() - startTime) << "ms" << std::endl;
			return;
		}
		
		// debug
		// std::cout << pros::millis() << "error " << errorCurrent << std::endl;
		// std::cout << pros::millis() << "voltage " << voltage << std::endl;

		// for csv output, graphing the function

		// nothing goes after this
		errorLast = errorCurrent;
		errorLastInt = errorLast;
		pros::delay(10);
	}
}

void armP(int degrees, int voltageMax) {
    frontLift.tare_position();
 	float kp = 1.6;
	float ki = 0.8;
	float kd = 0.45;

	// the untouchables
	int voltageCap = 0;
	float voltage = 0;
	float errorCurrent;
	float errorLast;
	int errorCurrentInt;
	int errorLastInt;
	int sameErrCycles = 0;
	int same0ErrCycles = 0;
	int p;
	float i;
	int d;
	int sign;
	float error;

	while(autonomous) {
		error = degrees - frontLift.get_position();
		errorCurrent = abs(error);
		errorCurrentInt = errorCurrent;
		sign = degrees / abs(degrees); // -1 or 1

		p = (error * kp);
		if (abs(error) < 10) { // if we are in range for I to be desireable
			i = ((i + error) * ki);
		}
		else
			i = 0;
		d = (error - errorLast) * kd;
		
		voltage = p + i + d;

		if(abs(voltage) > voltageMax) voltage = voltageMax * sign;

		// set the motors to the intended speed
        frontLift.move(voltage);
		// timeout utility
		if (errorLastInt == errorCurrentInt) {
			if (errorLast <= 2 and errorCurrent <= 2) { // saying that error less than 2 counts as 0
				same0ErrCycles +=1;
			}
			sameErrCycles += 1;
		}
		else {
			sameErrCycles = 0;
			same0ErrCycles = 0;
		}

		// exit paramaters
		if (same0ErrCycles >= 5 or sameErrCycles >= 60) { // allowing for smol error or exit if we stay the same err for .6 second
            frontLift.move_velocity(0);		
			return;
		}

		errorLast = errorCurrent;
		errorLastInt = errorLast;
		pros::delay(10);
	}
}

void turnQ(int voltageMax) {
  // the touchables ;)))))))) touch me uwu :):):)
  float kp = 0.1;
  float ki = 0.1;
  float kd = 0;
  float acc = 0.1;

  // the untouchables
  int voltage = 0;
  float errorCurrent;
  float errorLast;
  int errorCurrentInt;
  int errorLastInt;
  int sameErrCycles = 0;
  int same0ErrCycles = 0;
  int p;
  float i;
  int d;
  int sign;
  double error;
  pros::delay(20); // dunno

  while(autonomous){
    error = targetTurn - imu.get_rotation();
	errorCurrent = abs(error);
	errorCurrentInt = errorCurrent;
	sign = targetTurnRelative / abs(targetTurnRelative); // -1 or 1

	p = (error * kp);
	if (abs(error) < 10) // if we are in range for I to be desireable
        i = ((i + error) * ki);
      else
        i = 0;
	d = (error - errorLast) * kd;
	
	voltage = p + i + d;
	voltageCap = voltageCap + acc;  // slew rate
    
	if(voltageCap > voltageMax){
      voltageCap = voltageMax; // voltageCap cannot exceed 115
    }

    if(abs(voltage) > voltageCap){ // limit the voltage
      voltage = voltageCap * sign;
    }

	// set the motors to the intended speed
    chassisLF.move(voltage); 
    chassisLM.move(voltage);
    chassisLB.move(voltage);
    chassisRF.move(-voltage);
    chassisRM.move(-voltage);
    chassisRB.move(-voltage);

	// timeout utility
	if (errorLastInt == errorCurrentInt) {
		if (errorLast < 2 and errorCurrent < 2) {
			same0ErrCycles +=1;
		}
		sameErrCycles += 1;
	}
	else {
		sameErrCycles = 0;
		same0ErrCycles = 0;
	}

	// exit paramaters
	if (same0ErrCycles >= 5 or sameErrCycles >= 100) { // allowing for smol error or exit if we stay the same err for 1 second
        chassisLF.move_velocity(0); 
        chassisLM.move_velocity(0);
    	chassisLB.move_velocity(0);
		chassisRF.move_velocity(0);
        chassisRM.move_velocity(0);
    	chassisRB.move_velocity(0);	
		std::cout << "task complete with error " << errorCurrent << std::endl;
		return;
	}
	
	// debug
	if (sameErrCycles == 0) {
		std::cout << "error  " << errorCurrent << std::endl;
		std::cout << "voltage " << voltage << std::endl;
	}

	// nothing goes after this
	errorLast = errorCurrent;
	errorLastInt = errorLast;
    pros::delay(10);
  }
}

void turn(int target, int voltageMax=115){
  imu.tare_rotation();
  targetTurn = target + imu.get_rotation();
  targetTurnRelative = target;
  voltageCap = 0; //reset velocity cap for slew rate
  turnQ(voltageMax);
}