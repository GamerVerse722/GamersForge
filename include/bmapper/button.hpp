#include "pros/misc.h"
#include <vector>
#include <functional>

namespace button {
    class ActionKey {
        public:
            ActionKey();
    };

    class Button {
        public:
            Button(ActionKey actionKey);
            void registerKeybinds(pros::controller_digital_e_t button, std::function<void()> callback);
            void registerKeybind(std::vector<pros::controller_digital_e_t> buttons);
            void reset();
    };
}
