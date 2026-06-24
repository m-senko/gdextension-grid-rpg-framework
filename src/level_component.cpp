#include "level_component.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void LevelComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "p_progression"), &LevelComponent::init);
    ClassDB::bind_method(D_METHOD("add_xp", "p_value"), &LevelComponent::add_xp);
    ClassDB::bind_method(D_METHOD("get_xp"), &LevelComponent::get_xp);
    ClassDB::bind_method(D_METHOD("add_level", "p_level"), &LevelComponent::add_level);
    ClassDB::bind_method(D_METHOD("get_level"), &LevelComponent::get_level);

    ADD_SIGNAL(MethodInfo("level_up", PropertyInfo(Variant::INT, "new_level"), PropertyInfo(Variant::INT, "levels_added")));
}

LevelComponent::LevelComponent() 
:   total_xp(0),
    current_level(1),
    level_progression(1.1)
{
    UtilityFunctions::print("LevelComponent created");
}

LevelComponent::~LevelComponent() {
    UtilityFunctions::print("LevelComponent destroyed");
}

void LevelComponent::init(float p_progression)
{
    level_progression = p_progression;
}

void LevelComponent::add_xp(float p_value) {
    total_xp += p_value;
    float xp_needed_for_next = 100 * std::pow(level_progression, current_level-1);

    if(total_xp < xp_needed_for_next){ return; }

    int levels_to_add = 0;

    while (total_xp >= xp_needed_for_next) {
        total_xp -= xp_needed_for_next;
        levels_to_add++;
        current_level++;
        xp_needed_for_next = 100 * std::pow(level_progression, current_level-1);
    }
    emit_signal("level_up", current_level, levels_to_add);
}

float LevelComponent::get_xp() const {
    return total_xp;
}

void LevelComponent::add_level(int p_level)
{
    current_level += p_level;
    total_xp = 0;
    emit_signal("level_up", current_level, p_level);
}

int LevelComponent::get_level() const
{
    return current_level;
}

} // namespace godot
