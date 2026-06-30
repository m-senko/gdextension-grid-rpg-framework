#include "grid_map_singleton.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/tile_data.hpp>

namespace godot {

// =============================================================================
// Godot Binding
// =============================================================================

void GridMapSingleton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_tile_walkable", "p_cell"), &GridMapSingleton::is_tile_walkable);
    ClassDB::bind_method(D_METHOD("on_cell_entered", "p_cell", "p_node"), &GridMapSingleton::on_cell_entered);
    
    ClassDB::bind_method(D_METHOD("get_occupant", "p_cell"), &GridMapSingleton::get_occupant);
    ClassDB::bind_method(D_METHOD("get_effect", "p_cell"), &GridMapSingleton::get_effect);

    ClassDB::bind_method(D_METHOD("set_occupant", "p_cell", "p_node"), &GridMapSingleton::set_occupant);
    ClassDB::bind_method(D_METHOD("clear_occupant", "p_cell"), &GridMapSingleton::clear_occupant);
}

// =============================================================================
// Lifecycle & Initialization
// =============================================================================

GridMapSingleton* GridMapSingleton::singleton = nullptr;

GridMapSingleton* GridMapSingleton::get_singleton() {
    return singleton;
}

GridMapSingleton::GridMapSingleton() : visual_layer(nullptr), map_size(0, 0) {}

GridMapSingleton::~GridMapSingleton() {
    if (singleton == this) {
        singleton = nullptr;
    }
}

void GridMapSingleton::_ready() {
    if (singleton == nullptr) {
        singleton = this;
    }
    
    visual_layer = get_node<TileMapLayer>("../TileMapLayer");
    
    if (visual_layer != nullptr) {
        initialize_map();
    } else {
        UtilityFunctions::print("Warning: GridMapSingleton could not find TileMapLayer node!");
    }
}

void GridMapSingleton::initialize_map() {
    Rect2i used_rect = visual_layer->get_used_rect();
    map_size.x = used_rect.size.x;
    map_size.y = used_rect.size.y;
    
    map_offset = used_rect.position; 

    matrix.resize(map_size.x, std::vector<CellState>(map_size.y));

    for (int x = 0; x < map_size.x; ++x) {
        for (int y = 0; y < map_size.y; ++y) {
            Vector2i cell_coords(map_offset.x + x, map_offset.y + y);
            
            TileData* tile_data = visual_layer->get_cell_tile_data(cell_coords);
            if (tile_data != nullptr) {
                bool walkable = tile_data->get_custom_data("walkable");
                matrix[x][y].is_wall = !walkable;
            } else {
                matrix[x][y].is_wall = true;
            }
        }
    }
    UtilityFunctions::print("GridMapSingleton initialized. Size: ", map_size, " | Offset: ", map_offset);
}

// =============================================================================
// Coordinate System Helpers
// =============================================================================

Vector2i GridMapSingleton::global_to_grid(Vector2i p_global_cell) const {
    return p_global_cell - map_offset;
}

bool GridMapSingleton::is_index_valid(Vector2i p_local_cell) const {
    return p_local_cell.x >= 0 && p_local_cell.x < map_size.x &&
           p_local_cell.y >= 0 && p_local_cell.y < map_size.y;
}

// =============================================================================
// Grid Logic - Local Coordinates (Internal Business Logic)
// =============================================================================

Node* GridMapSingleton::get_occupant_local(Vector2i p_local_cell) {
    if (!is_index_valid(p_local_cell)) { return nullptr; }
    
    ObjectID id = matrix[p_local_cell.x][p_local_cell.y].occupant_id;
    if (id.is_null()) { return nullptr; }

    Object* obj = ObjectDB::get_instance(id);
    if (obj == nullptr) {
        matrix[p_local_cell.x][p_local_cell.y].occupant_id = ObjectID(); 
        return nullptr;
    }
    return Object::cast_to<Node>(obj);
}

Node* GridMapSingleton::get_effect_local(Vector2i p_local_cell) {
    if (!is_index_valid(p_local_cell)) { return nullptr; }
    
    ObjectID id = matrix[p_local_cell.x][p_local_cell.y].ground_effect_id;
    if (id.is_null()) { return nullptr; }

    Object* obj = ObjectDB::get_instance(id);
    if (obj == nullptr) {
        matrix[p_local_cell.x][p_local_cell.y].ground_effect_id = ObjectID(); 
        return nullptr;
    }
    return Object::cast_to<Node>(obj);
}

bool GridMapSingleton::is_tile_walkable_local(Vector2i p_local_cell) {
    if (!is_index_valid(p_local_cell)) { return false; }
    if (matrix[p_local_cell.x][p_local_cell.y].is_wall) { return false; }
    
    if (get_occupant_local(p_local_cell) != nullptr) { return false; } 

    return true; 
}

// =============================================================================
// Grid Logic - Global Coordinates (API Overtures for GDScript / External Nodes)
// =============================================================================

Node* GridMapSingleton::get_occupant(Vector2i p_global_cell) {
    return get_occupant_local(global_to_grid(p_global_cell));
}

Node* GridMapSingleton::get_effect(Vector2i p_global_cell) {
    return get_effect_local(global_to_grid(p_global_cell));
}

bool GridMapSingleton::is_tile_walkable(Vector2i p_global_cell) {
    return is_tile_walkable_local(global_to_grid(p_global_cell));
}

bool GridMapSingleton::set_occupant(Vector2i p_global_cell, Node* p_node) {
    if (p_node == nullptr) { return false; }

    Node* current_occupant = get_occupant(p_global_cell);
    if (current_occupant != nullptr && current_occupant != p_node) {
        return false; 
    }

    Vector2i local_cell = global_to_grid(p_global_cell);
    if (!is_index_valid(local_cell)) { return false; }

    matrix[local_cell.x][local_cell.y].occupant_id = p_node->get_instance_id();
    return true;
}

void GridMapSingleton::clear_occupant(Vector2i p_global_cell) {
    Vector2i local_cell = global_to_grid(p_global_cell);
    if (!is_index_valid(local_cell)) { return; }

    matrix[local_cell.x][local_cell.y].occupant_id = ObjectID();
}

void GridMapSingleton::on_cell_entered(Vector2i p_global_cell, Node* p_entity) {
    Node* ground_effect = get_effect(p_global_cell); 
    if (ground_effect != nullptr) {
        ground_effect->call("apply_effect", p_entity);
    }
}

} // namespace godot
