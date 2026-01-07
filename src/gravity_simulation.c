#include "gravity_simulation.h"
#include "api.h"
#include "defs.h"
#include "gdextension_interface.h"
#include "gravity_simulation_unit.h"
#include "sys/types.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define LINKED_LIST_TYPE GDExtensionObjectPtr
#include "linked_list.h"
#undef LINKED_LIST_TYPE

typedef GravitySimulationUnit *GravitySimulationUnitPtr;

#define LINKED_LIST_TYPE GravitySimulationUnitPtr
#include "linked_list.h"
#undef LINKED_LIST_TYPE

extern const GDExtensionInstanceBindingCallbacks gravity_simulation_unit_class_binding_callbacks;

typedef struct {
  GDExtensionObjectPtr rigidbody;
  uint units_count;
  linked_list_GravitySimulationUnitPtr *units_list; // list of pointers to GravitySimulationUnit objects in scene tree
} cluster;

#define LINKED_LIST_TYPE cluster
#include "linked_list.h"
#undef LINKED_LIST_TYPE

const GDExtensionInstanceBindingCallbacks
    gravity_simulation_class_binding_callbacks = {
        .create_callback = NULL,
        .free_callback = NULL,
        .reference_callback = NULL,
};

void gravity_simulation_class_constructor(GravitySimulation *self) {
  self->gravity_clusters.clusters_count = 0;
  self->gravity_clusters.clusters_list = NULL;

  self->visit_tree = true;

  self->G = 6.67 * 100000;
}

void *gravity_simulation_class_get_virtual_with_data(
    void *p_class_userdata, GDExtensionConstStringNamePtr p_name) {
  if (is_string_name_equal(p_name, "_ready")) {
    return (void *)gravity_simulation_class_ready;
  }

  if (is_string_name_equal(p_name, "_physics_process")) {
    return (void *)gravity_simulation_class_process;
  }

  // Otherwise, return NULL.
  return NULL;
}

void gravity_simulation_class_call_virtual_with_data(
    GDExtensionClassInstancePtr p_instance,
    GDExtensionConstStringNamePtr p_name, void *p_virtual_call_userdata,
    const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret) {
  if (p_virtual_call_userdata == &gravity_simulation_class_ready) {
    ptrcall_0_args_no_ret(p_virtual_call_userdata, p_instance, p_args, r_ret);
  }
  if (p_virtual_call_userdata == &gravity_simulation_class_process) {
    ptrcall_1_float_arg_no_ret(p_virtual_call_userdata, p_instance, p_args,
                               r_ret);
  }
}

void gravity_simulation_class_destructor(GravitySimulation *self) {}

void gravity_simulation_class_bind_methods() {
  bind_method_0("GravitySimulation", "_on_tree_changed",
                gravity_simulation_class_on_tree_changed);

  bind_method_0_r("GravitySimulation", "get_G",
                  gravity_simulation_class_get_G,
                  GDEXTENSION_VARIANT_TYPE_FLOAT);
  bind_method_1("GravitySimulation", "set_G",
                gravity_simulation_class_set_G, "G",
                GDEXTENSION_VARIANT_TYPE_FLOAT);
  bind_property("GravitySimulation", "G", GDEXTENSION_VARIANT_TYPE_FLOAT,
                "get_G", "set_G");
}

void gravity_simulation_class_set_G(GravitySimulation* self, double G) {
  self->G = G;
}

double gravity_simulation_class_get_G(GravitySimulation* self) {
  return self->G;
}

GDExtensionObjectPtr
gravity_simulation_class_create_instance(void *p_class_userdata) {
  // Create native Godot object;
  StringName class_name;
  constructors.string_name_new_with_latin1_chars(&class_name, "Node", false);
  GDExtensionObjectPtr object = api.classdb_construct_object(&class_name);
  destructors.string_name_destructor(&class_name);

  // Create extension object.
  GravitySimulation *self =
      (GravitySimulation *)api.mem_alloc(sizeof(GravitySimulation));
  gravity_simulation_class_constructor(self);
  self->object = object;

  // Set the extension instance in the native Godot object.
  constructors.string_name_new_with_latin1_chars(&class_name,
                                                 "GravitySimulation", false);
  api.object_set_instance(object, &class_name, self);
  api.object_set_instance_binding(object, class_library, self,
                                  &gravity_simulation_class_binding_callbacks);
  destructors.string_name_destructor(&class_name);

  return object;
}

void gravity_simulation_class_free_instance(
    void *p_class_userdata, GDExtensionClassInstancePtr p_instance) {
  if (p_instance == NULL) {
    return;
  }
  GravitySimulation *self = (GravitySimulation *)p_instance;
  gravity_simulation_class_destructor(self);
  api.mem_free(self);
}

