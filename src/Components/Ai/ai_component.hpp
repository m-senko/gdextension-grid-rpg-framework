#pragma once

#include "base_component.hpp"
#include "godot_cpp/classes/a_star_grid2d.hpp"
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class GridMovementComponent;
class CombatComponent;

class AIComponent : public BaseComponent {
    GDCLASS(AIComponent, BaseComponent);

private:
    GridMovementComponent* movement_comp = nullptr;
    CombatComponent* combat_comp = nullptr;

    Ref<AStarGrid2D> astar;

    void _initialize_astar();

protected:
    static void _bind_methods();

public:
    AIComponent();
    ~AIComponent();

    void _on_actor_ready(Actor* p_owner) override;


    void execute_turn();
};

} // namespace godot
