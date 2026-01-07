extends Node2D

const object_to_paint = preload("res://gravity_simulation_unit.tscn")

@onready var simulation = $GravitySimulation

# Distance in pixels the mouse must move before spawning another object
@export var brush_spacing: float = 50.0

var last_paint_position: Vector2 = Vector2.ZERO
var is_painting: bool = false
var last_mouse_position = Vector2.ZERO

func _process(_delta):
	# Check for left mouse button click/hold
	if Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT):
		var current_mouse_pos = get_global_mouse_position()
		
		# If it's the first click or we've moved further than the spacing distance
		var distance = current_mouse_pos - last_paint_position

		if !is_painting or distance.length() > brush_spacing:
			var movement = current_mouse_pos - last_mouse_position
			var l = movement.length()
			var n = ceili(l/brush_spacing)
			var dir = movement/l

			for i in range(n):
				paint_object(last_mouse_position + dir * brush_spacing * i)
			
			last_paint_position = current_mouse_pos
			is_painting = true
			
	else:
		is_painting = false

	last_mouse_position = get_global_mouse_position()

func paint_object(pos):
	if object_to_paint == null:
		print("Please assign a scene to 'object_to_paint' in the Inspector!")
		return
		
	var cluster = object_to_paint.instantiate() as DemoCluster

	cluster.global_position = pos
	cluster.rotation = randf_range(0, TAU)
	cluster.initial_size = randf_range(.5, 2)

	simulation.add_child(cluster)
