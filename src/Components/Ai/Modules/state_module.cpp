#include "state_module.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

namespace godot {

void StateAIModule::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_detection_range", "p_range"), &StateAIModule::set_detection_range);
    ClassDB::bind_method(D_METHOD("get_detection_range"), &StateAIModule::get_detection_range);

    ClassDB::add_property("StateAIModule", PropertyInfo(Variant::INT, "detection_range"), "set_detection_range", "get_detection_range");

    BIND_ENUM_CONSTANT(AI_WANDER);
    BIND_ENUM_CONSTANT(AI_CHASE);
    BIND_ENUM_CONSTANT(AI_COMBAT);
}

StateAIModule::StateAIModule() {}
StateAIModule::~StateAIModule() {}

void StateAIModule::set_detection_range(int p_range) {
    detection_range = std::max(1, p_range);
}

int StateAIModule::get_detection_range() const {
    return detection_range;
}

StateAIModule::AIState StateAIModule::evaluate_state(int p_distance) const {
    if (p_distance == 1) {
        return AI_COMBAT;
    }
    
    if (p_distance <= detection_range) {
        return AI_CHASE;
    }
    
    return AI_WANDER;
}

} // namespace godot
