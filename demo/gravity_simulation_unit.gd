extends Node2D
class_name DemoCluster

var initial_size = 1; 

func _enter_tree() -> void:
	var rigidbody = $RigidBody2D
	var unit = $RigidBody2D/GravitySimulationUnit
	var sprite = $RigidBody2D/Sprite2D as Sprite2D
	var collision_shape = $RigidBody2D/CollisionShape2D as CollisionShape2D
	rigidbody.mass = initial_size * 5
	unit.mass = initial_size * 5
	sprite.scale *= Vector2.ONE * initial_size
	collision_shape.scale *= Vector2.ONE * initial_size
