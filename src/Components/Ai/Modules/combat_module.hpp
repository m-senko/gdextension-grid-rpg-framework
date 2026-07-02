#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector2i.hpp>

namespace godot {

class CombatComponent; 

class CombatAIModule : public Node {
    GDCLASS(CombatAIModule, Node);

private:
    float base_attack_range = 1.0f;

protected:
    static void _bind_methods();

public:
    CombatAIModule();
    ~CombatAIModule();

    bool execute_combat_action(CombatComponent* p_combat_comp, Vector2i p_player_global_cell);

    void set_attack_range(float p_range);
    float get_attack_range() const;
};

} // namespace godot
