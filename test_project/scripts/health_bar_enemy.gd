extends ProgressBar

@onready var label : Label = $Label

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	await get_tree().process_frame
	var actor = owner as Actor
	if actor:
		_setup_health(actor)

func _setup_health(actor: Actor):
	var health_comp = actor.get_component("HealthComponent")
	if health_comp:
		health_comp.health_changed.connect(_on_health_changed)
		_on_health_changed(health_comp.current_health, health_comp.current_health, health_comp.max_health)

func _on_health_changed(next: float, prev: float, max_hp: float):
	max_value = max_hp
	value = next
	label.text = "%d / %d" %[next, max_hp]
