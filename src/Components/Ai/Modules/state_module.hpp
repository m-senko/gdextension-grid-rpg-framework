#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class StateAIModule : public Node {
    GDCLASS(StateAIModule, Node);

public:
    enum AIState {
        AI_WANDER = 0,
        AI_CHASE = 1,
        AI_COMBAT = 2
    };

private:
    int detection_range = 5; 

protected:
    static void _bind_methods();

public:
    StateAIModule();
    ~StateAIModule();

    AIState evaluate_state(int p_distance) const;

    void set_detection_range(int p_range);
    int get_detection_range() const;
};

} // namespace godot

VARIANT_ENUM_CAST(StateAIModule::AIState);
