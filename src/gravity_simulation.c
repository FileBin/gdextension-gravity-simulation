#include "gravity_simulation.h"
#include "defs.h"
#include "api.h"
#include "gdextension_interface.h"
#include <stdio.h>

const GDExtensionInstanceBindingCallbacks gravity_simulation_class_binding_callbacks = {
    .create_callback = NULL,
    .free_callback = NULL,
    .reference_callback = NULL,
};

void gravity_simulation_class_constructor(GravitySimulation *self)
{
    self->gravity_clusters = NULL;
}

void *gravity_simulation_class_get_virtual_with_data(void *p_class_userdata, GDExtensionConstStringNamePtr p_name) {
    if (is_string_name_equal(p_name, "_ready")) {
        return (void *)gravity_simulation_class_ready;
    }
    // Otherwise, return NULL.
    return NULL;
}

void gravity_simulation_class_call_virtual_with_data(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, void *p_virtual_call_userdata, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret){
    if (p_virtual_call_userdata == &gravity_simulation_class_ready)
    {
        ptrcall_0_args_no_ret(p_virtual_call_userdata, p_instance, p_args, r_ret);
    }
}

void gravity_simulation_class_destructor(GravitySimulation *self) { }

void gravity_simulation_class_bind_methods() {
    bind_method_0("GravitySimulation", "_on_tree_changed", gravity_simulation_class_on_tree_changed);
}

GDExtensionObjectPtr gravity_simulation_class_create_instance(void *p_class_userdata) {
    // Create native Godot object;
    StringName class_name;
    constructors.string_name_new_with_latin1_chars(&class_name, "Node", false);
    GDExtensionObjectPtr object = api.classdb_construct_object(&class_name);
    destructors.string_name_destructor(&class_name);

    // Create extension object.
    GravitySimulation *self = (GravitySimulation *)api.mem_alloc(sizeof(GravitySimulation));
    gravity_simulation_class_constructor(self);
    self->object = object;

    // Set the extension instance in the native Godot object.
    constructors.string_name_new_with_latin1_chars(&class_name, "GravitySimulation", false);
    api.object_set_instance(object, &class_name, self);
    api.object_set_instance_binding(object, class_library, self, &gravity_simulation_class_binding_callbacks);
    destructors.string_name_destructor(&class_name);

    return object;
}

void gravity_simulation_class_free_instance(void *p_class_userdata, GDExtensionClassInstancePtr p_instance) {
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
    api.object_method_bind_call(methods.node_get_tree, self->object, args, 0, &variant_ret, NULL);
    tree_instance_id = api.variant_get_object_instance_id(&variant_ret);
    destructors.variant_destroy(&variant_ret);

    if (!tree_instance_id)
      return;

    scene_tree = api.object_get_instance_from_id(tree_instance_id);

    constructors.string_name_new_with_latin1_chars(&method_name_string, "_on_tree_changed", false);

    constructors.variant_from_object_constructor(&arg1, &self->object);
    constructors.variant_from_string_name_constructor(&arg2, &method_name_string);

    destructors.string_name_destructor(&method_name_string);


    GDExtensionConstVariantPtr constructor_args[] = {&arg1, &arg2};
    constructors.variant_construct(GDEXTENSION_VARIANT_TYPE_CALLABLE, &callable_variant, constructor_args, 2, &error);

    destructors.variant_destroy(&arg1);
    destructors.variant_destroy(&arg2);

    if (error.error != GDEXTENSION_CALL_OK) {
        return;
    }

    constructors.string_name_new_with_latin1_chars(&signal_name_string, "tree_changed", false);

    constructors.variant_from_string_name_constructor(&arg1, &signal_name_string);

    GDExtensionConstVariantPtr connect_args[] = {&arg1, &callable_variant};
    api.object_method_bind_call(methods.object_connect, scene_tree, connect_args, 2, &variant_ret, &error);
    destructors.variant_destroy(&arg1);
    destructors.variant_destroy(&callable_variant);
    destructors.variant_destroy(&variant_ret);
    destructors.string_name_destructor(&signal_name_string);

    if (error.error != GDEXTENSION_CALL_OK) {
        return;
    }
}

void gravity_simulation_class_on_tree_changed(GravitySimulation *self) {
    // TODO iterate over tree
    // NOTE: set tree changed flag and update in _process cause its too many messages per frame
    puts("gravity_simulation_class_on_tree_changed called");
}