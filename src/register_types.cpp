// Include your classes, that you want to expose to Godot

#include "actor.hpp"
#include "ai_component.hpp"
#include "base_component.hpp"
#include "combat_component.hpp"
#include "game_manager_singleton.hpp"
#include "grid_map_singleton.hpp"
#include "stats_balance_config.hpp"
#include "stats_component.hpp"
#include "grid_movement_component.hpp"
#include "health_component.hpp"
#include "level_component.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<BaseComponent>();
	ClassDB::register_class<HealthComponent>();
    ClassDB::register_class<LevelComponent>();
	ClassDB::register_class<StatsComponent>();
	ClassDB::register_class<GridMovementComponent>();
	ClassDB::register_class<GridMapSingleton>();
	ClassDB::register_class<StatsBalanceConfig>();
	ClassDB::register_class<Actor>();
	ClassDB::register_class<CombatComponent>();
	ClassDB::register_class<GameManagerSingleton>();
	ClassDB::register_class<AIComponent>();
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT cpp_test_plugin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
