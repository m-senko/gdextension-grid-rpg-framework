#pragma once

#include <godot_cpp/classes/resource.hpp>

namespace godot {

class StatsBalanceConfig : public Resource {
    GDCLASS(StatsBalanceConfig, Resource);

private:
    float hp_per_vitality = 12.0f;
    float speed_per_agility = 1.5f;
    float reduction_per_endurance = 1.0f;
    float evade_per_agility = 0.5f;

protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("set_hp_per_vitality", "p_value"), &StatsBalanceConfig::set_hp_per_vitality);
        ClassDB::bind_method(D_METHOD("get_hp_per_vitality"), &StatsBalanceConfig::get_hp_per_vitality);
        ClassDB::bind_method(D_METHOD("set_speed_per_agility", "p_value"), &StatsBalanceConfig::set_speed_per_agility);
        ClassDB::bind_method(D_METHOD("get_speed_per_agility"), &StatsBalanceConfig::get_speed_per_agility);
        ClassDB::bind_method(D_METHOD("set_reduction_per_endurance", "p_value"), &StatsBalanceConfig::set_reduction_per_endurance);
        ClassDB::bind_method(D_METHOD("get_reduction_per_endurance"), &StatsBalanceConfig::get_reduction_per_endurance);
        ClassDB::bind_method(D_METHOD("set_evade_per_agility", "p_value"), &StatsBalanceConfig::set_evade_per_agility);
        ClassDB::bind_method(D_METHOD("get_evade_per_agility"), &StatsBalanceConfig::get_evade_per_agility);

        ClassDB::add_property("StatsBalanceConfig", PropertyInfo(Variant::FLOAT, "hp_per_vitality"), "set_hp_per_vitality", "get_hp_per_vitality");
        ClassDB::add_property("StatsBalanceConfig", PropertyInfo(Variant::FLOAT, "speed_per_agility"), "set_speed_per_agility", "get_speed_per_agility");
        ClassDB::add_property("StatsBalanceConfig", PropertyInfo(Variant::FLOAT, "reduction_per_endurance"), "set_reduction_per_endurance", "get_reduction_per_endurance");
        ClassDB::add_property("StatsBalanceConfig", PropertyInfo(Variant::FLOAT, "evade_per_agility"), "set_evade_per_agility", "get_evade_per_agility");
    }

public:
    StatsBalanceConfig() {}
    ~StatsBalanceConfig() {}

    void set_hp_per_vitality(float p_val) { hp_per_vitality = p_val; }
    float get_hp_per_vitality() const { return hp_per_vitality; }

    void set_speed_per_agility(float p_val) { speed_per_agility = p_val; }
    float get_speed_per_agility() const { return speed_per_agility; }

    void set_reduction_per_endurance(float p_val) { reduction_per_endurance = p_val; }
    float get_reduction_per_endurance() const { return reduction_per_endurance; }

    void set_evade_per_agility(float p_val) { evade_per_agility = p_val; }
    float get_evade_per_agility() const { return evade_per_agility; }
};

} // namespace godot
