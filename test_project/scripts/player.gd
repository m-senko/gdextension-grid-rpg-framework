extends Node2D

# Константа размера клетки (должна строго совпадать с cell_size из C++)
const CELL_SIZE: int = 32

# Ссылка на C++ компонент перемещения
@onready var movement_component: GridMovementComponent = $GridMovementComponent
@onready var health_component: HealthComponent = $HealthComponent
@onready var combat_component: CombatComponent = $CombatComponent

func _ready() -> void:
	if not movement_component:
		push_error("GridMovementComponent не найден на персонаже!")
		return
		
	movement_component.moved.connect(_on_grid_move)
	
	var start_grid_pos: Vector2i = movement_component.get_grid_position()
	
	global_position = Vector2(
		(start_grid_pos.x * CELL_SIZE),
		(start_grid_pos.y * CELL_SIZE)
	)
	
func _process(delta: float):
	pass

func _unhandled_input(event: InputEvent) -> void:
	var direction := Vector2i.ZERO
	
	if event.is_action_pressed("move_right"): direction = Vector2i(1, 0)
	elif event.is_action_pressed("move_left"): direction = Vector2i(-1, 0)
	elif event.is_action_pressed("move_down"): direction = Vector2i(0, 1)
	elif event.is_action_pressed("move_up"): direction = Vector2i(0, -1)
	
	if event.is_action_pressed("attack"):
		combat_component.attack(movement_component.grid_position + Vector2i(1,0))
	
	if direction != Vector2i.ZERO:
		movement_component.try_move(direction)

func _on_grid_move(next_pos: Vector2i, prev_pos: Vector2i) -> void:
	var target_pixel_pos := Vector2(
		(next_pos.x * CELL_SIZE),
		(next_pos.y * CELL_SIZE)
	)
	
	var tween := create_tween()
	
	tween.tween_property(self, "global_position", target_pixel_pos, 0.15)\
		.set_trans(Tween.TRANS_SINE)\
		.set_ease(Tween.EASE_IN_OUT)
