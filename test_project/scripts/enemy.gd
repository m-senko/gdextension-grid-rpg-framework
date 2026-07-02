extends Actor

@onready var movement_component: Node = get_component("GridMovementComponent")
@onready var health_component: Node = get_component("HealthComponent")

func _ready() -> void:
	if not movement_component:
		push_error("GridMovementComponent не найден на персонаже!")
		return
	movement_component.moved.connect(_on_grid_move)
	health_component.died.connect(_on_died)
	
	var start_grid_pos: Vector2i = movement_component.get_grid_position()
	global_position = GridMapSingleton.get_singleton().grid_to_pixel(start_grid_pos)
	
func _process(delta: float) -> void:
	pass


func _on_grid_move(next_pos: Vector2i, prev_pos: Vector2i) -> void:
	var target_pixel_pos: Vector2 = GridMapSingleton.get_singleton().grid_to_pixel(next_pos)
	
	var tween := create_tween()
	tween.tween_property(self, "global_position", target_pixel_pos, 0.15)\
		.set_trans(Tween.TRANS_SINE)\
		.set_ease(Tween.EASE_IN_OUT)
		
func _on_died() -> void:
	queue_free()
	
