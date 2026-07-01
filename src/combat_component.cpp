#include "combat_component.hpp"
#include "actor.hpp"
#include "health_component.hpp"
#include "grid_map_singleton.hpp"
#include <godot_cpp/core/class_db.hpp>

// TODO: Integration with StatsComponent for RPG damage scaling:
// 1. In initialization/_ready, try to find and cache a pointer to the owner's StatsComponent.
// 2. Add and maintain a 'base_attack_damage' field (with getters/setters) to act as a fallback.
// 3. Implement dynamic damage calculation logic inside the attack(p_global_cell) method:
//    - If StatsComponent is available: calculate final damage by combining 'base_attack_damage' with stats/attributes modifiers.
//    - If StatsComponent is missing: fall back to using just the 'base_attack_damage' value.

namespace godot {

void CombatComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("attack", "p_global_cell"), &CombatComponent::attack);
}

CombatComponent::CombatComponent() {}
CombatComponent::~CombatComponent() {}

void CombatComponent::attack(Vector2i p_global_cell) {

    GridMapSingleton* grid_map = GridMapSingleton::get_singleton();
    if (grid_map == nullptr) { return; }


    Node* occupant = grid_map->get_occupant(p_global_cell);
    if (occupant == nullptr) { return; }

    Actor* target_actor = Object::cast_to<Actor>(occupant);
    if (target_actor == nullptr) { return; }

    HealthComponent* target_health = target_actor->get_component<HealthComponent>();

    if (target_health != nullptr) {
        target_health->take_damage(base_attack_damage);
    }
}

} // namespace godot
