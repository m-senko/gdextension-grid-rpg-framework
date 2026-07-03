#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "item_template.hpp"

namespace godot{

class ItemInstance : public RefCounted {
    GDCLASS(ItemInstance, RefCounted);

private:
    Ref<ItemTemplate> item_template;
    int count = 1;

protected:
    static void _bind_methods();

public:
    ItemInstance() = default;
    ~ItemInstance() = default;

    void initialize(const Ref<ItemTemplate>& p_template, int p_count);
    Ref<ItemTemplate> get_template() const { return item_template; }
    StringName get_template_id() const { return item_template.is_valid() ? item_template->get_id() : StringName(); }

    void set_count(int p_count) { count = p_count; }
    int get_count() const { return count; }

    float get_total_weight() const; 
};

} // namespace godot