void gravity_simulation_class_ready(GravitySimulation *self) {
  Variant variant_ret, arg1, arg2, callable_variant;
  GDObjectInstanceID tree_instance_id;
  GDExtensionObjectPtr scene_tree;
  StringName method_name_string, signal_name_string;
  GDExtensionCallError error;

  GDExtensionConstVariantPtr args[] = {};
  api.object_method_bind_call(methods.node_get_tree, self->object, args, 0,
                              &variant_ret, NULL);
  tree_instance_id = api.variant_get_object_instance_id(&variant_ret);
  destructors.variant_destroy(&variant_ret);

  if (!tree_instance_id)
    return;

  scene_tree = api.object_get_instance_from_id(tree_instance_id);

  constructors.string_name_new_with_latin1_chars(&method_name_string,
                                                 "_on_tree_changed", false);

  constructors.variant_from_object_constructor(&arg1, &self->object);
  constructors.variant_from_string_name_constructor(&arg2, &method_name_string);

  destructors.string_name_destructor(&method_name_string);

  GDExtensionConstVariantPtr constructor_args[] = {&arg1, &arg2};
  constructors.variant_construct(GDEXTENSION_VARIANT_TYPE_CALLABLE,
                                 &callable_variant, constructor_args, 2,
                                 &error);

  destructors.variant_destroy(&arg1);
  destructors.variant_destroy(&arg2);

  if (error.error != GDEXTENSION_CALL_OK) {
    return;
  }

  constructors.string_name_new_with_latin1_chars(&signal_name_string,
                                                 "tree_changed", false);

  constructors.variant_from_string_name_constructor(&arg1, &signal_name_string);

  GDExtensionConstVariantPtr connect_args[] = {&arg1, &callable_variant};
  api.object_method_bind_call(methods.object_connect, scene_tree, connect_args,
                              2, &variant_ret, &error);
  destructors.variant_destroy(&arg1);
  destructors.variant_destroy(&callable_variant);
  destructors.variant_destroy(&variant_ret);
  destructors.string_name_destructor(&signal_name_string);

  if (error.error != GDEXTENSION_CALL_OK) {
    return;
  }
}

typedef struct {
  Vector2 force;
  Vector2 point;
} force_info;

typedef struct {
  force_info* forces;
  linked_list_GravitySimulationUnitPtr* units;
  GDExtensionObjectPtr rigidbody2d;
  uint count;
} cluster_forces;

typedef struct {
  cluster_forces *cluster_forces;
  uint count;
} cluster_force_info;

void update_clusters(GravitySimulation *self);
cluster_force_info run_simulation_CPU(GravitySimulation *self);

void gravity_simulation_class_process(GravitySimulation *self, double delta) {
  if (is_editor()) {
    return;
  }

  if (self->visit_tree) {
    self->visit_tree = false;
    update_clusters(self);
  }
  
  cluster_force_info simulation_result = run_simulation_CPU(self);

  for (uint i = 0; i < simulation_result.count; i++) {
    cluster_forces cluster_forces = simulation_result.cluster_forces[i];
    GDExtensionObjectPtr rigidbody2d = cluster_forces.rigidbody2d;
    for (uint j = 0; j < cluster_forces.count; j++) {
      force_info unit_forces_info = cluster_forces.forces[j];

      Variant force, position, ret;

      constructors.variant_from_vector2_constructor(&force, &unit_forces_info.force);
      constructors.variant_from_vector2_constructor(&position, &unit_forces_info.point);

      GDExtensionConstVariantPtr args[] = {&force, &position};
      api.object_method_bind_call(methods.rigidbody2d_apply_force, rigidbody2d, args, 2, &ret, NULL);

      destructors.variant_destroy(&force);
      destructors.variant_destroy(&position);
      destructors.variant_destroy(&ret);
    }

    if (cluster_forces.forces)
      free(cluster_forces.forces);
  }

  if (simulation_result.cluster_forces)
    free(simulation_result.cluster_forces);
}

void gravity_simulation_class_on_tree_changed(GravitySimulation *self) {
  self->visit_tree = true;
}

