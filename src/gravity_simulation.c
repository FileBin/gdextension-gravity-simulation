#include "gravity_simulation.h"
#include "defs.h"
#include "api.h"

const GDExtensionInstanceBindingCallbacks gravity_simulation_class_binding_callbacks = {
    .create_callback = NULL,
    .free_callback = NULL,
    .reference_callback = NULL,
};

void gravity_simulation_class_constructor(GravitySimulation *self)
{
}

void gravity_simulation_class_destructor(GravitySimulation *self)
{
}

void gravity_simulation_class_bind_methods()
{
}

GDExtensionObjectPtr gravity_simulation_class_create_instance(void *p_class_userdata)
{
    // Create native Godot object;
    StringName class_name;
    constructors.string_name_new_with_latin1_chars(&class_name, "RefCounted", false);
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

void gravity_simulation_class_free_instance(void *p_class_userdata, GDExtensionClassInstancePtr p_instance)
{
    if (p_instance == NULL)
    {
        return;
    }
    GravitySimulation *self = (GravitySimulation *)p_instance;
    gravity_simulation_class_destructor(self);
    api.mem_free(self);
}