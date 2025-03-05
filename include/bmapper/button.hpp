#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>

namespace button {
    class ActionKey {
        public:
            ActionKey();
    };

    class Button {
        private:
            pros::Controller controller;

        public:
            Button(pros::Controller& controller);
            void registerKeybind(ActionKey actionKey, pros::controller_digital_e_t button, std::function<void()> callback);
            void registerKeybind(pros::controller_digital_e_t button, std::function<void()> callback);
            void reset();
    };
}
