#include "bmapper/button.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"

namespace bmapping::button {
    void ButtonHandler::registerKeybind(std::optional<pros::controller_digital_e_t> action_key, pros::controller_digital_e_t key, keybind_actions_s_t keybind_actions) {
        keybind_s_t create_keybind;
        create_keybind.action_key = action_key;
        create_keybind.actions = keybind_actions;
        create_keybind.state = keybind_state_s_t{false, false, false};

        this->keybinds[key] = create_keybind;
    }

    template <typename... Buttons> bool ButtonHandler::areKeysPressed(Buttons... buttons) {
        return (... && this->controller.get_digital(buttons));
    }

    void ButtonHandler::update(pros::controller_digital_e_t key) {
        keybind_s_t& keybind = this->keybinds[key];
        keybind.state.wasPressed = keybind.state.isPressed;
        
        if (keybind.action_key.has_value()) {
            keybind.state.isPressed = this->areKeysPressed(key, keybind.action_key.value());
        } else {
            keybind.state.isPressed = this->controller.get_digital(key);
        }

        keybind.state.isHeld = keybind.state.isPressed && keybind.state.wasPressed;
    }

    void ButtonHandler::run(pros::controller_digital_e_t key) {
        keybind_s_t& keybind = this->keybinds[key];

        if (keybind.state.isPressed != keybind.state.wasPressed && keybind.actions.onPress.has_value()) {
            keybind.actions.onPress.value()();

        } else if (keybind.state.isHeld && keybind.actions.onHold.has_value()) {
            keybind.actions.onHold.value()();

        } else if (!keybind.state.isPressed && keybind.state.wasPressed && keybind.actions.onRelease.has_value()) {
            keybind.actions.onRelease.value()();
        }
    }
    
    void ButtonHandler::start() {
        this->activated = true;
        pros::Task button_task([&] {
            while (this->activated) {
                for (auto& [key, keybind] : this->keybinds) {
                    this->update(key);
                    this->run(key);
                }
                pros::delay(this->delay);
            }
        });
    }

    void ButtonHandler::stop() {
        this->activated = false;
    }

    void ButtonHandler::setDelay(int interval) {
        this->delay = interval;
    }

    int ButtonHandler::getDelay() const {
        return this->delay;
    }

    void ButtonHandler::reset() {
        this->keybinds.clear();
    }
}