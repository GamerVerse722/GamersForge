#include "main.h"

#include "gamers-forge/proslogger.hpp"
#include "gamers-forge/bmapper.hpp"

#include <queue>

std::queue<std::string> logs_sub;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({-1, 2, 3});
pros::MotorGroup right_mg({4, -5, -6});

BMapper::ButtonHandler button_handler(master);
PROSLogger::Logger logger("PROSLogger");

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	PROSLogger::Manager::subscribe([&](const PROSLogger::LoggerEvent event) {
        logs_sub.push(event.message);
    });

    PROSLogger::Manager::setLevel(PROSLogger::LogLevel::DEBUG);

    logger.debug("Debug Level");
    // result: [00:00:000] [Debug] [PROSLogger] Debug Level

    logger.info("Info Level");
    // result: [00:00:000] [Info] [PROSLogger] Info Level

    logger.warn("Warn Level");
    // result: [00:00:000] [Warn] [PROSLogger] Warn Level

    logger.error("Error Level");
    // result: [00:00:000] [Error] [PROSLogger] Error Level

    PROSLogger::Manager::setLevel(PROSLogger::LogLevel::WARN);

    logger.debug("Debug Level");
    // result: None

    logger.info("Info Level");
    // result: None

    logger.warn("Warn Level");
    // result: [00:00:000] [Warn] [PROSLogger] Warn Level

    logger.error("Error Level");
    // result: [00:00:000] [Error] [PROSLogger] Error Level
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
    PROSLogger::Manager::setLevel(PROSLogger::LogLevel::WARN);

    button_handler.bind(pros::E_CONTROLLER_DIGITAL_LEFT)
        .setCategory("RightMotor")
        .onPress([&]() -> void {
            right_mg.move(50);
        })
        .onHold([&]() -> void {
            pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 1, "%d", rand() % 101);
        })
        .onRelease([&]() -> void {
            right_mg.move(0);
        });

    button_handler.bind(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_DOWN)
        .setCategory("RightMotor")
        .onPress([&]() -> void {
            right_mg.move(-50);
        })
        .onHold([&]() -> void {
            pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 2, "%d", rand() % 51);
        })
        .onRelease([&]() -> void {
            right_mg.move(0);
        });

    button_handler.start();

    while (true) {
        if (!logs_sub.empty()) {
            pros::lcd::set_text(1, logs_sub.front());
            logs_sub.pop();

        } else {
            pros::lcd::set_text(1, "");
        }

        pros::delay(1000);
    }
}