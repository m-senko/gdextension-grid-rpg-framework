#include "grid_movement_component.hpp"
#include "game_grid_map.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void GridMovementComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_grid_position", "p_pos"), &GridMovementComponent::set_grid_position);
    ClassDB::bind_method(D_METHOD("get_grid_position"), &GridMovementComponent::get_grid_position);
    
    ClassDB::bind_method(D_METHOD("set_cell_size", "p_size"), &GridMovementComponent::set_cell_size);
    ClassDB::bind_method(D_METHOD("get_cell_size"), &GridMovementComponent::get_cell_size);

    ClassDB::bind_method(D_METHOD("try_move", "p_direction"), &GridMovementComponent::try_move);

    ClassDB::add_property("GridMovementComponent", PropertyInfo(Variant::VECTOR2I, "grid_position"), "set_grid_position", "get_grid_position");
    ClassDB::add_property("GridMovementComponent", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
    ADD_SIGNAL(MethodInfo("moved", PropertyInfo(Variant::VECTOR2I, "next_pos"), PropertyInfo(Variant::VECTOR2I, "prev_pos")));
}


GridMovementComponent::GridMovementComponent() 
:   grid_position(0, 0),
    cell_size(32),
    map_node(nullptr)
{}

GridMovementComponent::~GridMovementComponent() {
    if (map_node != nullptr) {
        map_node->clear_occupant(grid_position);
    }
}

void GridMovementComponent::_ready() {
    if(!map_init()){ return; }

    Vector2 pixel_pos = get_global_position();

    grid_position.x = static_cast<int>(std::floor(pixel_pos.x / cell_size));
    grid_position.y = static_cast<int>(std::floor(pixel_pos.y / cell_size));

    UtilityFunctions::print("Entity initialized at C++ Grid Position: ", grid_position);
    set_grid_position(grid_position);
    
    map_node->set_occupant(grid_position, get_owner()); 
}

bool GridMovementComponent::map_init()
{
    map_node = GameGridMap::get_singleton();
    if (map_node == nullptr) {
        ERR_PRINT("GameGridMap singleton is missing! Grid movement will not function.");
        return false;
    }
    return true;
}

void GridMovementComponent::set_grid_position(Vector2i p_pos) {
    grid_position = p_pos;
}

Vector2i GridMovementComponent::get_grid_position() const { return grid_position; }

void GridMovementComponent::set_cell_size(int p_size) { cell_size = std::max(1, p_size);}
int GridMovementComponent::get_cell_size() const { return cell_size; }


bool GridMovementComponent::try_move(Vector2i p_direction) {
    if (p_direction == Vector2i(0, 0)) { return false; }

    Vector2i target_cell = grid_position + p_direction;

    GameGridMap* map = GameGridMap::get_singleton();
    
    if (map != nullptr && map->is_tile_walkable(target_cell)) {
        map->clear_occupant(grid_position);
        map->set_occupant(target_cell, get_owner());

        emit_signal("moved", target_cell, grid_position);

        grid_position = target_cell;
        map->on_cell_entered(get_owner(), grid_position);

        return true; 
    }

    return false;
}

} // namespace godot
