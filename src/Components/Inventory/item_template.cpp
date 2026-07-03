#include "item_template.hpp"

namespace godot {

void ItemTemplate::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &ItemTemplate::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &ItemTemplate::get_id);

    ClassDB::bind_method(D_METHOD("set_item_name", "p_name"), &ItemTemplate::set_item_name);
    ClassDB::bind_method(D_METHOD("get_item_name"), &ItemTemplate::get_item_name);

    ClassDB::bind_method(D_METHOD("set_base_weight", "p_weight"), &ItemTemplate::set_base_weight);
    ClassDB::bind_method(D_METHOD("get_base_weight"), &ItemTemplate::get_base_weight);

    ClassDB::bind_method(D_METHOD("set_is_stackable", "p_stackable"), &ItemTemplate::set_is_stackable);
    ClassDB::bind_method(D_METHOD("get_is_stackable"), &ItemTemplate::get_is_stackable);

    ClassDB::bind_method(D_METHOD("set_icon", "p_icon"), &ItemTemplate::set_icon);
    ClassDB::bind_method(D_METHOD("get_icon"), &ItemTemplate::get_icon);

    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "id"), "set_id", "get_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_name"), "set_item_name", "get_item_name");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_weight"), "set_base_weight", "get_base_weight");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_stackable"), "set_is_stackable", "get_is_stackable");
    
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
}

} // namespace godot
