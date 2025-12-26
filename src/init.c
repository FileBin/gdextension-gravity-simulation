#include "init.h"
#include "api.h"
#include "defs.h"
#include "gdextension_interface.h"
#include "gravity_simulation.h"


void initialize_gdexample_module(void *p_userdata, GDExtensionInitializationLevel p_level)
{
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE)
    {
        return;
    }

    // Register class.
    StringName class_name;
    constructors.string_name_new_with_latin1_chars(&class_name, "GravitySimulation", false);
    StringName parent_class_name;
    constructors.string_name_new_with_latin1_chars(&parent_class_name, "RefCounted", false);

    GDExtensionClassCreationInfo2 class_info = {
        .is_virtual = false,
        .is_abstract = false,
        .is_exposed = true,
        .set_func = NULL,
        .get_func = NULL,
        .get_property_list_func = NULL,
        .free_property_list_func = NULL,
        .property_can_revert_func = NULL,
        .property_get_revert_func = NULL,
        .validate_property_func = NULL,
        .notification_func = NULL,
        .to_string_func = NULL,
        .reference_func = NULL,
        .unreference_func = NULL,
        .create_instance_func = gravity_simulation_class_create_instance,
        .free_instance_func = gravity_simulation_class_free_instance,
        .recreate_instance_func = NULL,
        .get_virtual_func = NULL,
        .get_virtual_call_data_func = NULL,
        .call_virtual_with_data_func = NULL,
        .get_rid_func = NULL,
        .class_userdata = NULL,
    };

    api.classdb_register_extension_class2(class_library, &class_name, &parent_class_name, &class_info);


    StringName engine_name;
    StringName engine_register_singleton_name;
    StringName engine_unregister_singleton_name;

    constructors.string_name_new_with_latin1_chars(&engine_name, "Engine", false);
    constructors.string_name_new_with_latin1_chars(&engine_register_singleton_name, "register_singleton", false);
    constructors.string_name_new_with_latin1_chars(&engine_unregister_singleton_name, "unregister_singleton", false);

    GDExtensionObjectPtr engine = api.global_get_singleton(&engine_name);

    methods.engine_register_singleton = api.classdb_get_method_bind(&engine_name, &engine_register_singleton_name, 965313290);
    methods.engine_unregister_singleton = api.classdb_get_method_bind(&engine_name, &engine_unregister_singleton_name, 3304788590);


    GDExtensionObjectPtr instance = class_info.create_instance_func(class_info.class_userdata);

    Variant arg1, arg2, ret;

    constructors.variant_from_string_name_constructor(&arg1, &class_name);
    constructors.variant_from_object_constructor(&arg2, &instance);

    GDExtensionConstVariantPtr args[] = {&arg1, &arg2};


    api.object_method_bind_call(methods.engine_register_singleton, engine, args, 2, &ret, NULL);

    destructors.variant_destroy(&arg1);
    destructors.variant_destroy(&arg2);
    destructors.variant_destroy(&ret);

    destructors.string_name_destructor(&engine_name);
    destructors.string_name_destructor(&engine_register_singleton_name);
    destructors.string_name_destructor(&engine_unregister_singleton_name);

    // Bind methods.
    gravity_simulation_class_bind_methods();

    // Destruct things.
    destructors.string_name_destructor(&class_name);
    destructors.string_name_destructor(&parent_class_name);
}

void deinitialize_gdexample_module(void *p_userdata,
                                   GDExtensionInitializationLevel p_level) {}

GDExtensionBool GDE_EXPORT
gdexample_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                       GDExtensionClassLibraryPtr p_library,
                       GDExtensionInitialization *r_initialization) {
  class_library = p_library;
  load_api(p_get_proc_address);

  r_initialization->initialize = initialize_gdexample_module;
  r_initialization->deinitialize = deinitialize_gdexample_module;
  r_initialization->userdata = NULL;
  r_initialization->minimum_initialization_level =
      GDEXTENSION_INITIALIZATION_SCENE;

  return true;
}