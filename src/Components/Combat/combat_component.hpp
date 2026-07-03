#pragma  once

#include "base_component.hpp"
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class StatsComponent;

class CombatComponent : public BaseComponent {
    GDCLASS(CombatComponent, BaseComponent);

private:
    StatsComponent* stats_component = nullptr;

    float base_attack_damage = 10.0f;

protected:
    static void _bind_methods();

public:
    CombatComponent();
    ~CombatComponent();

    void _on_actor_ready(Actor* p_owner) override;

    Dictionary attack(Vector2i p_global_cell);

    void set_base_attack_damage(float p_damage);
    float get_base_attack_damage() const;
};

} // namespace godot
