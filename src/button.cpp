#include "bmapper/button.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <iostream>
#include <optional>

namespace bmapping {
    KeybindBuilder::KeybindBuilder(pros::controller_digital_e_t key, bmapping::ButtonHandler& handler, std::optional<pros::controller_digital_e_t> modifier): key(key), handler(handler), actionKey(modifier) {
        
    }

    KeybindBuilder::~KeybindBuilder() {
        if (!applied) {
            handler.registerKeybind(key, actions, actionKey, this->category);
        }
    }

    KeybindBuilder& KeybindBuilder::onPress(keybind_method_t callback) {
        actions.onPress = callback;
        return *this;
    }

    KeybindBuilder& KeybindBuilder::onHold(keybind_method_t callback) {
        actions.onHold = callback;
        return *this;
    }

    KeybindBuilder& KeybindBuilder::onRelease(keybind_method_t callback) {
        actions.onRelease = callback;
        return *this;
    }

    KeybindBuilder& KeybindBuilder::setCategory(std::string category) {
        this->category = category;
        return *this;
    }

    void KeybindBuilder::apply() {
        applied = true;
        handler.registerKeybind(key, actions, actionKey, this->category);
    }

    void ButtonHandler::registerKeybind(pros::controller_digital_e_t key, keybind_actions_s_t keybind_actions, std::optional<pros::controller_digital_e_t> action_key, std::string category) {
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
            if (keybind.state.isPressed && !keybind.state.wasPressed && keybind.actions.onPress) {
                std::cout << "Keybind Running press" << std::endl;
                keybind.actions.onPress();

            } else if (keybind.state.isHeld && keybind.actions.onHold) {
                keybind.actions.onHold();

            } else if (!keybind.state.isPressed && keybind.state.wasPressed && keybind.actions.onRelease) {
                std::cout << "Keybind Running release" << std::endl;
                keybind.actions.onRelease();
            }
        }

        if (this->action_keybinds.contains(key)) {
            keybind_s_t& action_keybind = this->action_keybinds[key];
            if (action_keybind.state.isPressed && !action_keybind.state.wasPressed && action_keybind.actions.onPress) {
                std::cout << "Action Running press" << std::endl;
                action_keybind.actions.onPress();

            } else if (action_keybind.state.isHeld && action_keybind.actions.onHold) {
                action_keybind.actions.onHold();

            } else if (!action_keybind.state.isPressed && action_keybind.state.wasPressed && action_keybind.actions.onRelease) {
                std::cout << "Action Running release" << std::endl;
                action_keybind.actions.onRelease();
                if (this->keybinds.contains(key)) {
                    keybind_s_t& keybind = this->keybinds[key];
                    if (keybind.state.isPressed && keybind.actions.onPress) {
                        keybind.actions.onPress();
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