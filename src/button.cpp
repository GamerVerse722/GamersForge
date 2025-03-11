#include "bmapper/button.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <iostream>

namespace bmapping::button {
    void ButtonHandler::registerKeybind(std::optional<pros::controller_digital_e_t> action_key, pros::controller_digital_e_t key, keybind_actions_s_t keybind_actions) {
        this->register_key_set.insert(key);

        keybind_s_t create_keybind;
        create_keybind.action_key = action_key;
        create_keybind.actions = keybind_actions;
        create_keybind.state = keybind_state_s_t{false, false, false};
        
        if (action_key.has_value()) {
            this->action_keybinds[key] = create_keybind;
        } else {
            this->keybinds[key] = create_keybind;
        }
    }

    template <typename... Buttons> bool ButtonHandler::areKeysPressed(Buttons... buttons) {
        return (... && this->controller.get_digital(buttons));
    }

    void ButtonHandler::update(pros::controller_digital_e_t key) {
        if (this->action_keybinds.contains(key)) {
            keybind_s_t& action_keybind = this->action_keybinds[key];

            action_keybind.state.wasPressed = action_keybind.state.isPressed;
            action_keybind.state.isPressed = this->areKeysPressed(key, action_keybind.action_key.value());
            action_keybind.state.isHeld = action_keybind.state.isPressed && action_keybind.state.wasPressed;
        }

        if (this->keybinds.contains(key)) {
            keybind_s_t& keybind = this->keybinds[key];

            keybind.state.wasPressed = keybind.state.isPressed;
            keybind.state.isPressed = this->controller.get_digital(key);
            if (this->action_keybinds.contains(key)) {
                keybind.state.isPressed = keybind.state.isPressed && !this->controller.get_digital(this->action_keybinds[key].action_key.value());
            }
            keybind.state.isHeld = keybind.state.isPressed && keybind.state.wasPressed;
        }
    }

    void ButtonHandler::run(pros::controller_digital_e_t key) {
        if (this->keybinds.contains(key)) {
            keybind_s_t& keybind = this->keybinds[key];
            if (keybind.state.isPressed && !keybind.state.wasPressed && keybind.actions.onPress.has_value()) {
                std::cout << "Keybind Running press" << std::endl;
                keybind.actions.onPress.value()();

            } else if (keybind.state.isHeld && keybind.actions.onHold.has_value()) {
                keybind.actions.onHold.value()();

            } else if (!keybind.state.isPressed && keybind.state.wasPressed && keybind.actions.onRelease.has_value()) {
                std::cout << "Keybind Running release" << std::endl;
                keybind.actions.onRelease.value()();
            }
        }

        if (this->action_keybinds.contains(key)) {
            keybind_s_t& action_keybind = this->action_keybinds[key];
            if (action_keybind.state.isPressed && !action_keybind.state.wasPressed && action_keybind.actions.onPress.has_value()) {
                std::cout << "Action Running press" << std::endl;
                action_keybind.actions.onPress.value()();

            } else if (action_keybind.state.isHeld && action_keybind.actions.onHold.has_value()) {
                action_keybind.actions.onHold.value()();

            } else if (!action_keybind.state.isPressed && action_keybind.state.wasPressed && action_keybind.actions.onRelease.has_value()) {
                std::cout << "Action Running release" << std::endl;
                action_keybind.actions.onRelease.value()();
                if (this->keybinds.contains(key)) {
                    keybind_s_t& keybind = this->keybinds[key];
                    if (keybind.state.isPressed && keybind.actions.onPress.has_value()) {
                        keybind.actions.onPress.value()();
                    }
                }
            }
        }
    }
    
    void ButtonHandler::start() {
        this->activated = true;
        pros::Task button_task([&] {
            while (this->activated) {
                for (auto key : this->register_key_set) {
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
        this->action_keybinds.clear();
        this->register_key_set.clear();
    }
}