extends Button

@onready var icon_rect: TextureRect = $MarginContainer/HBoxContainer/ItemIcon
@onready var name_label: Label = $MarginContainer/HBoxContainer/ItemName
@onready var weight_label: Label = $MarginContainer/HBoxContainer/ItemWeight
@onready var count_label: Label = $MarginContainer/HBoxContainer/ItemCount

signal item_clicked(item_id)

var current_item_id = ""

func _ready() -> void:
	pressed.connect(_on_pressed)

func setup(id: String, item_name: String, icon_texture: Texture2D, initial_weight: float, initial_count: int) -> void:
	current_item_id = id
	name_label.text = item_name
	icon_rect.texture = icon_texture
	
	set_weight(initial_weight)
	set_count(initial_count)

func set_weight(new_weight: float) -> void:
	weight_label.text = "%0.1f" % new_weight

func set_count(new_count: int) -> void:
	count_label.text = "х%d" % new_count
	
func _on_pressed() -> void:
	item_clicked.emit(current_item_id)
