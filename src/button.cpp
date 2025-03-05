#include "bmapper/button.hpp"
#include "pros/rtos.hpp"

namespace button {

    ButtonHandler::ButtonHandler(pros::Controller& controller) : controller(controller) {}

    bool ButtonHandler::areAllKeysPressed(const std::set<pros::controller_digital_e_t>& keys) {
        for (const auto& key : keys) {
            if (!controller.get_digital(key)) {
                return false; // If any key in the set is not pressed, return false
            }
        }
        return true;
    }

    void ButtonHandler::registerKeybind(std::set<pros::controller_digital_e_t> keys, KeybindActions keybind) {
        keybinds[keys] = keybind;
        keyStates[keys] = false; // Initialize state tracking
    }

    void ButtonHandler::setDelay(int interval) {
        delay = interval;
    }

    int ButtonHandler::getDelay() const {
        return delay;
    }

    void ButtonHandler::reset() {
        keybinds.clear();
        keyStates.clear();
    }

    void ButtonHandler::update() {
        for (auto& [keys, actions] : keybinds) {
            bool currentlyPressed = areAllKeysPressed(keys);

            // Handle onPress
            if (currentlyPressed && !keyStates[keys]) {
                if (actions.onPress) {
                    actions.onPress.value()();
                }
            }

            // Handle onRelease
            if (!currentlyPressed && keyStates[keys]) {
                if (actions.onRelease) {
                    actions.onRelease.value()();
                }
            }

            // Update state tracking
            keyStates[keys] = currentlyPressed;
        }

        pros::delay(delay);
    }

}
