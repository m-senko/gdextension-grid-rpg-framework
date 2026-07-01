#include "stats_component.hpp"
#include <algorithm>

namespace godot {

void StatsComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_balance_config", "p_config"), &StatsComponent::set_balance_config);
    ClassDB::bind_method(D_METHOD("get_balance_config"), &StatsComponent::get_balance_config);

    ClassDB::bind_method(D_METHOD("set_strength", "p_value"), &StatsComponent::set_strength);
    ClassDB::bind_method(D_METHOD("get_strength"), &StatsComponent::get_strength);
    ClassDB::bind_method(D_METHOD("set_agility", "p_value"), &StatsComponent::set_agility);
    ClassDB::bind_method(D_METHOD("get_agility"), &StatsComponent::get_agility);
    ClassDB::bind_method(D_METHOD("set_vitality", "p_value"), &StatsComponent::set_vitality);
    ClassDB::bind_method(D_METHOD("get_vitality"), &StatsComponent::get_vitality);
    ClassDB::bind_method(D_METHOD("set_endurance", "p_value"), &StatsComponent::set_endurance);
    ClassDB::bind_method(D_METHOD("get_endurance"), &StatsComponent::get_endurance);

    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "strength"), "set_strength", "get_strength");
    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "agility"), "set_agility", "get_agility");
    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "vitality"), "set_vitality", "get_vitality");
    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::INT, "endurance"), "set_endurance", "get_endurance");

    ClassDB::add_property("StatsComponent", PropertyInfo(Variant::OBJECT, "balance_config", PROPERTY_HINT_RESOURCE_TYPE, "StatsBalanceConfig"), "set_balance_config", "get_balance_config");
    
    ADD_SIGNAL(MethodInfo("stats_changed"));
}

StatsComponent::StatsComponent() {}
StatsComponent::~StatsComponent() {}

void StatsComponent::set_balance_config(const Ref<StatsBalanceConfig>& p_config) {
    balance_config = p_config;
    emit_signal("stats_changed");
}

Ref<StatsBalanceConfig> StatsComponent::get_balance_config() const {
    return balance_config;
}

void StatsComponent::add_health_bonus(float p_flat, float p_percent)
{
    base_health_modifier += p_flat;
    health_percent_modifier += p_percent;
    emit_signal("stats_changed");
}

float StatsComponent::get_stat_hp() const
{
    float hp_mult = balance_config.is_valid() ? balance_config->get_hp_per_vitality() : 12.0f;
    return vitality * hp_mult;
}

float StatsComponent::get_health_percent_modifier() const { return health_percent_modifier; }

float StatsComponent::get_base_health_modifier() const { return base_health_modifier; }


void StatsComponent::set_strength(int p_value) { strength = std::max(1, p_value); emit_signal("stats_changed"); }
int StatsComponent::get_strength() const { return strength; }

void StatsComponent::set_agility(int p_value) { agility = std::max(1, p_value); emit_signal("stats_changed"); }
int StatsComponent::get_agility() const { return agility; }

void StatsComponent::set_vitality(int p_value) { vitality = std::max(1, p_value); emit_signal("stats_changed"); }
int StatsComponent::get_vitality() const { return vitality; }

void StatsComponent::set_endurance(int p_value) { endurance = std::max(1, p_value); emit_signal("stats_changed"); }
int StatsComponent::get_endurance() const { return endurance; }

float StatsComponent::get_attribute(Attribute p_attribute) const {
    float hp_mult = balance_config.is_valid() ? balance_config->get_hp_per_vitality() : 10.0f;
    float speed_mult = balance_config.is_valid() ? balance_config->get_speed_per_agility() : 1.5f;
    float reduction_mult = balance_config.is_valid() ? balance_config->get_reduction_per_endurance() : 1.0f;
    float evade_mult = balance_config.is_valid() ? balance_config->get_evade_per_agility() : 0.5f;

    switch (p_attribute) {
        case ATTR_MAX_HEALTH: {
            float stat_hp = vitality * hp_mult;
            return stat_hp * health_percent_modifier + base_health_modifier;
        }
        case ATTR_DAMAGE_REDUCTION:   return std::min(75.0f, endurance * reduction_mult);
        case ATTR_EVADE_CHANCE:       return std::min(50.0f, agility * evade_mult);
        case ATTR_CRIT_CHANCE:        return std::min(100.0f, agility * 0.2f);
        case ATTR_PHYSICAL_ATTACK_BONUS: return strength * 2.0f;
    }
    return 0.0f;
}

} // namespace godot
