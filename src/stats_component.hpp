#pragma once

#include "base_component.hpp"
#include "stats_balance_config.hpp"

namespace godot {

class StatsComponent : public BaseComponent {
    GDCLASS(StatsComponent, BaseComponent);

public:
    enum Attribute {
        ATTR_MAX_HEALTH,
        ATTR_DAMAGE_REDUCTION,
        ATTR_EVADE_CHANCE,
        ATTR_CRIT_CHANCE,
        ATTR_PHYSICAL_ATTACK_BONUS
    };

private:
    int strength = 10;
    int agility = 10;
    int vitality = 10;
    int endurance = 10;

    float base_health_modifier = 0.0f;
    float health_percent_modifier = 1.0f;

    Ref<StatsBalanceConfig> balance_config;

protected:
    static void _bind_methods();

public:
    StatsComponent();
    ~StatsComponent();

    void set_balance_config(const Ref<StatsBalanceConfig>& p_config);
    Ref<StatsBalanceConfig> get_balance_config() const;

    void add_health_bonus(float p_flat, float p_percent);

    float get_stat_hp() const;
    float get_health_percent_modifier() const;
    float get_base_health_modifier() const;

    void set_strength(int p_value);
    int get_strength() const;
    void set_agility(int p_value);
    int get_agility() const;
    void set_vitality(int p_value);
    int get_vitality() const;
    void set_endurance(int p_value);
    int get_endurance() const;

    float get_attribute(Attribute p_attribute) const;
};

} // namespace godot
