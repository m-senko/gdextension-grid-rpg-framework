#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/object_id.hpp>
#include <unordered_set>

namespace godot {

class GameManagerSingleton : public Node {
    GDCLASS(GameManagerSingleton, Node);

private:
    static GameManagerSingleton* singleton;

    std::unordered_set<uint64_t> active_enemies;

protected:
    static void _bind_methods();

public:
    GameManagerSingleton();
    ~GameManagerSingleton();

    void _notification(int p_what);

    static GameManagerSingleton* get_singleton();

    void register_enemy(Node* p_enemy);
    void unregister_enemy(Node* p_enemy);

    void trigger_enemy_turns();
};

} // namespace godot
