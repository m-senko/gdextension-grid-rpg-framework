#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <vector>

namespace godot {

// Легковесная структура состояния одной ячейки сетки
struct CellState {
    bool is_wall = false;          
    Node* occupant = nullptr;     
    Node* ground_effect = nullptr; 
    Node* dropped_item = nullptr;
};

class GameGridMap : public Node {
    GDCLASS(GameGridMap, Node);

private:
    std::vector<std::vector<CellState>> matrix;
    Vector2i map_size;
    Vector2i map_offset;
    TileMapLayer* visual_layer;

    static GameGridMap* singleton;

protected:
    static void _bind_methods();

public:
    GameGridMap();
    ~GameGridMap();

    void _ready() override;

    static GameGridMap* get_singleton();

    void initialize_map();

    bool is_tile_walkable(Vector2i p_cell) const;

    bool set_occupant(Vector2i p_cell, Node* p_node);
    Node* get_occupant(Vector2i p_cell) const;
    void clear_occupant(Vector2i p_cell);

    void on_cell_entered(Node* p_entity, Vector2i p_cell);
};

} // namespace godot
