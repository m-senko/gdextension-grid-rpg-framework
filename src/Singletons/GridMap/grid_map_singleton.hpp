#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <vector>

namespace godot {

// =============================================================================
// Grid Structures
// =============================================================================

struct CellState {
    bool is_wall = false;          
    ObjectID occupant_id;
    ObjectID ground_effect_id;
    ObjectID dropped_item_id;
};

class GridMapSingleton : public Node {
    GDCLASS(GridMapSingleton, Node);

// =============================================================================
// Internal Data Members & Helpers
// =============================================================================
private:
    std::vector<std::vector<CellState>> matrix;
    Vector2i map_size;
    Vector2i map_offset;
    float cell_size = 32;
    TileMapLayer* visual_layer;

    static GridMapSingleton* singleton;

    bool is_index_valid(Vector2i p_local_cell) const;

// =============================================================================
// Godot Binding
// =============================================================================
protected:
    static void _bind_methods();
    void _notification(int p_what); 

// =============================================================================
// Lifecycle & Initialization
// =============================================================================
public:
    GridMapSingleton();
    ~GridMapSingleton();

    static GridMapSingleton* get_singleton();

    void initialize_map();

// =============================================================================
// Grid Logic - Global Coordinates (API Overtures for GDScript / External Nodes)
// =============================================================================  
    Vector2i global_to_grid(Vector2i p_global_cell) const;

    Vector2i get_map_size() const { return map_size; }
    Vector2i get_map_offset() const { return map_offset; }

    void set_cell_size(float p_size);
    float get_cell_size() const;

    Vector2i pixel_to_grid(Vector2 p_pixel_pos) const;
    Vector2 grid_to_pixel(Vector2i p_grid_pos) const;

    Node* get_occupant(Vector2i p_global_cell);
    Node* get_effect(Vector2i p_global_cell);
    bool is_tile_walkable(Vector2i p_global_cell);
    
    bool set_occupant(Vector2i p_global_cell, Node* p_node);
    void clear_occupant(Vector2i p_global_cell);
    void on_cell_entered(Vector2i p_global_cell, Node* p_node);

// =============================================================================
// Grid Logic - Local Coordinates (Internal Business Logic)
// =============================================================================
    Node* get_occupant_local(Vector2i p_local_cell);
    Node* get_effect_local(Vector2i p_local_cell);
    bool is_tile_walkable_local(Vector2i p_local_cell);
};

} // namespace godot
