extends Actor

@onready var movement_component: Node = get_component("GridMovementComponent")
@onready var health_component: Node = get_component("HealthComponent")
@onready var combat_component: Node = get_component("CombatComponent")

func _ready() -> void:
	if not movement_component:
		push_error("GridMovementComponent не найден на персонаже!")
		return
	movement_component.moved.connect(_on_grid_move)
	var start_grid_pos: Vector2i = movement_component.get_grid_position()
	global_position = GridMapSingleton.get_singleton().grid_to_pixel(start_grid_pos)
	
func _process(delta: float) -> void:
	pass

func _unhandled_input(event: InputEvent) -> void:
	var direction := Vector2i.ZERO
	
	if event.is_action_pressed("move_right"): direction = Vector2i(1, 0)
	elif event.is_action_pressed("move_left"): direction = Vector2i(-1, 0)
	elif event.is_action_pressed("move_down"): direction = Vector2i(0, 1)
	elif event.is_action_pressed("move_up"): direction = Vector2i(0, -1)
	
	if event.is_action_pressed("attack") and combat_component:
		var current_cell: Vector2i = movement_component.get_grid_position()

		var directions = [Vector2i.RIGHT, Vector2i.LEFT, Vector2i.DOWN, Vector2i.UP]
		
		for dir in directions:
			var target_cell = current_cell + dir
			var report = combat_component.attack(target_cell)

			if report.result != HealthComponent.DAMAGE_INVALID:
				break 

		GameManagerSingleton.get_singleton().trigger_enemy_turns()
		return
	
	if direction != Vector2i.ZERO:
		movement_component.try_move(direction)
		GameManagerSingleton.get_singleton().trigger_enemy_turns()
		return

func _on_grid_move(next_pos: Vector2i, prev_pos: Vector2i) -> void:
	var target_pixel_pos: Vector2 = GridMapSingleton.get_singleton().grid_to_pixel(next_pos)
	
	var tween := create_tween()
	tween.tween_property(self, "global_position", target_pixel_pos, 0.15)\
		.set_trans(Tween.TRANS_SINE)\
		.set_ease(Tween.EASE_IN_OUT)
