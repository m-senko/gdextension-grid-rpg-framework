#pragma  once

#include "Components/base_component.hpp"
#include <godot_cpp/variant/vector2i.hpp>


namespace godot {

class CombatComponent : public BaseComponent {
    GDCLASS(CombatComponent, BaseComponent);

private:
    float base_attack_damage = 10.0f;

protected:
    static void _bind_methods();

public:
    CombatComponent();
    ~CombatComponent();

    
    void attack(Vector2i p_global_cell);
};

} // namespace godot
