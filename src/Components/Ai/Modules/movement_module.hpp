#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/a_star_grid2d.hpp>
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class GridMapSingleton; // Опережающее объявление

class MovementAIModule : public Node {
    GDCLASS(MovementAIModule, Node);

private:
    int movement_cooldown_turns = 0;

    int current_cooldown_ticks = 0;

    Ref<AStarGrid2D> astar;

protected:
    static void _bind_methods();

public:
    MovementAIModule();
    ~MovementAIModule();

    void initialize_navigation(Vector2i p_map_size);

    bool update_cooldown_and_check();

    void update_navigation_grid(GridMapSingleton* p_grid_map, Vector2i p_my_cell, Vector2i p_player_cell);

    Vector2i calculate_next_step(Vector2i p_my_local_cell, Vector2i p_player_local_cell);

    void set_movement_cooldown(int p_turns);
    int get_movement_cooldown() const;
};

} // namespace godot
