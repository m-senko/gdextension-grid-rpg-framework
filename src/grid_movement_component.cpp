#include "grid_movement_component.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void GridMovementComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_grid_position", "p_pos"), &GridMovementComponent::set_grid_position);
    ClassDB::bind_method(D_METHOD("get_grid_position"), &GridMovementComponent::get_grid_position);
    
    ClassDB::bind_method(D_METHOD("set_cell_size", "p_size"), &GridMovementComponent::set_cell_size);
    ClassDB::bind_method(D_METHOD("get_cell_size"), &GridMovementComponent::get_cell_size);

    ClassDB::bind_method(D_METHOD("set_map_node", "p_node"), &GridMovementComponent::set_map_node);
    ClassDB::bind_method(D_METHOD("get_map_node"), &GridMovementComponent::get_map_node);

    ClassDB::bind_method(D_METHOD("try_move", "p_direction"), &GridMovementComponent::try_move);
    ClassDB::bind_method(D_METHOD("is_cell_walkable", "p_target_cell"), &GridMovementComponent::is_cell_walkable);

    // Экспортируем в Инспектор Godot
    ClassDB::add_property("GridMovementComponent", PropertyInfo(Variant::VECTOR2I, "grid_position"), "set_grid_position", "get_grid_position");
    ClassDB::add_property("GridMovementComponent", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");

    ClassDB::add_property("GridMovementComponent", PropertyInfo(Variant::OBJECT, "map_node", PROPERTY_HINT_NODE_TYPE, "Node"), "set_map_node", "get_map_node");
}

GridMovementComponent::GridMovementComponent() 
:   grid_position(0, 0),
    cell_size(32),
    map_node(nullptr)
{}

GridMovementComponent::~GridMovementComponent() {}

void GridMovementComponent::set_grid_position(Vector2i p_pos) {
    grid_position = p_pos;
    update_pixel_position();
}
Vector2i GridMovementComponent::get_grid_position() const { return grid_position; }

void GridMovementComponent::set_cell_size(int p_size) { cell_size = std::max(1, p_size); update_pixel_position(); }
int GridMovementComponent::get_cell_size() const { return cell_size; }

void GridMovementComponent::set_map_node(Node* p_node) { map_node = p_node; }
Node* GridMovementComponent::get_map_node() const { return map_node; }

void GridMovementComponent::update_pixel_position() {
    float offset = cell_size / 2.0f;
    Vector2 pixel_pos(
        (grid_position.x * cell_size) + offset,
        (grid_position.y * cell_size) + offset
    );
    
    set_position(pixel_pos);
}

bool GridMovementComponent::is_cell_walkable(Vector2i p_target_cell) const {
    if (map_node == nullptr) {
        return true; 
    }

    Variant result = map_node->call("is_tile_walkable", p_target_cell);
    return result.operator bool();
}

bool GridMovementComponent::try_move(Vector2i p_direction) {
    if (p_direction == Vector2i(0, 0)) { return false; }

    Vector2i target_cell = grid_position + p_direction;

    if (is_cell_walkable(target_cell)) {
        grid_position = target_cell;
        
        update_pixel_position(); 
        return true; 
    }

    return false;
}

} // namespace godot
