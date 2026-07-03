#pragma once

#include "actor.hpp"
#include "base_component.hpp"
#include "stats_component.hpp"

namespace godot 
{
    class HealthComponent : public BaseComponent {

    GDCLASS(HealthComponent, BaseComponent);

    protected:
        void _on_actor_ready(Actor* p_owner) override;
        static void _bind_methods();

    public:
        HealthComponent();
        ~HealthComponent();

        void update_max_health();
        void _on_stats_changed();

        void set_health(float p_health); 
        float get_health() const;

        void set_max_health(float p_max_health);
        float get_max_health() const;

        bool is_alive() const;

        enum DamageResult {
            DAMAGE_INVALID,
            DAMAGE_APPLIED,
            DAMAGE_EVADED,
            DAMAGE_BLOCKED
        };

        // void set_regeneration(float p_amount, float p_interval);
        
        // void set_regen_amount(float p_amount);
        // float get_regen_amount() const;

        // void set_regen_interval(float p_interval);
        // float get_regen_interval() const;

        Dictionary take_damage(float p_base_damage);

    private:
        float base_health = 100;
        float max_health = 100;
        float current_health = 100;

        // float regen_amount;
        // float regen_interval;
        // double regen_timer;

        StatsComponent* stats_component = nullptr; 
    };
} // namespace godot

VARIANT_ENUM_CAST(godot::HealthComponent::DamageResult);
