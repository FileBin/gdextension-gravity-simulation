#include "gravity_simulation.h"
#include "api.h"
#include "defs.h"
#include "gdextension_interface.h"
#include "gravity_simulation_unit.h"
#include <stdbool.h>
#include <stdio.h>

#define LINKED_LIST_TYPE GDExtensionObjectPtr
#include "linked_list.h"
#undef LINKED_LIST_TYPE

typedef struct {
  GDExtensionObjectPtr rigidbody;
  linked_list_GDExtensionObjectPtr *units; // list of pointers to GravitySimulationUnit objects in scene tree
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
  self->gravity_clusters = NULL;
  self->visit_tree = true;
}

void *gravity_simulation_class_get_virtual_with_data(
    void *p_class_userdata, GDExtensionConstStringNamePtr p_name) {
  if (is_string_name_equal(p_name, "_ready")) {
    return (void *)gravity_simulation_class_ready;
  }

  if (is_string_name_equal(p_name, "_process")) {
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

void update_clusters(GravitySimulation *self);

void gravity_simulation_class_process(GravitySimulation *self, double delta) {
  if (self->visit_tree) {
    self->visit_tree = false;
    update_clusters(self);
  }
}

void gravity_simulation_class_on_tree_changed(GravitySimulation *self) {
  // TODO iterate over tree
  // NOTE: set tree changed flag and update in _process cause its too many
  // messages per frame
  self->visit_tree = true;
}

void update_clusters(GravitySimulation *self) {

  void *gravity_simulation_unit_class_tag = classdb_get_class_tag("GravitySimulationUnit");
  void *rigidbody2d_class_tag = classdb_get_class_tag("RigidBody2D");

  linked_list_cluster *clusters = 0;

  linked_list_GDExtensionObjectPtr *stack = 0;

  linked_list_GDExtensionObjectPtr_push_front(&stack, self->object);

  linked_list_GDExtensionObjectPtr** current_units_list_ptr = 0;

  while (stack) {
    Variant children_variant_ret, child_variant_ret;
    GDExtensionCallError error;
    GDExtensionObjectPtr node = linked_list_GDExtensionObjectPtr_pop_front(&stack);

    GDExtensionObjectPtr found_rigidbody2d =
        api.object_cast_to(node, rigidbody2d_class_tag);

    if (found_rigidbody2d) {
      cluster new_cluster;

      new_cluster.rigidbody = found_rigidbody2d;
      new_cluster.units = 0;
      linked_list_cluster_push_front(&clusters, new_cluster);
      current_units_list_ptr = &clusters->data.units;
    } else {

      GDExtensionObjectPtr found_unit =
          api.object_cast_to(node, gravity_simulation_unit_class_tag);

      if (found_unit) {
        if (current_units_list_ptr) {
          linked_list_GDExtensionObjectPtr_push_front(current_units_list_ptr, found_unit);
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

    if (error.error != GDEXTENSION_CALL_OK) {
      return;
    }
  }
}