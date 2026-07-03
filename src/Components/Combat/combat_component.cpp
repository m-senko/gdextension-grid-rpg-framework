#include "combat_component.hpp"
#include "actor.hpp"
#include "grid_map_singleton.hpp"
#include "health_component.hpp"
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

    ClassDB::bind_method(D_METHOD("set_base_attack_damage", "p_damage"), &CombatComponent::set_base_attack_damage);
    ClassDB::bind_method(D_METHOD("get_base_attack_damage"), &CombatComponent::get_base_attack_damage);

    ClassDB::add_property("CombatComponent", PropertyInfo(Variant::FLOAT, "base_attack_damage"), "set_base_attack_damage", "get_base_attack_damage");
}

CombatComponent::CombatComponent() {}
CombatComponent::~CombatComponent() {}

void CombatComponent::_on_actor_ready(Actor* p_owner) {
    if (p_owner == nullptr) return;

    stats_component = p_owner->get_component<StatsComponent>();
}

void CombatComponent::set_base_attack_damage(float p_damage) {
    base_attack_damage = std::max(0.0f, p_damage);
}

float CombatComponent::get_base_attack_damage() const {
    return base_attack_damage;
}

Dictionary CombatComponent::attack(Vector2i p_global_cell) {
    Dictionary report;
        report["result"] = HealthComponent::DAMAGE_INVALID;
        report["final"] = 0.0f;
        report["absorbed"] = 0.0f;

    GridMapSingleton* grid_map = GridMapSingleton::get_singleton();
    if (grid_map == nullptr) { return report; }

    Node* occupant = grid_map->get_occupant(p_global_cell);
    if (occupant == nullptr) { return report; }

    Actor* target_actor = Object::cast_to<Actor>(occupant);
    if (target_actor == nullptr) { return report; }

    HealthComponent* target_health = target_actor->get_component<HealthComponent>();
    if (target_health == nullptr) { return report; }

    float final_damage = base_attack_damage;

    if (stats_component != nullptr) {
        float strength_bonus = stats_component->get_attribute(StatsComponent::ATTR_PHYSICAL_ATTACK_BONUS);
        
        final_damage += strength_bonus;
        
    }
    
    UtilityFunctions::print("CombatComponent: Inflicting scaled damage: ", final_damage, " onto target.");
    return target_health->take_damage(final_damage);
}

} // namespace godot
