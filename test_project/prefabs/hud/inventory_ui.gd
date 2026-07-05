extends MarginContainer


const ITEM_ROW_SCENE = preload("res://prefabs/hud/item_row.tscn")

@export var list_container: VBoxContainer
@export var weight_label: Label

var inventory_component: InventoryComponent

func _ready() -> void:
	await get_tree().process_frame
	var hud = owner as CanvasLayer
	if hud and hud.player:
		inventory_component = hud.player.get_component("InventoryComponent")
	hide()

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("toggle_inventory"):
		visible = !visible
		if visible and inventory_component:
			initialize()
			_id_or_index_redraw()

func initialize() -> void:
	inventory_component.inventory_changed.connect(_on_inventory_changed)
	inventory_component.weight_changed.connect(_on_weight_changed)
	
	_on_inventory_changed()
	if weight_label:
		var current_weight = inventory_component.get_current_weight()
		var max_weight = inventory_component.base_max_weight
		weight_label.text = "%0.1f / %0.1f" % [current_weight, max_weight]

func _on_inventory_changed() -> void:
	if not visible: 
		return
	
	_id_or_index_redraw()

func _on_weight_changed(current_weight: float, max_weight: float) -> void:
	if weight_label:
		weight_label.text = "%0.1f / %0.1f" % [current_weight, max_weight]

func _id_or_index_redraw() -> void:
	for child in list_container.get_children():
		child.queue_free()
	
	var items_array: Array = inventory_component.get_items()
	
	for i in range(items_array.size()):
		var item_instance : ItemInstance = items_array[i]
		if not item_instance:
			continue
			
		var item_template : ItemTemplate = item_instance.get_template() 
		
		var row = ITEM_ROW_SCENE.instantiate()
		list_container.add_child(row)
		
		var item_id = item_template.id
		var item_name = item_template.item_name
		var item_icon = item_template.icon
		var item_count = item_instance.count
		var item_weight = item_instance.get_total_weight()
		
		row.setup(item_id, item_name, item_icon, item_weight, item_count)
		
		row.set_meta("inventory_index", i)
		
		
		row.item_clicked.connect(_on_item_row_clicked.bind(row))

func _on_item_row_clicked(item_id: String, row_node: Button) -> void:
	var index_in_backend = row_node.get_meta("inventory_index")
	
	inventory_component.remove_item(index_in_backend, 1)
