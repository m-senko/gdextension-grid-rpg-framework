#pragma once

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class GridMovementComponent : public Node2D {
    GDCLASS(GridMovementComponent, Node2D);

private:
    Vector2i grid_position;
    int cell_size;

    Node* map_node; 

protected:
    static void _bind_methods();

public:
    GridMovementComponent();
    ~GridMovementComponent();

    void set_grid_position(Vector2i p_pos);
    Vector2i get_grid_position() const;

    void set_cell_size(int p_size);
    int get_cell_size() const;

    void set_map_node(Node* p_node);
    Node* get_map_node() const;

    bool try_move(Vector2i p_direction);
    bool is_cell_walkable(Vector2i p_target_cell) const;
    
    void update_pixel_position();
};

} // namespace godot
