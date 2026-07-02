#pragma once

#include "base_component.hpp"
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class GridMovementComponent;
class CombatComponent;

class StateAIModule;
class MovementAIModule;
class CombatAIModule;

class AIComponent : public BaseComponent {
    GDCLASS(AIComponent, BaseComponent);

private:
    GridMovementComponent* movement_comp = nullptr;
    CombatComponent* combat_comp = nullptr;

    StateAIModule* state_module = nullptr;
    MovementAIModule* movement_module = nullptr;
    CombatAIModule* combat_module = nullptr;

protected:
    static void _bind_methods();

public:
    AIComponent();
    ~AIComponent();

    void _on_actor_ready(Actor* p_owner) override;

    void execute_turn();
};

} // namespace godot
