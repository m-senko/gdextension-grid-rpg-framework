#include "game_manager_singleton.hpp"
#include "actor.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

GameManagerSingleton* GameManagerSingleton::singleton = nullptr;

void GameManagerSingleton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("register_enemy", "p_enemy"), &GameManagerSingleton::register_enemy);
    ClassDB::bind_method(D_METHOD("unregister_enemy", "p_enemy"), &GameManagerSingleton::unregister_enemy);
    ClassDB::bind_method(D_METHOD("trigger_enemy_turns"), &GameManagerSingleton::trigger_enemy_turns);

    ClassDB::bind_static_method("GameManagerSingleton", D_METHOD("get_singleton"), &GameManagerSingleton::get_singleton);
}

GameManagerSingleton::GameManagerSingleton() {}

GameManagerSingleton::~GameManagerSingleton() {
    if (singleton == this) {
        singleton = nullptr;
    }
}

GameManagerSingleton* GameManagerSingleton::get_singleton() {
    return singleton;
}

void GameManagerSingleton::_notification(int p_what) {
    if (p_what == NOTIFICATION_READY) {
        if (singleton == nullptr) {
            singleton = this;
            UtilityFunctions::print("GameManagerSingleton singleton initialized successfully.");
        }
    }
    Node::_notification(p_what);
}

void GameManagerSingleton::register_enemy(Node* p_enemy) {
    if (p_enemy == nullptr) return;

    uint64_t id = p_enemy->get_instance_id();
    auto result = active_enemies.insert(id);
    
    if (result.second) {
        UtilityFunctions::print("GameManagerSingleton: Enemy registered. Total active: ", active_enemies.size());
    }
}

void GameManagerSingleton::unregister_enemy(Node* p_enemy) {
    if (p_enemy == nullptr) return;

    uint64_t id = p_enemy->get_instance_id();
    size_t removed_count = active_enemies.erase(id);
    
    if (removed_count > 0) {
        UtilityFunctions::print("GameManagerSingleton: Enemy unregistered. Total active: ", active_enemies.size());
    }
}

void GameManagerSingleton::trigger_enemy_turns() {
    if (active_enemies.empty()) return;

    std::vector<uint64_t> dead_ids;

    for (uint64_t id : active_enemies) {
        Object* obj = ObjectDB::get_instance(ObjectID(id));
        
        if (obj == nullptr) {
            dead_ids.push_back(id);
            continue;
        }

        Actor* enemy_actor = Object::cast_to<Actor>(obj);
        if (enemy_actor == nullptr) continue;

        // ==========================================================================
        // FUTURE PLUG FOR ENEMY AI
        // ==========================================================================================
        // TODO: When an AI component (e.g., EnemyAIComponent) appears:
        // 1. Get it through the cache: my_ai = enemy_actor->get_component<EnemyAIComponent>();
        // 2. Call the decision-making method: if (my_ai) { my_ai->execute_turn(); }
        //
        // For now, we'll just output a debug print to the console to see that the manager is running.
        UtilityFunctions::print("GameManagerSingleton: Enemy AI tick triggered for Actor ID: ", id);
        // =====================================================================

        // TODO (AI Integration Phase):
        // 1. Create a dedicated EnemyAIComponent inherited from BaseComponent.
        // 2. Replace this print statement with a cache lookup for EnemyAIComponent.
        // 3. Delegate movement/attack logic entirely to the AI component's decision-making methods.
    }

    for (uint64_t dead_id : dead_ids) {
        active_enemies.erase(dead_id);
    }
}

} // namespace godot
