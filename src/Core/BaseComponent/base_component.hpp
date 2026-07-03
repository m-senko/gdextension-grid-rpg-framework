#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class Actor; 

class BaseComponent : public Node {
    GDCLASS(BaseComponent, Node);

protected:
    static void _bind_methods();

public:
    BaseComponent();
    virtual ~BaseComponent();

    virtual void _on_actor_ready(Actor* p_owner);
};

} // namespace godot

