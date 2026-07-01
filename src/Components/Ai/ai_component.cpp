#include "ai_component.hpp"
#include "actor.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "grid_movement_component.hpp"
#include "combat_component.hpp"
#include "grid_map_singleton.hpp"
#include "game_manager_singleton.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

namespace godot {

void AIComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("execute_turn"), &AIComponent::execute_turn);
}

AIComponent::AIComponent() {
    astar.instantiate(); // Создаем экземпляр AStar
}

AIComponent::~AIComponent() {}

void AIComponent::_on_actor_ready(Actor* p_owner) {
    if (p_owner == nullptr) return;

    movement_comp = p_owner->get_component<GridMovementComponent>();
    combat_comp = p_owner->get_component<CombatComponent>();

    GameManagerSingleton* game_manager = GameManagerSingleton::get_singleton();
    if (game_manager != nullptr) {
        game_manager->register_enemy(p_owner);
    }

    _initialize_astar();
}

void AIComponent::_initialize_astar() {
    GridMapSingleton* grid_map = GridMapSingleton::get_singleton();
    if (grid_map == nullptr) return;

    Vector2i map_size = grid_map->get_map_size();
    astar->set_region(Rect2i(0, 0, map_size.x, map_size.y));
    astar->set_cell_size(Vector2(1, 1));
    astar->set_diagonal_mode(AStarGrid2D::DIAGONAL_MODE_NEVER);
    astar->update();
}

void AIComponent::execute_turn() {
    if (movement_comp == nullptr) return;

    GridMapSingleton* grid_map = GridMapSingleton::get_singleton();
    if (grid_map == nullptr) return;

    SceneTree* tree = get_tree();
    if (tree == nullptr) return;

    Node* main_scene = tree->get_current_scene();
    if (main_scene == nullptr) return;
    
    Node* player_node = main_scene->find_child("Player", true, false);
    if (player_node == nullptr) return;

    Actor* player_actor = Object::cast_to<Actor>(player_node);
    if (player_actor == nullptr) return;

    GridMovementComponent* player_move = player_actor->get_component<GridMovementComponent>();
    if (player_move == nullptr) return;

    Vector2i player_cell = player_move->get_grid_position();
    Vector2i my_cell = movement_comp->get_grid_position();

    Vector2i diff = player_cell - my_cell;
    int distance = std::abs(diff.x) + std::abs(diff.y);

    if (distance == 1) {
        if (combat_comp != nullptr) {
            UtilityFunctions::print("AI: Attacking player at cell: ", player_cell);
            combat_comp->attack(player_cell);
            return;
        }
    }

    Vector2i map_size = grid_map->get_map_size();
    Vector2i map_offset = grid_map->get_map_offset();
    for (int x = 0; x < map_size.x; ++x) {
        for (int y = 0; y < map_size.y; ++y) {
            Vector2i global_cell(map_offset.x + x, map_offset.y + y);
            bool is_solid = !grid_map->is_tile_walkable(global_cell) && (global_cell != player_cell) && (global_cell != my_cell);
            
            astar->set_point_solid(Vector2i(x, y), is_solid);
        }
    }
    astar->update();

    Vector2i local_my_cell = grid_map->global_to_grid(my_cell);
    Vector2i local_player_cell = grid_map->global_to_grid(player_cell);

    TypedArray<Vector2i> path = astar->get_id_path(local_my_cell, local_player_cell);
    
    if (path.size() > 1) {
        Vector2i next_local_cell = path[1];
        
        Vector2i direction = next_local_cell - local_my_cell; 

        UtilityFunctions::print("AI: Moving towards player. Direction: ", direction);
        movement_comp->try_move(direction);
    }
}

} // namespace godot
