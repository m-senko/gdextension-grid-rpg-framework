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
    TileMapLayer* visual_layer;

    static GridMapSingleton* singleton;

    Vector2i global_to_grid(Vector2i p_global_cell) const;
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
