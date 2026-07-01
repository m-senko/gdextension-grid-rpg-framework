#pragma  once

#include "base_component.hpp"
#include <godot_cpp/variant/vector2i.hpp>

// TODO: Integration with StatsComponent for RPG damage scaling:
// 1. In initialization/_ready, try to find and cache a pointer to the owner's StatsComponent.
// 2. Add and maintain a 'base_attack_damage' field (with getters/setters) to act as a fallback.
// 3. Implement dynamic damage calculation logic inside the attack(p_global_cell) method:
//    - If StatsComponent is available: calculate final damage by combining 'base_attack_damage' with stats/attributes modifiers.
//    - If StatsComponent is missing: fall back to using just the 'base_attack_damage' value.

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
