#pragma once

#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>
#include <optional>
#include <map>

namespace bmapping::button {
    typedef std::optional<std::function<void()>> keybind_method_t;

    typedef struct keybind_actions_s {
        keybind_method_t onPress = std::nullopt;
        keybind_method_t whileHeld = std::nullopt;
        keybind_method_t onRelease = std::nullopt;
    } keybind_actions_s_t;

    typedef struct keybind_state {
        bool isPressed = false;
        bool isHeld = false;
        bool wasPressed = false;
    } keybind_state_s_t;

    typedef struct keybind_s {
        std::optional<pros::controller_digital_e_t> action_key = std::nullopt;
        keybind_actions_s_t actions;
        keybind_state_s_t state;
    } keybind_s_t;

    class ButtonHandler {
        private:
            std::map<pros::controller_digital_e_t, keybind_s_t> keybinds;
            pros::Controller& controller;
            int delay = 10;

        public:
            ButtonHandler(pros::Controller& controller);
            void registerHotkey(pros::controller_digital_e_t action_key, pros::controller_digital_e_t key, keybind_actions_s_t keybind);
            void registerKeybind(pros::controller_digital_e_t key, keybind_actions_s_t keybind);
            void setDelay(int interval);
            int getDelay() const;
            void reset();
            void update();
        };
}
