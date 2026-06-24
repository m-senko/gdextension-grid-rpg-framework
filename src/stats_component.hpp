#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class StatsComponent : public Node {
    GDCLASS(StatsComponent, Node);

private:
    int strength;     // Affects Damage
    int agility;      // Affects deviation/initiative
    int vitality;     // Affects max health
    int endurance;    // Affects damage resistance (Armor)

    // Coefficient: how much HP does 1 unit of vitality give
    float hp_per_vitality;

protected:
    static void _bind_methods();

public:
    StatsComponent();
    ~StatsComponent();

    void set_strength(int p_value);
    int get_strength() const;

    void set_agility(int p_value);
    int get_agility() const;

    void set_vitality(int p_value);
    int get_vitality() const;

    void set_endurance(int p_value);
    int get_endurance() const;

    void update_health_component();

    // Mathematics of protection
    bool check_evade() const;                                        // Returns 'true' if the character dodged
    float damage_reduction_calculation(float p_incoming_damage) const; // Returns damage after armor
};

} // namespace godot
