#include "main.h"


/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// z
/////


const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED  = 90;
const int SWING_SPEED = 90;



///
// Constants
///

// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

void default_constants() {
  chassis.set_slew_min_power(90, 90);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.3, 0, 2.5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.3, 0, 2.5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 1.6, 0.8, 5, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void one_mogo_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5.5, 0.003, 5, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void two_mogo_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void exit_condition_defaults() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}



///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater then the slew distance + a few inches


  chassis.set_drive_pid(48, DRIVE_SPEED, true);
  chassis.wait_drive();
  
  // chassis.set_drive_pid(-24, DRIVE_SPEED);
  // chassis.wait_drive();

  // chassis.set_drive_pid(-24, DRIVE_SPEED);
  // chassis.wait_drive();
}



///
// Turn Example
///
void turn_example() {
  // The first parameter is target degrees
  // The second parameter is max speed the robot will drive at

  default_constants();
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();

  // chassis.set_turn_pid(45, TURN_SPEED);
  // chassis.wait_drive();

  // chassis.set_turn_pid(0, TURN_SPEED);
  // chassis.wait_drive();
}



///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_drive();
}



///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // wait_until will wait until the robot gets to a desired position


  // When the robot gets to 6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  // When the robot gets to -6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_until(-6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();
}



///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is target degrees
  // The third parameter is speed of the moving side of the drive


  chassis.set_swing_pid(ez::LEFT_SWING, 105, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(48, DRIVE_SPEED, true);
  chassis.wait_until(12);

  //chassis.set_swing_pid(ez::RIGHT_SWING, 0, SWING_SPEED);
  //chassis.wait_drive();
}



