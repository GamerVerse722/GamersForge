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
        keybind_method_t onHold = std::nullopt;
        keybind_method_t onRelease = std::nullopt;
    } keybind_actions_s_t;

    typedef struct keybind_state {
        bool isPressed = false;
        bool wasPressed = false;
        bool isHeld = false;
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
            bool activated = false;
            int delay = 10;

        public:
            ButtonHandler(pros::Controller& controller) : controller(controller) {};
            void registerKeybind(std::optional<pros::controller_digital_e_t> action_key, pros::controller_digital_e_t key, keybind_actions_s_t keybind_actions);
            template <typename... Buttons>bool areKeysPressed(Buttons... buttons);
            void update(pros::controller_digital_e_t key);
            void run(pros::controller_digital_e_t key);
            void start();
            void stop();
            void setDelay(int interval);
            int getDelay() const;
            void reset();
        };
}
