#include "movement_module.hpp"
#include "grid_map_singleton.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

namespace godot {

void MovementAIModule::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_movement_cooldown", "p_turns"), &MovementAIModule::set_movement_cooldown);
    ClassDB::bind_method(D_METHOD("get_movement_cooldown"), &MovementAIModule::get_movement_cooldown);

    ClassDB::add_property("MovementAIModule", PropertyInfo(Variant::INT, "movement_cooldown_turns"), "set_movement_cooldown", "get_movement_cooldown");
}

MovementAIModule::MovementAIModule() {
    astar.instantiate();
}

MovementAIModule::~MovementAIModule() {}

void MovementAIModule::set_movement_cooldown(int p_turns) {
    movement_cooldown_turns = std::max(0, p_turns);
}

int MovementAIModule::get_movement_cooldown() const {
    return movement_cooldown_turns;
}

void MovementAIModule::initialize_navigation(Vector2i p_map_size) {
    astar->set_region(Rect2i(0, 0, p_map_size.x, p_map_size.y));
    astar->set_cell_size(Vector2(1, 1));
    astar->set_diagonal_mode(AStarGrid2D::DIAGONAL_MODE_NEVER);
    astar->update();
}

bool MovementAIModule::update_cooldown_and_check() {
    if (movement_cooldown_turns == 0) {
        return true; 
    }

    if (current_cooldown_ticks < movement_cooldown_turns) {
        current_cooldown_ticks++;
        return false;
    }

    current_cooldown_ticks = 0;
    return true;
}

void MovementAIModule::update_navigation_grid(GridMapSingleton* p_grid_map, Vector2i p_my_cell, Vector2i p_player_cell) {
    if (p_grid_map == nullptr) return;

    Vector2i map_size = p_grid_map->get_map_size();
    Vector2i map_offset = p_grid_map->get_map_offset();

    for (int x = 0; x < map_size.x; ++x) {
        for (int y = 0; y < map_size.y; ++y) {
            Vector2i global_cell(map_offset.x + x, map_offset.y + y);
            
            bool is_solid = !p_grid_map->is_tile_walkable(global_cell) && 
                             (global_cell != p_player_cell) && 
                             (global_cell != p_my_cell);
            
            astar->set_point_solid(Vector2i(x, y), is_solid);
        }
    }
    astar->update();
}

Vector2i MovementAIModule::calculate_next_step(Vector2i p_my_local_cell, Vector2i p_player_local_cell) {
    TypedArray<Vector2i> path = astar->get_id_path(p_my_local_cell, p_player_local_cell);
    
    if (path.size() > 1) {
        Vector2i next_local_cell = path[1];
        return next_local_cell - p_my_local_cell;
    }
    return Vector2i(0, 0);
}

} // namespace godot
