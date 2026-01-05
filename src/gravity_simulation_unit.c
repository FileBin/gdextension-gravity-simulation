#include "gravity_simulation_unit.h"
#include "defs.h"
#include "api.h"
#include "gdextension_interface.h"

const GDExtensionInstanceBindingCallbacks gravity_simulation_unit_class_binding_callbacks = {
    .create_callback = NULL,
    .free_callback = NULL,
    .reference_callback = NULL,
};

void gravity_simulation_unit_class_constructor(GravitySimulationUnit *self) {
    self->mass = 1;
}

void gravity_simulation_unit_class_destructor(GravitySimulationUnit *self) {}

void gravity_simulation_unit_class_bind_methods() {
    bind_method_0_r("GravitySimulationUnit", "get_mass", gravity_simulation_unit_class_get_mass, GDEXTENSION_VARIANT_TYPE_FLOAT);
    bind_method_1("GravitySimulationUnit", "set_mass", gravity_simulation_unit_class_set_mass, "mass", GDEXTENSION_VARIANT_TYPE_FLOAT);
    bind_property("GravitySimulationUnit", "mass", GDEXTENSION_VARIANT_TYPE_FLOAT, "get_mass", "set_mass");
}

void *gravity_simulation_unit_class_get_virtual_with_data(void *p_class_userdata, GDExtensionConstStringNamePtr p_name) {
    return NULL;
}

void gravity_simulation_unit_class_call_virtual_with_data(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, void *p_virtual_call_userdata, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret) { }


void gravity_simulation_unit_class_ready(GravitySimulationUnit *self) { }

GDExtensionObjectPtr gravity_simulation_unit_class_create_instance(void *p_class_userdata)
{
    // Create native Godot object;
    StringName class_name;
    constructors.string_name_new_with_latin1_chars(&class_name, "Node2D", false);
    GDExtensionObjectPtr object = api.classdb_construct_object(&class_name);
    destructors.string_name_destructor(&class_name);

    // Create extension object.
    GravitySimulationUnit *self = (GravitySimulationUnit *)api.mem_alloc(sizeof(GravitySimulationUnit));
    gravity_simulation_unit_class_constructor(self);
    self->node2d = object;

    // Set the extension instance in the native Godot object.
    constructors.string_name_new_with_latin1_chars(&class_name, "GravitySimulationUnit", false);
    api.object_set_instance(object, &class_name, self);
    api.object_set_instance_binding(object, class_library, self, &gravity_simulation_unit_class_binding_callbacks);
    destructors.string_name_destructor(&class_name);

    return object;
}

void gravity_simulation_unit_class_free_instance(void *p_class_userdata, GDExtensionClassInstancePtr p_instance) {
    if (p_instance == NULL)
        return;

    GravitySimulationUnit *self = (GravitySimulationUnit *)p_instance;
    gravity_simulation_unit_class_destructor(self);
    api.mem_free(self);
}

void gravity_simulation_unit_class_set_mass(GravitySimulationUnit* self, double mass) {
    self->mass = (real)mass;
}

double gravity_simulation_unit_class_get_mass(GravitySimulationUnit* self) {
    return (double)self->mass;
}
