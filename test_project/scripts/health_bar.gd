extends ProgressBar

@onready var label : Label = $Label

func _ready():
	# owner — это наш PlayerHud. Ждем один кадр, чтобы корень успел получить ссылку
	await get_tree().process_frame
	
	var hud = owner as CanvasLayer
	if hud and hud.player:
		_setup_health(hud.player)

func _setup_health(player: Node2D):
	var health_comp = player.health_component
	if health_comp:
		health_comp.health_changed.connect(_on_health_changed)
		print(health_comp)
		_on_health_changed(health_comp.current_health, health_comp.current_health, health_comp.max_health)

func _on_health_changed(next: float, prev: float, max_hp: float):
	max_value = max_hp
	value = next
	label.text = "%d / %d" %[next, max_hp]
