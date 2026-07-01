#include "actor.hpp"
#include "grid_movement_component.hpp"
#include "grid_map_singleton.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

// TODO (Future Expansion / Movement Backlog):
// 1. Add teleport_to(p_target_cell) method that triggers set_grid_position() after a walkability check,
//    and emits a dedicated "teleported" signal for custom VFX (smoke, flashes) instead of standard walk anims.
// 2. Implement Jump/Leap logic requiring a trajectory scan: allow passing over pits/low entities but block if high walls cross the path.
//    (Requires extending GridMapSingleton with custom tile height data like 'is_high_wall').
// 3. For Dashes/Charges, manage the multi-step cycle inside GDScript using repeated try_move() calls with an 'await tween.finished' 
//    cooldown, before transferring the turn back via GameManager.

namespace godot {

void GridMovementComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_grid_position", "p_pos"), &GridMovementComponent::set_grid_position);
    ClassDB::bind_method(D_METHOD("get_grid_position"), &GridMovementComponent::get_grid_position);

    ClassDB::bind_method(D_METHOD("try_move", "p_direction"), &GridMovementComponent::try_move);

    ClassDB::add_property("GridMovementComponent", PropertyInfo(Variant::VECTOR2I, "grid_position"), "set_grid_position", "get_grid_position");
    ADD_SIGNAL(MethodInfo("moved", PropertyInfo(Variant::VECTOR2I, "next_pos"), PropertyInfo(Variant::VECTOR2I, "prev_pos")));
}

GridMovementComponent::GridMovementComponent() : grid_position(0, 0), owner(nullptr) {}
GridMovementComponent::~GridMovementComponent() {}

void GridMovementComponent::_exit_tree() {
    GridMapSingleton* map_singltone = GridMapSingleton::get_singleton();
    if (map_singltone != nullptr) {
        map_singltone->clear_occupant(grid_position);
    }
}

void GridMovementComponent::_on_actor_ready(Actor* p_owner){
    owner = p_owner;
    GridMapSingleton* map_singltone = GridMapSingleton::get_singleton();
    if (map_singltone == nullptr || owner == nullptr) { return; }

    Vector2 pixel_pos = owner->get_global_position();
    Vector2i starting_cell = map_singltone->pixel_to_grid(pixel_pos);

    UtilityFunctions::print("Entity initialized at C++ Grid Position: ", starting_cell);
    
    grid_position = starting_cell;
    map_singltone->set_occupant(grid_position, owner); 
}

void GridMovementComponent::set_grid_position(Vector2i p_pos) {
    if (grid_position == p_pos) { return; }

    GridMapSingleton* map_singltone = GridMapSingleton::get_singleton();
    if (map_singltone != nullptr) {
        map_singltone->clear_occupant(grid_position);
        map_singltone->set_occupant(p_pos, owner);
    }

    Vector2i prev_pos = grid_position;
    grid_position = p_pos;

    emit_signal("moved", grid_position, prev_pos); 
}

Vector2i GridMovementComponent::get_grid_position() const { return grid_position; }

bool GridMovementComponent::try_move(Vector2i p_direction) {
    if (p_direction == Vector2i(0, 0) || owner == nullptr) { return false; }

    GridMapSingleton* map_singltone = GridMapSingleton::get_singleton();
    if (map_singltone == nullptr) { return false; }

    Vector2i target_cell = grid_position + p_direction;

    if (map_singltone->is_tile_walkable(target_cell)) {
        map_singltone->clear_occupant(grid_position);
        map_singltone->set_occupant(target_cell, owner);

        Vector2i prev_pos = grid_position;
        grid_position = target_cell;

        emit_signal("moved", grid_position, prev_pos);

        map_singltone->on_cell_entered(target_cell, owner);
        
        return true; 
    }

    return false;
}

} // namespace godot
