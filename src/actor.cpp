#include "actor.hpp"

namespace godot {

void Actor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_component", "p_class_name"), &Actor::get_component_gd);
}

Actor::Actor() {}
Actor::~Actor() {}

void Actor::_ready() {
    int child_count = get_child_count();
    for (int i = 0; i < child_count; ++i) {
        Node* child = get_child(i);
        if (child == nullptr) continue;

        StringName child_class = child->get_class();
        
        components_cache[child_class] = child;
    }
}

Node* Actor::get_component_gd(StringName p_class_name) {
    if (Node** component = components_cache.getptr(p_class_name)) {
        return *component; 
    }
    return nullptr;
}

} // namespace godot
