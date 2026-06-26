#pragma once

#include "grid_map_singleton.hpp"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class GridMovementComponent : public Node2D {
    GDCLASS(GridMovementComponent, Node2D);

private:
    Vector2i grid_position;
    int cell_size;

    GridMapSingleton* map_singltone; 

protected:
    static void _bind_methods();

public:
    GridMovementComponent();
    ~GridMovementComponent();

    void _ready() override;

    bool map_init();

    void set_grid_position(Vector2i p_pos);
    Vector2i get_grid_position() const;

    void set_cell_size(int p_size);
    int get_cell_size() const;

    bool try_move(Vector2i p_direction);
};

} // namespace godot
