#include "main.h"
#include <fstream>

using namespace std;

enum class autonStates { // the possible auton selections
	off,
	Diagonal,
	DiagonalMiddle,
	DiagonalDouble,
	Platform,
	SoloWP,
	Skills,
	test
};

autonStates autonSelection = autonStates::off;

static lv_res_t DiagonalBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::Diagonal;
	std::cout << pros::millis() << "Diagonal" << std::endl;
	return LV_RES_OK;
}

static lv_res_t DiagonalMiddleBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::DiagonalMiddle;
	std::cout << pros::millis() << "DiagonalMiddle" << std::endl;
	return LV_RES_OK;
}

static lv_res_t DiagonalDoubleBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::DiagonalDouble;
	std::cout << pros::millis() << "DiagonalDouble" << std::endl;
	return LV_RES_OK;
}

static lv_res_t PlatformBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::Platform;
	std::cout << pros::millis() << "Platform" << std::endl;
	return LV_RES_OK;
}

static lv_res_t SoloWPBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::SoloWP;
	std::cout << pros::millis() << "SoloWP" << std::endl;
	return LV_RES_OK;
}

static lv_res_t SkillsBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::Skills;
	std::cout << pros::millis() << "Skills" << std::endl;
	return LV_RES_OK;
}

// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-6, 7, -8}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{18, -20, 19}

  // IMU Port
  ,5

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,2.75

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,60.0/36.0

  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
);



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

  	// Print our branding over your terminal :D
  	imu.reset();
	std::cout << pros::millis() << ": calibrating imu..." << std::endl;

	lv_theme_t *th = lv_theme_alien_init(360, NULL); //Set a HUE value and keep font default RED
	lv_theme_set_current(th);

	// create a tab view object
	std::cout << pros::millis() << ": creating gui..." << std::endl;
	lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), NULL);

	//button layout
	lv_obj_t *DiagonalTab = lv_tabview_add_tab(tabview, "Diagonal");
	lv_obj_t *DiagonalMiddleTab = lv_tabview_add_tab(tabview, "DiagonalMiddle");
	lv_obj_t *DiagonalDoubleTab = lv_tabview_add_tab(tabview, "DiagonalDouble");
	lv_obj_t *PlatformTab = lv_tabview_add_tab(tabview, "Platform");
	lv_obj_t *SoloWPTab = lv_tabview_add_tab(tabview, "SoloWP");
	lv_obj_t *SkillsTab = lv_tabview_add_tab(tabview, "Skills");

	// Diagonal tab
	lv_obj_t *DiagonalBtn = lv_btn_create(DiagonalTab, NULL);
	lv_obj_t *label = lv_label_create(DiagonalBtn, NULL);
	lv_label_set_text(label, "Diagonal");
	lv_btn_set_action(DiagonalBtn, LV_BTN_ACTION_CLICK, DiagonalBtnAction);
	lv_obj_set_size(DiagonalBtn, 450, 50);
	lv_btnm_set_toggle(DiagonalBtn, true, 1);
	lv_obj_set_pos(DiagonalBtn, 0, 100);
	lv_obj_align(DiagonalBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	// DiagonalMiddle tab
	lv_obj_t *DiagonalMiddleBtn = lv_btn_create(DiagonalMiddleTab, NULL);
	lv_obj_t *label2 = lv_label_create(DiagonalMiddleBtn, NULL);
	lv_label_set_text(label2, "DiagonalMiddle");
	lv_btn_set_action(DiagonalMiddleBtn, LV_BTN_ACTION_CLICK, DiagonalMiddleBtnAction);
	lv_obj_set_size(DiagonalMiddleBtn, 450, 50);
	lv_btnm_set_toggle(DiagonalMiddleBtn, true, 1);
	lv_obj_set_pos(DiagonalMiddleBtn, 0, 100);
	lv_obj_align(DiagonalMiddleBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	//DiagonalDouble tab
	lv_obj_t *DiagonalDoubleBtn = lv_btn_create(DiagonalDoubleTab, NULL);
	lv_obj_t *label3 = lv_label_create(DiagonalDoubleBtn, NULL);
	lv_label_set_text(label3, "DiagonalDouble");
	lv_btn_set_action(DiagonalDoubleBtn, LV_BTN_ACTION_CLICK, DiagonalDoubleBtnAction);
	lv_obj_set_size(DiagonalDoubleBtn, 450, 50);
	lv_btnm_set_toggle(DiagonalDoubleBtn, true, 1);
	lv_obj_set_pos(DiagonalDoubleBtn, 0, 100);
	lv_obj_align(DiagonalDoubleBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	// Platform tab
	lv_obj_t *PlatformBtn = lv_btn_create(PlatformTab, NULL);
	lv_obj_t *label4 = lv_label_create(PlatformBtn, NULL);
	lv_label_set_text(label4, "Platform");
	lv_btn_set_action(PlatformBtn, LV_BTN_ACTION_CLICK, PlatformBtnAction);
	lv_obj_set_size(PlatformBtn, 450, 50);
	lv_btnm_set_toggle(PlatformBtn, true, 1);
	lv_obj_set_pos(PlatformBtn, 0, 100);
	lv_obj_align(PlatformBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	// SoloWP tab
	lv_obj_t *SoloWPBtn = lv_btn_create(SoloWPTab, NULL);
	lv_obj_t *label5 = lv_label_create(SoloWPBtn, NULL);
	lv_label_set_text(label5, "SoloWP");
	lv_btn_set_action(SoloWPBtn, LV_BTN_ACTION_CLICK, SoloWPBtnAction);
	lv_obj_set_size(SoloWPBtn, 450, 50);
	lv_btnm_set_toggle(SoloWPBtn, true, 1);
	lv_obj_set_pos(SoloWPBtn, 0, 100);
	lv_obj_align(SoloWPBtn, NULL, LV_ALIGN_CENTER, 0, 0); 

	// skills tab
	lv_obj_t *SkillsBtn = lv_btn_create(SkillsTab, NULL);
	lv_obj_t *label6 = lv_label_create(SkillsBtn, NULL);
	lv_label_set_text(label6, "Skills");
	lv_btn_set_action(SkillsBtn, LV_BTN_ACTION_CLICK, SkillsBtnAction);
	lv_obj_set_size(SkillsBtn, 450, 50);
	lv_btnm_set_toggle(SkillsBtn, true, 1);
	lv_obj_set_pos(SkillsBtn, 0, 100);
	lv_obj_align(SkillsBtn, NULL, LV_ALIGN_CENTER, 0, 0);


	std::cout << pros::millis() << ": generating paths..." << std::endl;
	//generatePaths();
	std::cout << pros::millis() << ": finished generating paths..." << std::endl;
	
	std::cout << pros::millis() << ": finished creating gui!" << std::endl;
	while (imu.is_calibrating() and pros::millis() < 5000)
	{
		pros::delay(10);
	}
	if (pros::millis() < 5000) std::cout << pros::millis() << ": finished calibrating!" << std::endl;
	else std::cout << pros::millis() << ": calibration failed, moving on" << std::endl;

	std::cout << pros::millis() << ": lift: " << frontLift.get_temperature() << std::endl;
	std::cout << pros::millis() << ": intake: " << intake.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLF: " << chassisLB.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLM: " << chassisLM.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLB: " << chassisLF.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRF: " << chassisRF.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRM: " << chassisRM.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRB: " << chassisRB.get_temperature() << std::endl;
  // Initialize chassis and auton selector
  chassis.initialize();
  default_constants();
  one_mogo_constants();
  two_mogo_constants();

  frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  pros::ADIDigitalOut piston1(CLAW_FRONT, true);
  pros::ADIDigitalOut piston2(CLAW_BACK, false);

  
}

void disabled() {
  pros::ADIDigitalOut piston1(CLAW_FRONT, true);
  pros::ADIDigitalOut piston2(CLAW_BACK, false);
}

void competition_initialize() {
  // . . .
}


void autonomous() {
  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
  chassis.reset_drive_sensor(); // Reset drive sensors to 0
  //chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.
  setChassisBreakMode(pros::E_MOTOR_BRAKE_HOLD);
  frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  if(autonSelection == autonStates::off) {
		autonSelection = autonStates::Skills;
	}
	switch(autonSelection) {
		case autonStates::Diagonal:
			Diagonal();
			break;
		case autonStates::DiagonalMiddle:
			DiagonalMiddle();
			break;
		case autonStates::DiagonalDouble:
			DiagonalDouble();
			break;
		case autonStates::Platform:
			Platform();
			break;
		case autonStates::SoloWP:
			SoloWP();
			break;
		case autonStates::Skills:
			skills();
			break;
		case autonStates::test:
			test();
			//turn_example();
			//swing_example();
			break;
		default:
			break;
	}
}


void opcontrol() {
  chassis.set_drive_brake(MOTOR_BRAKE_COAST);
  frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  	bool intakeSpin = true;
	bool chassisBreak = false;
	while(true) {
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		bool frontLiftUp = master.get_digital(DIGITAL_L1) == 1;
		bool frontLiftDownManual = master.get_digital(DIGITAL_L2) == 1;
		bool frontLiftDown = master.get_digital(DIGITAL_DOWN) == 1;

		bool backClampDown = master.get_digital_new_press(DIGITAL_R1) == 1;
		bool backClampUp = master.get_digital_new_press(DIGITAL_R2) == 1;

		bool clampDown = master.get_digital_new_press(DIGITAL_RIGHT) == 1;
		bool clampUp = master.get_digital_new_press(DIGITAL_Y) == 1;

		bool intakeOut = master.get_digital_new_press(DIGITAL_UP) == 1;
		bool chassisBreakBtn = master.get_digital_new_press(DIGITAL_LEFT) == 1;
		bool intake = master.get_digital_new_press(DIGITAL_X) == 1;

		//chassis move
		chassisDrive(left, right);
		//chassis.tank();

		
		//front Lift code
		//front Lift code
		if (frontLiftUp && intakeSpin) {
			frontliftMove(1);
			moveIntake(600);
			frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		} else if(frontLiftUp && !intakeSpin) {
			frontliftMove(1);
			frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		} else if (frontLiftDown) {
			frontliftMove(-1);
			frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		} else if (frontLiftDownManual) {
			manualLift(600);
			moveIntake(0);
			frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			std::cout << pros::millis() << ": liftPos: " << frontLift.get_brake_mode() << std::endl;
		} else {
			frontliftMove(0);
			frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		}

		// if(frontLift.get_position() >= -10 && frontLift.get_position() <= 50) {
		// 	frontLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		// } 



		//clamp on lift move
		if (clampDown) {
			moveClamp(true);
		} else if(clampUp) {
			moveClamp(false);
		}

		//back clamp move
		if(backClampDown) {
			moveBackClamp(true);
		} else if(backClampUp) {
			moveBackClamp(false);
		}

		if(intakeOut) {
			moveIntake(-600);
		}

		if(intake) {
			intakeSpin = !intakeSpin;
		}
		
		if(chassisBreakBtn) {
			chassisBreak = !chassisBreak;
		}

		if(chassisBreak) {
			setChassisBreakMode(pros::E_MOTOR_BRAKE_BRAKE);
		} else if(!chassisBreak) {
			setChassisBreakMode(pros::E_MOTOR_BRAKE_COAST);
		}

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