///
// Auto that tests everything
///
void combining_movements() {
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_swing_pid(ez::RIGHT_SWING, -45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_drive();
}



///
// Interference example
///
void tug (int attempts) {
  for (int i=0; i<attempts-1; i++) {
    // Attempt to drive backwards
    printf("i - %i", i);
    chassis.set_drive_pid(-12, 127);
    chassis.wait_drive();

    // If failsafed...
    if (chassis.interfered) {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(-2, 20);
      pros::delay(1000);
    }
    // If robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, robot will drive forward and turn 90 degrees. 
// If interfered, robot will drive forward and then attempt to drive backwards. 
void interfered_example() {
 chassis.set_drive_pid(24, DRIVE_SPEED, true);
 chassis.wait_drive();

 if (chassis.interfered) {
   tug(3);
   return;
 }

 chassis.set_turn_pid(90, TURN_SPEED);
 chassis.wait_drive();
}



// . . .
// Make your own autonomous functions here!
// . . .

void Diagonal() { //middle + rings
    //;.frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    moveClamp(false);
    chassisDriveDistance(1775, 127);
    moveBackClamp(false);
    wait(100);
    moveClamp(true);
    wait(150);
    driveP(-1800, -1800, 120);
    moveFrontLift(-675, 127);
    turnP(-90, 100);
    driveP(-1200, -1200, 80);
    moveBackClamp(true);
    driveP(300, 300, 50);
    moveIntake(600);
    turnP(88, 100);
    driveP(2400, 2400, 40);
    wait(100);
    driveP(-2600, -2600, 120);
    moveBackClamp(false);
    moveIntake(0);
    moveFrontLift(675, 127);
}

void DiagonalMiddle() {
    //frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    moveClamp(false);
    chassisDriveDistance(1850, 127);
    moveBackClamp(false);
    wait(250);
    moveClamp(true);
    wait(150);
    driveP(-1800, -1800, 120);
    moveFrontLift(-675, 127);
    turnP(-65, 100);
    driveP(-2100, -2100, 80);
    moveBackClamp(true);
    driveP(300, 300, 50);
    moveIntake(600);
    turnP(92, 100);
    driveP(2400, 2400, 30);
    wait(100);
    driveP(-2500, -2500, 120);
    moveBackClamp(false);
    moveIntake(0);
    moveFrontLift(675, 127);
}

void DiagonalDouble() {}

void Platform() {
  //frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  moveClamp(false);
  chassisDriveDistance(1550, 127);
  moveBackClamp(false);
  wait(200);
  moveClamp(true);
  wait(150);
  driveP(-2600, -2600, 120);
  wait(100);
  moveFrontLift(-675, 127);
  wait(100);
  turnP(-60, 115);
  driveP(-800, -800, 80);
  wait(50);
  moveBackClamp(true);
  turnP(-40, 115);
  wait(2000);
  moveIntake(600);
  driveP(1500, 1500, 30);
  driveP(-600, -600, 80);
  moveBackClamp(false);
}


void SoloWP() {
  moveBackClamp(false);
  moveFrontLift(-150, -127);
  frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  wait(100);
  driveP(200, 200, 80);
  moveClamp(false);
  wait(100);
  driveP(-200, -200, 80);
  moveFrontLift(150, -127);
  frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  wait(400);
  turnP(90, 120);
  driveP(-1200, -1200, 100);
  turnP(86, 120);
  driveP(-5600, -5600, 120);
  driveP(-600, -600, 80);
  wait(100);
  moveBackClamp(true);
  wait(100);
  moveFrontLift(-150, -127);
  moveIntake(600);
  driveP(500, 500, 100);
  wait(100);
  moveBackClamp(false);
}

void skills() {
    moveClamp(false);
    chassis.set_drive_pid(-5, DRIVE_SPEED,false); // drives into goal
    chassis.wait_drive();
    moveBackClamp(true); // clamps blue mogo
    pros::delay(200);
    chassis.set_drive_pid(4,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_swing_pid(ez::LEFT_SWING, 104, SWING_SPEED); // swing turn towards neutral mogo
    chassis.wait_drive();
    chassis.set_drive_pid(34,DRIVE_SPEED,false); // drives towards neutral
    chassis.wait_drive();
    chassis.set_drive_pid(6,40,false);
    chassis.wait_drive();
    moveClamp(true); // clamps neutral
    pros::delay(200);
    moveFrontLift(-500, 100); // lifts mogo
    moveIntake(600);
    chassis.set_turn_pid(114, TURN_SPEED); // turns to plat //here
    chassis.wait_drive();
    chassis.set_drive_pid(38,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveBackClamp(false);
    chassis.set_drive_pid(26,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_turn_pid(70,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_turn_pid(110,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(6,DRIVE_SPEED,false);
    chassis.wait_drive();
    liftmove(200, 300);
    wait(200);
    moveClamp(false); //stack yellow
    liftmove(-200, 300);
    pros::delay(300);
    chassis.set_drive_pid(-10, DRIVE_SPEED,false);
    chassis.wait_drive();
    pros::delay(300);
    moveIntake(0);
    moveFrontLift(100, 100);
    chassis.set_turn_pid(-60 ,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(24,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveClamp(true);
    pros::delay(300);
    moveFrontLift(-450, 100);
    chassis.set_turn_pid(123,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(38,DRIVE_SPEED*0.6,false);
    chassis.wait_drive();
    moveClamp(false);
    pros::delay(300); //stack second mogo
    chassis.wait_drive();
    chassis.set_drive_pid(-10,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveFrontLift(50,100);
    chassis.set_turn_pid(-3,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(34,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_drive_pid(6,40,false);
    chassis.wait_drive();
    moveClamp(true); // grab alliance mogo
    frontLift.move(0);
    pros::delay(300);
    chassis.set_drive_pid(-11,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_swing_pid(ez::RIGHT_SWING, 45, SWING_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(-52, 60, false);
    chassis.wait_drive();
    moveBackClamp(true);
    chassis.set_drive_pid(-72,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveBackClamp(false); // ungrab middle tall goal
    moveFrontLift(-450,100);
    moveIntake(600);
    chassis.set_drive_pid(8,40,true);
    chassis.wait_drive();
    chassis.set_drive_pid(16,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_swing_pid(ez::RIGHT_SWING, -45, SWING_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(18,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveClamp(false); //stack alliance mogo
    pros::delay(300);
    chassis.set_drive_pid(-6, DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_turn_pid(0,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(-35,DRIVE_SPEED,false);
    moveIntake(0);
    moveFrontLift(50,100);
    chassis.wait_drive();
    chassis.set_drive_pid(-8, 40,false);
    chassis.wait_drive();
    moveBackClamp(true); //grab other side alliance gaol
    pros::delay(300);
    chassis.set_drive_pid(22, DRIVE_SPEED, true);
    chassis.wait_drive();
    chassis.set_turn_pid(90,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(22,DRIVE_SPEED,true);
    chassis.wait_drive();
    chassis.set_drive_pid(6,40,true);
    chassis.wait_drive();      
    moveClamp(true); //grab far side middle goal
    frontLift.move(0);
    pros::delay(300);
    moveFrontLift(-450,100);
    moveIntake(600);
    chassis.set_turn_pid(65,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(36,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveBackClamp(false); 
    chassis.set_drive_pid(22,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveClamp(false); //stack yellow goal
    pros::delay(300);
    chassis.set_drive_pid(-4,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_turn_pid(245,TURN_SPEED);
    chassis.wait_drive();
    moveIntake(0);
    moveFrontLift(50,100);
    pros::delay(750);
    chassis.set_drive_pid(8,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_drive_pid(6,40,false);
    chassis.wait_drive();
    moveClamp(true);
    //frontLift.move(0);
    pros::delay(500);
    moveFrontLift(-450,100);
    chassis.set_turn_pid(55,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(30,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveClamp(false); //stack other alliance goal
    pros::delay(300);
    chassis.set_swing_pid(ez::LEFT_SWING, 0, SWING_SPEED);
    chassis.wait_drive();
    moveFrontLift(50,100);
    chassis.set_drive_pid(-24,DRIVE_SPEED,false);
    chassis.wait_drive();
    chassis.set_turn_pid(45,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(12,40,false);
    chassis.wait_drive();
    moveClamp(true); 
    frontLift.move(0);
    pros::delay(300);
    chassis.set_drive_pid(-12,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveFrontLift(-450,100);
    chassis.set_turn_pid(305,TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(104,DRIVE_SPEED,false);
    chassis.wait_drive();
    moveClamp(false);
    pros::delay(300);
    chassis.set_drive_pid(-14,DRIVE_SPEED,false);
    // chassis.wait_drive();
    // moveFrontLift(50,100);
    // chassis.set_turn_pid(215,TURN_SPEED);
    // chassis.wait_drive();
    // chassis.set_drive_pid(48,DRIVE_SPEED,false);
    // chassis.wait_drive();
    // moveClamp(true); //grab last alliance goal
    // pros::delay(300);
    // moveFrontLift(-450,100);
    // pros::delay(750);
    // chassis.set_turn_pid(0,TURN_SPEED);
    // chassis.wait_drive();
    // chassis.set_drive_pid(42,DRIVE_SPEED,false);
    // chassis.wait_drive();
    // moveClamp(false); //stack last alliance goal
    // chassis.set_drive_pid(-20, DRIVE_SPEED);
    
    
}

void test() {
  chassis.set_turn_pid(12, TURN_SPEED);
}

