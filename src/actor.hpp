#pragma once

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>


namespace godot {

class Actor : public Node2D {
    GDCLASS(Actor, Node2D);

private:
    HashMap<StringName, Node*> components_cache;

protected:
    static void _bind_methods();

public:
    Actor();
    ~Actor();

    void _ready() override;

    template <typename T>
    T* get_component() {
        StringName class_name = T::get_class_static(); 
        
        if (Node** component = components_cache.getptr(class_name)) {
            return Object::cast_to<T>(*component); 
        }
        return nullptr;
}

    Node* get_component_gd(StringName p_class_name);
};

} // namespace godot
