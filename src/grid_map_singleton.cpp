#include "grid_map_singleton.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/tile_data.hpp>

namespace godot {

void GridMapSingleton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_tile_walkable", "p_cell"), &GridMapSingleton::is_tile_walkable);
    ClassDB::bind_method(D_METHOD("on_cell_entered", "p_entity", "p_cell"), &GridMapSingleton::on_cell_entered);
    
    ClassDB::bind_method(D_METHOD("set_occupant", "p_cell", "p_node"), &GridMapSingleton::set_occupant);
    ClassDB::bind_method(D_METHOD("clear_occupant", "p_cell"), &GridMapSingleton::clear_occupant);
}

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

bool GridMapSingleton::is_tile_walkable(Vector2i p_cell) const {
    Vector2i local_cell = p_cell - map_offset;

    if (local_cell.x < 0 || local_cell.x >= map_size.x || local_cell.y < 0 || local_cell.y >= map_size.y) {
        return false;
    }
    const CellState& cell = matrix[local_cell.x][local_cell.y];

    if (cell.is_wall) { return false; }
    if (cell.occupant != nullptr) { return false; }

    return true; 
}


bool GridMapSingleton::set_occupant(Vector2i p_cell, Node* p_node) {
    p_cell -= map_offset;
    if (p_cell.x < 0 || p_cell.x >= map_size.x || p_cell.y < 0 || p_cell.y >= map_size.y || p_node == nullptr) { return false; }
    
    matrix[p_cell.x][p_cell.y].occupant = p_node;
    return true;
}

void GridMapSingleton::clear_occupant(Vector2i p_cell) {
    p_cell -= map_offset;
    if (p_cell.x < 0 || p_cell.x >= map_size.x || p_cell.y < 0 || p_cell.y >= map_size.y) { return; }
    matrix[p_cell.x][p_cell.y].occupant = nullptr;
}

Node* GridMapSingleton::get_occupant(Vector2i p_cell) const {
    p_cell -= map_offset;
    if (p_cell.x < 0 || p_cell.x >= map_size.x || p_cell.y < 0 || p_cell.y >= map_size.y) { return nullptr; }
    return matrix[p_cell.x][p_cell.y].occupant;
}

void GridMapSingleton::on_cell_entered(Node* p_entity, Vector2i p_cell) {
    p_cell -= map_offset;
    if (p_cell.x < 0 || p_cell.x >= map_size.x || p_cell.y < 0 || p_cell.y >= map_size.y) { return; }

    CellState& cell = matrix[p_cell.x][p_cell.y];

    if (cell.ground_effect != nullptr) {
        cell.ground_effect->call("apply_effect", p_entity);
    }
}

} // namespace godot
