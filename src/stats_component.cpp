#include "stats_component.hpp"
#include "health_component.hpp" 
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

namespace godot {

void StatsComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_strength", "p_value"), &StatsComponent::set_strength);
    ClassDB::bind_method(D_METHOD("get_strength"), &StatsComponent::get_strength);

    ClassDB::bind_method(D_METHOD("set_agility", "p_value"), &StatsComponent::set_agility);
    ClassDB::bind_method(D_METHOD("get_agility"), &StatsComponent::get_agility);

    ClassDB::bind_method(D_METHOD("set_vitality", "p_value"), &StatsComponent::set_vitality);
    ClassDB::bind_method(D_METHOD("get_vitality"), &StatsComponent::get_vitality);

    ClassDB::bind_method(D_METHOD("set_endurance", "p_value"), &StatsComponent::set_endurance);
    ClassDB::bind_method(D_METHOD("get_endurance"), &StatsComponent::get_endurance);

    ClassDB::bind_method(D_METHOD("update_health_component"), &StatsComponent::update_health_component);

    ClassDB::bind_method(D_METHOD("check_evade"), &StatsComponent::check_evade);
    ClassDB::bind_method(D_METHOD("damage_reduction_calculation"), &StatsComponent::damage_reduction_calculation);

    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "strength"), "set_strength", "get_strength");
    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "agility"), "set_agility", "get_agility");
    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "vitality"), "set_vitality", "get_vitality");
    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "endurance"), "set_endurance", "get_endurance");
}

StatsComponent::StatsComponent() 
:   strength(10),
    agility(10),
    vitality(10),
    endurance(10),
    hp_per_vitality(12.0f)
{}

StatsComponent::~StatsComponent() {}

void StatsComponent::set_strength(int p_value) { strength = std::max(1, p_value); }
int StatsComponent::get_strength() const { return strength; }

void StatsComponent::set_agility(int p_value) { agility = std::max(1, p_value); }
int StatsComponent::get_agility() const { return agility; }

void StatsComponent::set_vitality(int p_value) { 
    vitality = std::max(1, p_value); 
    update_health_component();
}
int StatsComponent::get_vitality() const { return vitality; }

void StatsComponent::set_endurance(int p_value) { endurance = std::max(1, p_value); }
int StatsComponent::get_endurance() const { return endurance; }

void StatsComponent::update_health_component() {
    HealthComponent* health = get_node<HealthComponent>("../HealthComponent");
    
    if (health != nullptr) {
        float new_max_hp = vitality * hp_per_vitality;
        
        float current_hp = health->get_health();
        float max_hp = health->get_max_health();
        float percent = current_hp / max_hp;
        
        health->change_health(new_max_hp, new_max_hp * percent);
    }
}

bool StatsComponent::check_evade() const {
    float evade_chance = std::min(50.0f, agility * 0.5f);
    float random_roll = UtilityFunctions::randf_range(0.0f, 100.0f);
    
    return random_roll < evade_chance;
}

float StatsComponent::damage_reduction_calculation(float p_damage) const {
    float reduction_percent = std::min(75.0f, endurance * 1.0f);
    
    float reduction_damage = p_damage * (1.0f - (reduction_percent / 100.0f));
    return std::max(0.0f, reduction_damage);
}

} // namespace godot
