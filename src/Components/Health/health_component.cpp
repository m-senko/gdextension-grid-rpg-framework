#include "health_component.hpp"
#include "stats_component.hpp"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

// TODO: Architectural improvements for the HealthComponent (implement later):
// 1. Add an enum for DamageResult (DAMAGE_INVALID, DAMAGE_APPLIED, DAMAGE_EVADED, DAMAGE_BLOCKED) 
//    and register it via VARIANT_ENUM_CAST / BIND_ENUM_CONSTANT.
// 2. Change the return type of take_damage from bool to Dictionary.
//    This will allow returning a detailed report {result, final_damage, absorbed_damage}
//    fast and without RefCounted overhead in both C++ and GDScript.
// 3. Add a "damage_received"(final_damage, absorbed_damage, result) signal
//    for easy rendering of floating damage/armor text in the UI.

namespace godot
{

    void HealthComponent::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_health", "new_health"), &HealthComponent::set_health);
        ClassDB::bind_method(D_METHOD("get_health"), &HealthComponent::get_health);

        ClassDB::bind_method(D_METHOD("is_alive"), &HealthComponent::is_alive);

        ClassDB::bind_method(D_METHOD("set_max_health", "p_max_health"), &HealthComponent::set_max_health);
        ClassDB::bind_method(D_METHOD("get_max_health"), &HealthComponent::get_max_health);

        ClassDB::add_property("HealthComponent", PropertyInfo(Variant::FLOAT, "current_health"), "set_health", "get_health");
        ClassDB::add_property("HealthComponent", PropertyInfo(Variant::FLOAT, "max_health"), "set_max_health", "get_max_health");

        ClassDB::bind_method(D_METHOD("take_damage", "p_base_damage"), &HealthComponent::take_damage);

        ADD_SIGNAL(MethodInfo("died"));
        ADD_SIGNAL(MethodInfo("health_changed", PropertyInfo(Variant::FLOAT, "next_health"), PropertyInfo(Variant::FLOAT, "prev_health"), PropertyInfo(Variant::FLOAT, "max_health")));
    }

    HealthComponent::HealthComponent(){}

    HealthComponent::~HealthComponent(){}

    void HealthComponent::_on_actor_ready(Actor* p_owner)
    {
        stats_component = p_owner->get_component<StatsComponent>();

        if (stats_component != nullptr) {
            if (!stats_component->is_connected("stats_changed", Callable(this, "_on_stats_changed"))) 
            {
                stats_component->connect("stats_changed", Callable(this, "_on_stats_changed"));
            }
        }
        update_max_health();
        current_health = max_health;
    }


    void HealthComponent::update_max_health() {
        float target_max = base_health;

        if (stats_component != nullptr) {
            float stat_hp = stats_component->get_stat_hp();
            float percent_mod = stats_component->get_health_percent_modifier();
            float flat_mod = stats_component->get_base_health_modifier();

            target_max = (base_health + stat_hp) * percent_mod + flat_mod;
        }
        
        set_max_health(target_max);
    }

    void HealthComponent::_on_stats_changed() {
        update_max_health();
    }

    void HealthComponent::set_health(float p_health)
    {
        float clamped_health = std::clamp(p_health, 0.f, max_health);
        if(current_health == clamped_health) { return; }
        
        float prev_health = current_health;
        current_health = clamped_health;

        emit_signal("health_changed", current_health, prev_health, max_health);

        if (current_health <= 0.0f && prev_health > 0) {
            emit_signal("died");
        }
    }

    float HealthComponent::get_health() const
    {
        return current_health;
    }
    
    bool HealthComponent::is_alive()const { return current_health > 0; }

    void HealthComponent::set_max_health(float p_max_health) 
    {
        float old_max = max_health;
        max_health = std::max(0.1f, p_max_health);
        float percent = (old_max > 0.0f) ? (current_health / old_max) : 1.0f;
        set_health(max_health * percent); 
    }

    float HealthComponent::get_max_health() const 
    {
        return max_health;
    }

    bool HealthComponent::take_damage(float p_base_damage) {
        if (!is_alive() || p_base_damage <= 0) { return false; }

        float final_damage = p_base_damage;

        if (stats_component != nullptr) {
            float evade_chance = stats_component->get_attribute(StatsComponent::ATTR_EVADE_CHANCE);
            if (UtilityFunctions::randf() * 100 < evade_chance) {
                return false;
            }

            float reduction = stats_component->get_attribute(StatsComponent::ATTR_DAMAGE_REDUCTION);
            final_damage = p_base_damage * (1 - (reduction / 100));
        }

        if (final_damage <= 0) { return true; }
        set_health(current_health - final_damage);

        return true;
    }

} // namespace godot
