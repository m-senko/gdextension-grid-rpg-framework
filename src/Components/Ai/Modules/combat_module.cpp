#include "combat_module.hpp"
#include "combat_component.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

namespace godot {

void CombatAIModule::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_attack_range", "p_range"), &CombatAIModule::set_attack_range);
    ClassDB::bind_method(D_METHOD("get_attack_range"), &CombatAIModule::get_attack_range);

    ClassDB::add_property("CombatAIModule", PropertyInfo(Variant::FLOAT, "base_attack_range"), "set_attack_range", "get_attack_range");
}

CombatAIModule::CombatAIModule() {}
CombatAIModule::~CombatAIModule() {}

void CombatAIModule::set_attack_range(float p_range) {
    base_attack_range = std::max(1.0f, p_range);
}

float CombatAIModule::get_attack_range() const {
    return base_attack_range;
}

bool CombatAIModule::execute_combat_action(CombatComponent* p_combat_comp, Vector2i p_player_global_cell) {
    if (p_combat_comp == nullptr) {
        return false;
    }

    UtilityFunctions::print("CombatAIModule: Initiating grid attack at cell: ", p_player_global_cell);
    
    p_combat_comp->attack(p_player_global_cell);
    
    return true; 
}

} // namespace godot
