#pragma once

#include "base_component.hpp"
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class Actor;

class GridMovementComponent : public BaseComponent {
    GDCLASS(GridMovementComponent, BaseComponent);

private:
    Vector2i grid_position;
    int cell_size;
    Actor* owner = nullptr;

protected:
    static void _bind_methods();

public:
    GridMovementComponent();
    ~GridMovementComponent();

    void _exit_tree() override;
    void _on_actor_ready(Actor* p_owner) override;

    bool map_init();

    void set_grid_position(Vector2i p_pos);
    Vector2i get_grid_position() const;

    void set_cell_size(int p_size);
    int get_cell_size() const;

    bool try_move(Vector2i p_direction);
};

} // namespace godot
