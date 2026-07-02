#include "ai_component.hpp"
#include "actor.hpp"
#include "grid_movement_component.hpp"
#include "combat_component.hpp"
#include "grid_map_singleton.hpp"
#include "game_manager_singleton.hpp"

#include "state_module.hpp"
#include "movement_module.hpp"
#include "combat_module.hpp"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void AIComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("execute_turn"), &AIComponent::execute_turn);
}

AIComponent::AIComponent() {}
AIComponent::~AIComponent() {}

void AIComponent::_on_actor_ready(Actor* p_owner) {
    if (p_owner == nullptr) return;

    movement_comp = p_owner->get_component<GridMovementComponent>();
    combat_comp = p_owner->get_component<CombatComponent>();

    int child_count = get_child_count();
    for (int i = 0; i < child_count; ++i) {
        Node* child = get_child(i);
        if (child == nullptr) continue;

        if (auto s_mod = Object::cast_to<StateAIModule>(child)) {
            state_module = s_mod;
        } 
        else if (auto m_mod = Object::cast_to<MovementAIModule>(child)) {
            movement_module = m_mod;
        } 
        else if (auto c_mod = Object::cast_to<CombatAIModule>(child)) {
            combat_module = c_mod;
        }
    }

    GridMapSingleton* grid_map = GridMapSingleton::get_singleton();
    if (movement_module != nullptr && grid_map != nullptr) {
        movement_module->initialize_navigation(grid_map->get_map_size());
    }

    GameManagerSingleton* game_manager = GameManagerSingleton::get_singleton();
    if (game_manager != nullptr) {
        game_manager->register_enemy(p_owner);
    }
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

    int distance = std::abs(player_cell.x - my_cell.x) + std::abs(player_cell.y - my_cell.y);

    // --- AI MODULE WORK CONVEYOR ---

    // 1 State Module
    int current_state = 1;
    if (state_module != nullptr) {
        current_state = state_module->evaluate_state(distance);
    }

    // 2 Combat Module

    if (current_state == 2) { 
        if (combat_module != nullptr) {
            bool attacked = combat_module->execute_combat_action(combat_comp, player_cell);
            if (attacked) {
                return;
            }
        }
    }

    // 3 Movement Module
    if (current_state == 1) {
        if (movement_module != nullptr) {
            if (movement_module->update_cooldown_and_check()) {
                Vector2i local_my_cell = grid_map->global_to_grid(my_cell);
                Vector2i local_player_cell = grid_map->global_to_grid(player_cell);

                movement_module->update_navigation_grid(grid_map, my_cell, player_cell);
                Vector2i direction = movement_module->calculate_next_step(local_my_cell, local_player_cell);

                if (direction != Vector2i(0, 0)) {
                    movement_comp->try_move(direction);
                }
            } else {
            }
        }
    }

}

} // namespace godot
