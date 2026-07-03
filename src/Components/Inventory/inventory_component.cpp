#include "inventory_component.hpp"
#include "actor.hpp"
#include <godot_cpp/core/object.hpp>

namespace godot {

// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER
// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER
bool get_mock_is_stackable(const StringName& p_template_id);
// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER
// --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER --- DELETE LATER

void InventoryComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_item", "p_template_id", "p_count"), &InventoryComponent::add_item);
    ClassDB::bind_method(D_METHOD("remove_item", "p_index", "p_count"), &InventoryComponent::remove_item);
    ClassDB::bind_method(D_METHOD("transfer_to", "p_index", "p_target_inventory_node", "p_count"), &InventoryComponent::transfer_to);
    
    ClassDB::bind_method(D_METHOD("get_items"), &InventoryComponent::get_items);
    ClassDB::bind_method(D_METHOD("get_current_weight"), &InventoryComponent::get_current_weight);
    ClassDB::bind_method(D_METHOD("get_max_weight"), &InventoryComponent::get_max_weight);
    ClassDB::bind_method(D_METHOD("set_base_max_weight", "p_weight"), &InventoryComponent::set_base_max_weight);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_max_weight"), "set_base_max_weight", "get_max_weight");

    ADD_SIGNAL(MethodInfo("inventory_changed"));
    ADD_SIGNAL(MethodInfo("weight_changed", PropertyInfo(Variant::FLOAT, "new_weight"), PropertyInfo(Variant::FLOAT, "max_weight")));
}

void InventoryComponent::_on_actor_ready(Actor* p_owner) {

    // // TODO: STATS_INTEGRATION - Find StatsComponent via p_owner->get_component_gd()
    
    recalculate_weight();
}

void InventoryComponent::recalculate_weight() {
    float new_weight = 0.0f;
    
    for (int i = 0; i < inventory_array.size(); ++i) {
        Ref<ItemInstance> item = inventory_array[i];
        if (item.is_valid()) {
            new_weight += item->get_total_weight();
        }
    }
    
    current_weight = new_weight;
    
    // // TODO: STATS_INTEGRATION - base_max_weight should be dynamically modified by stats
    emit_signal("weight_changed", current_weight, base_max_weight);
}

bool InventoryComponent::add_item(const StringName& p_template_id, int p_count) {
    if (p_count <= 0) return false;

    bool is_stackable = get_mock_is_stackable(p_template_id);
    bool item_merged = false;

    if (is_stackable) {
        for (int i = 0; i < inventory_array.size(); ++i) {
            Ref<ItemInstance> existing_item = inventory_array[i];
            if (existing_item.is_valid() && existing_item->get_template_id() == p_template_id) {
                existing_item->set_count(existing_item->get_count() + p_count);
                item_merged = true;
                break;
            }
        }
    }

    if (!item_merged) {
        Ref<ItemInstance> new_item;
        new_item.instantiate();
        new_item->initialize(p_template_id, p_count);
        inventory_array.append(new_item);
    }

    recalculate_weight();
    emit_signal("inventory_changed");
    return true;
}

bool InventoryComponent::remove_item(int p_index, int p_count) {
    if (p_index < 0 || p_index >= inventory_array.size() || p_count <= 0) {
        return false;
    }

    Ref<ItemInstance> item = inventory_array[p_index];
    if (!item.is_valid()) return false;

    int current_count = item->get_count();
    
    if (p_count > current_count) {
        return false; 
    }

    if (p_count == current_count) {
        inventory_array.remove_at(p_index);
    } else {
        item->set_count(current_count - p_count);
    }

    recalculate_weight();
    emit_signal("inventory_changed");
    return true;
}

bool InventoryComponent::transfer_to(int p_index, Node* p_target_inventory_node, int p_count) {
    if (p_target_inventory_node == nullptr) return false;

    InventoryComponent* target_inventory = Object::cast_to<InventoryComponent>(p_target_inventory_node);
    if (target_inventory == nullptr) {
        return false;
    }

    if (p_index < 0 || p_index >= inventory_array.size() || p_count <= 0) {
        return false;
    }

    Ref<ItemInstance> item = inventory_array[p_index];
    if (!item.is_valid() || item->get_count() < p_count) {
        return false; 
    }

    StringName item_id = item->get_template_id();

    if (target_inventory->add_item(item_id, p_count)) {
        this->remove_item(p_index, p_count);
        return true;
    }

    return false;
}

void InventoryComponent::set_base_max_weight(float p_weight) {
    base_max_weight = p_weight;
    recalculate_weight();
}

} // namespace godot
