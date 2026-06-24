#include "health_component.hpp"
#include "stats_component.hpp"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    void HealthComponent::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_health", "new_health"), &HealthComponent::set_health);
        ClassDB::bind_method(D_METHOD("get_health"), &HealthComponent::get_health);
        ClassDB::bind_method(D_METHOD("add_health", "p_health"), &HealthComponent::add_health);

        ClassDB::bind_method(D_METHOD("is_alive", "p_emit_signal"), &HealthComponent::is_alive);

        ClassDB::bind_method(D_METHOD("set_max_health", "p_max_health"), &HealthComponent::set_max_health);
        ClassDB::bind_method(D_METHOD("get_max_health"), &HealthComponent::get_max_health);

        ClassDB::bind_method(D_METHOD("change_health", "p_new_max", "p_new_current"), &HealthComponent::change_health);
        
        ClassDB::bind_method(D_METHOD("set_regeneration", "p_amount", "p_interval"), &HealthComponent::set_regeneration);

        ClassDB::bind_method(D_METHOD("set_regen_amount", "p_amount"), &HealthComponent::set_regen_amount);
        ClassDB::bind_method(D_METHOD("get_regen_amount"), &HealthComponent::get_regen_amount);
        ClassDB::bind_method(D_METHOD("set_regen_interval", "p_interval"), &HealthComponent::set_regen_interval);
        ClassDB::bind_method(D_METHOD("get_regen_interval"), &HealthComponent::get_regen_interval);

        ClassDB::add_property("HealthComponent", PropertyInfo(Variant::FLOAT, "current_health"), "set_health", "get_health");
        ClassDB::add_property("HealthComponent", PropertyInfo(Variant::FLOAT, "max_health"), "set_max_health", "get_max_health");

        ClassDB::add_property("HealthComponent", PropertyInfo(Variant::FLOAT, "regen_amount"), "set_regen_amount", "get_regen_amount");
        ClassDB::add_property("HealthComponent", PropertyInfo(Variant::FLOAT, "regen_interval"), "set_regen_interval", "get_regen_interval");

        ClassDB::bind_method(D_METHOD("take_damage", "p_base_damage"), &HealthComponent::take_damage);

        ADD_SIGNAL(MethodInfo("died"));
        ADD_SIGNAL(MethodInfo("health_changed", PropertyInfo(Variant::FLOAT, "next_health"), PropertyInfo(Variant::FLOAT, "prev_health")));
    }

    HealthComponent::HealthComponent()
    :   max_health(100.0f),
        current_health(100.0f),
        regen_amount(1.0f),   
        regen_interval(1.0f),
        regen_timer(0.0)
    {
        UtilityFunctions::print("HealthComponent created");
    }

    HealthComponent::~HealthComponent()
    {
        UtilityFunctions::print("HealthComponent deleted");
    }

    void HealthComponent::_process(double p_delta) {
        if (regen_amount <= 0.0f || regen_interval <= 0.0f) { return; }
        
        if (current_health >= max_health) {
            regen_timer = 0.0;
            return;
        }

        regen_timer += p_delta;

        if (regen_timer >= regen_interval) {
            regen_timer -= regen_interval; 
            add_health(regen_amount);
        }
    }

    void HealthComponent::set_health(float p_health)
    {
        float prev_health = current_health;
        current_health = std::clamp(p_health, 0.0f, max_health);

        emit_signal("health_changed", current_health, prev_health);
        is_alive(true);
    }

    float HealthComponent::get_health() const
    {
        return current_health;
    }

    float HealthComponent::add_health(float p_health)
    {
        float prev_health = current_health;
        current_health += p_health;
        current_health = std::clamp(current_health, 0.0f, max_health);

        emit_signal("health_changed", current_health, prev_health);
        is_alive(true);

        return current_health;
    }
    
    bool HealthComponent::is_alive(bool p_emit_signal)
    {
        if(current_health > 0){
            return true;
        }
        else if(p_emit_signal){
            emit_signal("died");
            UtilityFunctions::print("Entity is dead!");
        }
        return false;
    }

    

    void HealthComponent::set_max_health(float p_max_health) 
    {
        max_health = std::max(0.1f, p_max_health);
        current_health = std::clamp(current_health, 0.0f, max_health);
    }

    float HealthComponent::get_max_health() const 
    {
        return max_health;
    }

    void HealthComponent::change_health(float p_new_max, float p_new_current) 
    {
        max_health = std::max(0.1f, p_new_max);
        current_health = std::clamp(p_new_current, 0.0f, max_health);
    }
    
    void HealthComponent::set_regeneration(float p_amount, float p_interval)
    {
        // If the regeneration interval or the amount of added health <= 0, then regeneration is considered disabled.
        regen_amount = p_amount;
        regen_interval = p_interval;
    }

    void HealthComponent::set_regen_amount(float p_amount) { regen_amount = p_amount; }
    float HealthComponent::get_regen_amount() const { return regen_amount; }

    void HealthComponent::set_regen_interval(float p_interval) { regen_interval = p_interval; }
    float HealthComponent::get_regen_interval() const { return regen_interval; }

    bool HealthComponent::take_damage(float p_damage) {
        if (current_health <= 0.0f) { return false; }

        StatsComponent* stats = get_node<StatsComponent>("../ActorStats");

        if (stats != nullptr) {
            if (stats->check_evade()) {
                UtilityFunctions::print("Evaded! No damage taken.");
                return false;
            }

            float final_damage = stats->damage_reduction_calculation(p_damage);
            
            add_health(-final_damage);
            
            UtilityFunctions::print("Took damage: ", final_damage, " | Remaining HP: ", current_health);
        } else {
            add_health(-p_damage);
        }
        return true;
    }

} // namespace godot
