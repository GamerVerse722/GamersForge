#include "main.h"
#include "bmapper/button.hpp"
#include "pros/misc.h"
#include "pros/screen.h"
#include <cstdlib>
#include <optional>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({-14, -13, -12});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup right_mg({18, 17, -16});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6

	using namespace bmapping::button;
	ButtonHandler button_handler(master);
	button_handler.registerKeybind(std::nullopt, pros::E_CONTROLLER_DIGITAL_RIGHT, {
		[&]() -> void {
			left_mg.move(-127);
		},
		[&]() -> void {
			pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 1, "%d", rand() % 101);
		},
		[&]() -> void {
			left_mg.move(0);
		}
	});

	button_handler.registerKeybind(pros::E_CONTROLLER_DIGITAL_DOWN, pros::E_CONTROLLER_DIGITAL_LEFT, {
		[&]() -> void {
			right_mg.move(127);
		},
		[&]() -> void {
			pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 2, "%d", rand() % 51);
		},
		[&]() -> void {
			right_mg.move(0);
		}
	});

	button_handler.start();
	
	while (true) {
		// pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs
		pros::delay(20);
	}
}