void update_clusters(GravitySimulation *self) {

  void *gravity_simulation_unit_class_tag = classdb_get_class_tag("GravitySimulationUnit");
  void *rigidbody2d_class_tag = classdb_get_class_tag("RigidBody2D");

  linked_list_cluster *clusters = 0;
  uint clusters_count = 0;

  linked_list_GDExtensionObjectPtr *stack = 0;

  linked_list_GDExtensionObjectPtr_push_front(&stack, self->object);

  cluster* current_cluster_ptr = 0;

  while (stack) {
    Variant children_variant_ret, child_variant_ret;
    GDExtensionCallError error;
    GDExtensionObjectPtr node = linked_list_GDExtensionObjectPtr_pop_front(&stack);

    GDExtensionObjectPtr found_rigidbody2d =
        api.object_cast_to(node, rigidbody2d_class_tag);

    if (found_rigidbody2d) {
      cluster new_cluster;

      new_cluster.rigidbody = found_rigidbody2d;
      new_cluster.units_list = 0;
      new_cluster.units_count = 0;
      linked_list_cluster_push_front(&clusters, new_cluster);
      clusters_count++;
      current_cluster_ptr = &clusters->data;
    } else {

      GDExtensionObjectPtr found_unit =
          api.object_cast_to(node, gravity_simulation_unit_class_tag);

      if (found_unit && current_cluster_ptr) {
        GravitySimulationUnitPtr unit = api.object_get_instance_binding(found_unit, class_library, NULL);
        if (unit) {
          linked_list_GravitySimulationUnitPtr_push_front(&current_cluster_ptr->units_list, unit);
          current_cluster_ptr->units_count++;
        }
      }
    }

    GDExtensionConstVariantPtr args[] = {};
    api.object_method_bind_call(methods.node_get_children, node, args,
                                0, &children_variant_ret, &error);

    Variant iter;
    GDExtensionBool success, valid;

    success = api.variant_iter_init(&children_variant_ret, &iter, &valid);

    if (!success) {
      valid = false;
    }

    while (valid) {
      api.variant_iter_get(&children_variant_ret, &iter, &child_variant_ret, &valid);
      if (valid) {
        GDObjectInstanceID child_instance_id = api.variant_get_object_instance_id(&child_variant_ret);
        destructors.variant_destroy(&child_variant_ret);

        if (child_instance_id) {
          GDExtensionObjectPtr child_node = api.object_get_instance_from_id(child_instance_id);

          linked_list_GDExtensionObjectPtr_push_front(&stack, child_node);
        }
      }

      success = api.variant_iter_next(&children_variant_ret, &iter, &valid);
      
      if (!success) {
        valid = false;
      }
    }

    destructors.variant_destroy(&children_variant_ret);
  }

  linked_list_GDExtensionObjectPtr_destroy(stack);

  self->gravity_clusters.clusters_count = clusters_count;
  linked_list_cluster *old_list = self->gravity_clusters.clusters_list;
  self->gravity_clusters.clusters_list = clusters;

  for (linked_list_cluster *iter = old_list; iter; iter = iter->p_next) {
    linked_list_GravitySimulationUnitPtr_destroy(iter->data.units_list);
  }
  linked_list_cluster_destroy(old_list);
}

cluster_force_info run_simulation_CPU(GravitySimulation *self) {
  if (self->gravity_clusters.clusters_count < 1) {
    cluster_force_info empty = {
      .count = 0,
      .cluster_forces = 0,
    };

    return empty;
  }

  double G = self->G;

  cluster_force_info cluster_force_info;

  cluster_force_info.count = self->gravity_clusters.clusters_count;

  cluster_force_info.cluster_forces = calloc(cluster_force_info.count, sizeof(cluster_forces));
  memset(cluster_force_info.cluster_forces, 0, sizeof(cluster_forces) * cluster_force_info.count);
  
  {
    cluster_forces* cluster_force_iterator = cluster_force_info.cluster_forces;
    for (linked_list_cluster *iter = self->gravity_clusters.clusters_list; iter; iter = iter->p_next) {
      cluster_force_iterator->units = iter->data.units_list;
      cluster_force_iterator->count = iter->data.units_count;
      cluster_force_iterator->rigidbody2d = iter->data.rigidbody;
      if(cluster_force_iterator->count < 1) {
        cluster_force_iterator->forces = 0;
      } else {
        cluster_force_iterator->forces = calloc(cluster_force_iterator->count, sizeof(force_info));
        memset(cluster_force_iterator->forces, 0, sizeof(force_info) * cluster_force_iterator->count);
      }
      cluster_force_iterator++;
    }
  }


  for (uint i = 0; i < cluster_force_info.count; i++) {
    cluster_forces* cluster_forces_i = cluster_force_info.cluster_forces + i;
    for (uint j = i+1; j < cluster_force_info.count; j++) {
      cluster_forces* cluster_forces_j = cluster_force_info.cluster_forces + j;
      force_info *unit_a_force_iterator = cluster_forces_i->forces;
      for (linked_list_GravitySimulationUnitPtr *unit_a = cluster_forces_i->units; unit_a; unit_a = unit_a->p_next) {
        force_info *unit_b_force_iterator = cluster_forces_j->forces;
        for (linked_list_GravitySimulationUnitPtr *unit_b = cluster_forces_j->units; unit_b; unit_b = unit_b->p_next) {
          Vector2 position_a = gravity_simulation_unit_class_get_global_position(unit_a->data);
          Vector2 position_b = gravity_simulation_unit_class_get_global_position(unit_b->data);
          Vector2 r = {
            .x = position_b.x - position_a.x,
            .y = position_b.y - position_a.y,
          };

          double r2 = r.x * r.x + r.y * r.y;
          double r_length = sqrt(r2);

          Vector2 direction = {
            .x = r.x / r_length,
            .y = r.y / r_length,
          };

          double force_magnitude = G * unit_a->data->mass * unit_b->data->mass / r2;

          Vector2 force = {
            .x = direction.x * force_magnitude,
            .y = direction.y * force_magnitude,
          };

          unit_a_force_iterator->force.x += force.x;
          unit_a_force_iterator->force.y += force.y;

          unit_b_force_iterator->force.x -= force.x;
          unit_b_force_iterator->force.y -= force.y;

          unit_b_force_iterator->point = gravity_simulation_unit_class_get_position(unit_b->data);
          unit_b_force_iterator++;
        }

        unit_a_force_iterator->point = gravity_simulation_unit_class_get_position(unit_a->data);
        unit_a_force_iterator++;
      }
    }
  }

  return cluster_force_info;
}