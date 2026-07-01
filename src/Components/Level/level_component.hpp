#pragma once

#include "base_component.hpp"

namespace godot {

class LevelComponent : public BaseComponent {
    GDCLASS(LevelComponent, BaseComponent);

private:
    float total_xp;
    int current_level;
    float level_progression;

protected:
    static void _bind_methods();

public:
    LevelComponent();
    ~LevelComponent();

    void init(float p_progression);
    
    void add_xp(float p_value);
    float get_xp() const;
    void add_level(int p_level);
    int get_level() const;
};

} // namespace godot
