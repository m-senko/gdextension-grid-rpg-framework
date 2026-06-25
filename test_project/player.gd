extends Node2D

# Константа размера клетки (должна строго совпадать с cell_size из C++)
const CELL_SIZE: int = 32

# Ссылка на C++ компонент перемещения
@onready var movement_component: Node = $GridMovementComponent

func _ready() -> void:
	# 1. Безопасно проверяем, подключился ли C++ компонент
	if not movement_component:
		push_error("GridMovementComponent не найден на персонаже!")
		return
		
	# 2. Подписываемся на C++ сигнал перемещения
	# Имя сигнала ("move") должно строго совпадать с тем, что зарегистрировано в ClassDB
	movement_component.moved.connect(_on_grid_move)
	
	# 3. Синхронизируем стартовую позицию в пикселях
	# Считываем из C++ стартовую grid_position, которую вы выставили в Инспекторе
	var start_grid_pos: Vector2i = movement_component.get_grid_position()
	
	# Переводим клетки в пиксели мира и центрируем спрайт (+16 пикселей)
	global_position = Vector2(
		(start_grid_pos.x * CELL_SIZE),
		(start_grid_pos.y * CELL_SIZE)
	)

func _unhandled_input(event: InputEvent) -> void:
	# Считываем ввод игрока (WASD или стрелочки)
	var direction := Vector2i.ZERO
	
	if event.is_action_pressed("move_right"): direction = Vector2i(1, 0)
	elif event.is_action_pressed("move_left"): direction = Vector2i(-1, 0)
	elif event.is_action_pressed("move_down"): direction = Vector2i(0, 1)
	elif event.is_action_pressed("move_up"): direction = Vector2i(0, -1)
	
	# Если направление выбрано, отдаем его на просчет в C++
	if direction != Vector2i.ZERO:
		# try_move вернет true, если клетка свободна, и сам поменяет данные в матрице карты
		var can_move: bool = movement_component.try_move(direction)
		
		if not can_move:
			# Если C++ вернул false, значит там стена или враг — можно проиграть звук "стука"
			print("Way is blocked!")

# Этот метод автоматически сработает, когда C++ успешно выполнит математический шаг
func _on_grid_move(next_pos: Vector2i, prev_pos: Vector2i) -> void:
	# Рассчитываем целевую пиксельную точку в мире Godot
	var target_pixel_pos := Vector2(
		(next_pos.x * CELL_SIZE),
		(next_pos.y * CELL_SIZE)
	)
	
	# Создаем плавную интерполяцию (Tween) силами Godot
	# Персонаж больше не телепортируется, а плавно скользит в новую клетку!
	var tween := create_tween()
	
	# Перемещаем свойство "global_position" этой ноды до target_pixel_pos за 0.15 секунды
	# TRANS_SINE и EASE_IN_OUT сделают движение мягким на старте и финише
	tween.tween_property(self, "global_position", target_pixel_pos, 0.15)\
		.set_trans(Tween.TRANS_SINE)\
		.set_ease(Tween.EASE_IN_OUT)
