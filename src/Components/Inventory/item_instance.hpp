#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot{

class ItemInstance : public RefCounted {
    GDCLASS(ItemInstance, RefCounted);

private:
    StringName template_id;
    int count = 1;

protected:
    static void _bind_methods();

public:
    ItemInstance() = default;
    ~ItemInstance() = default;

    void initialize(const StringName& p_id, int p_count);

    StringName get_template_id() const { return template_id; }
    void set_count(int p_count) { count = p_count; }
    int get_count() const { return count; }

    float get_total_weight() const; 
};

} // namespace godot
