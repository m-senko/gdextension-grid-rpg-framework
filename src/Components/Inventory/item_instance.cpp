#include "item_instance.hpp"

namespace godot {

void ItemInstance::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize", "p_template", "p_count"), &ItemInstance::initialize);
    ClassDB::bind_method(D_METHOD("get_template"), &ItemInstance::get_template);
    ClassDB::bind_method(D_METHOD("get_template_id"), &ItemInstance::get_template_id);
    ClassDB::bind_method(D_METHOD("get_count"), &ItemInstance::get_count);
    ClassDB::bind_method(D_METHOD("set_count", "p_count"), &ItemInstance::set_count);
    ClassDB::bind_method(D_METHOD("get_total_weight"), &ItemInstance::get_total_weight);

    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "template_id"), "", "get_template_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");
}

void ItemInstance::initialize(const Ref<ItemTemplate>& p_template, int p_count) {
    item_template = p_template;
    count = p_count < 1 ? 1 : p_count;
}

float ItemInstance::get_total_weight() const {
    if (item_template.is_valid()) {
        return item_template->get_base_weight() * static_cast<float>(count);
    }
    return 0.0f;
}
} // namespace godot
