#pragma once

#include "pros/misc.hpp"
#include <functional>
#include <optional>
#include <map>
#include <set>

namespace button {
    struct KeybindActions {
        std::optional<std::function<void()>> onPress = std::nullopt;
        std::optional<std::function<void()>> onRelease = std::nullopt;
    };

    class ButtonHandler {
    private:
        std::map<std::set<pros::controller_digital_e_t>, KeybindActions> keybinds;
        std::map<std::set<pros::controller_digital_e_t>, bool> keyStates;
        pros::Controller& controller;
        int delay = 10;

        bool areAllKeysPressed(const std::set<pros::controller_digital_e_t>& keys);

    public:
        ButtonHandler(pros::Controller& controller);

        void registerKeybind(std::set<pros::controller_digital_e_t> keys, KeybindActions keybind);
        void setDelay(int interval);
        int getDelay() const;
        void reset();
        void update();
    };
}
