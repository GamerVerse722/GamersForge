#pragma once

#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>
#include <optional>
#include <map>
#include <set>

namespace bmapping {
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
            template <typename... Buttons>bool areKeysPressed(Buttons... buttons);
            std::map<pros::controller_digital_e_t, keybind_s_t> action_keybinds;
            std::map<pros::controller_digital_e_t, keybind_s_t> keybinds;
            std::set<pros::controller_digital_e_t> register_key_set;
            pros::Controller& controller;
            bool activated = false;
            int delay = 10;

        public:
            /**
             * @brief Construct a new Button Handler object
             * 
             * @param controller The controller to listen to
             */
            ButtonHandler(pros::Controller& controller) : controller(controller) {};

            /**
             * @brief Register a keybind
             * 
             * @param action_key optional action key (acts like a ctrl, alt or shift)
             * @param key 
             * @param keybind_actions
             */
            void registerKeybind(std::optional<pros::controller_digital_e_t> action_key, pros::controller_digital_e_t key, keybind_actions_s_t keybind_actions);
            
            /** Update the state of the keybinds */
            void update(pros::controller_digital_e_t key);

            /** Run the keybinds */
            void run(pros::controller_digital_e_t key);

            /** Start the button handler */
            void start();

            /** Stop the button handler */
            void stop();

            /** @param interval in milliseconds */
            void setDelay(int interval);

            /** @return interval in milliseconds */
            int getDelay() const;

            /** Reset all keybinds */
            void reset();
        };
}
