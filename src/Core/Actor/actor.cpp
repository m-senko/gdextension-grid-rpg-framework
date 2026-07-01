#include "actor.hpp"
#include "base_component.hpp"

namespace godot {

void Actor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_component", "p_class_name"), &Actor::get_component_gd);
}

Actor::Actor() {}
Actor::~Actor() {}

void Actor::_notification(int p_what) {
    if (p_what == NOTIFICATION_POST_ENTER_TREE) {
        int child_count = get_child_count();
        
        for (int i = 0; i < child_count; ++i) {
            Node* child = get_child(i);
            if (child == nullptr) continue;
            
            BaseComponent* comp_check = Object::cast_to<BaseComponent>(child);
            if (comp_check != nullptr) {
                components_cache[child->get_class()] = child;
            }
        }

        for (int i = 0; i < child_count; ++i) {
            BaseComponent* component = Object::cast_to<BaseComponent>(get_child(i));
            if (component != nullptr) {
                component->_on_actor_ready(this); 
            }
        }
    } 
    Node2D::_notification(p_what);
}

Node* Actor::get_component_gd(StringName p_class_name) {
    if (Node** component = components_cache.getptr(p_class_name)) {
        return *component; 
    }
    return nullptr;
}

} // namespace godot
