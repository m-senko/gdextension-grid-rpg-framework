#include "combat_component.hpp"
#include "actor.hpp"
#include "health_component.hpp"
#include "grid_map_singleton.hpp"
#include <godot_cpp/core/class_db.hpp>

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
