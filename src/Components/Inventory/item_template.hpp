#pragma once

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class ItemTemplate : public Resource {
    GDCLASS(ItemTemplate, Resource);

private:
    StringName id;
    String item_name;
    float base_weight = 1.0f;
    bool is_stackable = true;
    Ref<Texture2D> icon;

protected:
    static void _bind_methods();

public:
    ItemTemplate() = default;
    ~ItemTemplate() = default;

    void set_id(const StringName& p_id) { id = p_id; }
    StringName get_id() const { return id; }

    void set_item_name(const String& p_name) { item_name = p_name; }
    String get_item_name() const { return item_name; }

    void set_base_weight(float p_weight) { base_weight = p_weight; }
    float get_base_weight() const { return base_weight; }

    void set_is_stackable(bool p_stackable) { is_stackable = p_stackable; }
    bool get_is_stackable() const { return is_stackable; }

    void set_icon(const Ref<Texture2D>& p_icon) { icon = p_icon; }
    Ref<Texture2D> get_icon() const { return icon; }
};

} // namespace godot
