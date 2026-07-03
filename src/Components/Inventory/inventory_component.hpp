#pragma once

#include "base_component.hpp" 
#include "item_instance.hpp"
#include <godot_cpp/variant/typed_array.hpp>

namespace godot{

class InventoryComponent : public BaseComponent {
    GDCLASS(InventoryComponent, BaseComponent);

private:
    TypedArray<ItemInstance> inventory_array;
    float base_max_weight = 50.0f;
    float current_weight = 0.0f;

    void recalculate_weight();

protected:
    static void _bind_methods();

public:
    InventoryComponent() = default;
    ~InventoryComponent() = default;

    virtual void _on_actor_ready(Actor* p_owner) override;

    bool add_item(const Ref<ItemTemplate>& p_template, int p_count);
    bool remove_item(int p_index, int p_count);
    bool transfer_to(int p_index, Node* p_target_inventory_node, int p_count);

    TypedArray<ItemInstance> get_items() const { return inventory_array; }
    float get_current_weight() const { return current_weight; }
    float get_max_weight() const { return base_max_weight; }
    void set_base_max_weight(float p_weight);
};

} // namespace godot
