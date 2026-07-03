#include "item_instance.hpp"

namespace godot {

// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER
// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER
// TEMPORARY METHOD PLUG
float get_mock_base_weight(const StringName& p_template_id) {
    if (p_template_id == StringName("potion_health")) return 0.2f;
    if (p_template_id == StringName("sword_iron")) return 4.0f;
    if (p_template_id == StringName("shield_wood")) return 6.0f;
    return 1.0f; 
}

// TODO: ITEM_TAGS - stackability check plug
bool get_mock_is_stackable(const StringName& p_template_id) {
    if (p_template_id == StringName("sword_iron") || p_template_id == StringName("shield_wood")) {
        return false;
    }
    return true; 
}
// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER
// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER

void ItemInstance::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize", "p_id", "p_count"), &ItemInstance::initialize);
    ClassDB::bind_method(D_METHOD("get_template_id"), &ItemInstance::get_template_id);
    ClassDB::bind_method(D_METHOD("get_count"), &ItemInstance::get_count);
    ClassDB::bind_method(D_METHOD("set_count", "p_count"), &ItemInstance::set_count);
    ClassDB::bind_method(D_METHOD("get_total_weight"), &ItemInstance::get_total_weight);

    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "template_id"), "", "get_template_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");
}

void ItemInstance::initialize(const StringName& p_id, int p_count) {
    template_id = p_id;
    count = p_count < 1 ? 1 : p_count;
}

float ItemInstance::get_total_weight() const {
    return get_mock_base_weight(template_id) * static_cast<float>(count);
}

} // namespace godot
