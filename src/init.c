#include "init.h"
#include "api.h"
#include "defs.h"
#include "gdextension_interface.h"
#include "gravity_simulation.h"
#include "gravity_simulation_unit.h"


void initialize_gdexample_module(void *p_userdata, GDExtensionInitializationLevel p_level)
{
    if (p_level != GDEXTENSION_INITIALIZATION_SCENE)
    {
        return;
    }

    // Register class.
    StringName simulation_class_name;
    constructors.string_name_new_with_latin1_chars(&simulation_class_name, "GravitySimulation", false);
    StringName parent_class_name;
    constructors.string_name_new_with_latin1_chars(&parent_class_name, "Node", false);

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
        .get_virtual_call_data_func = gravity_simulation_class_get_virtual_with_data,
        .call_virtual_with_data_func = gravity_simulation_class_call_virtual_with_data,
        .get_rid_func = NULL,
        .class_userdata = NULL,
    };

    api.classdb_register_extension_class2(class_library, &simulation_class_name, &parent_class_name, &class_info);

    destructors.string_name_destructor(&parent_class_name);

    StringName unit_class_name;
    constructors.string_name_new_with_latin1_chars(&unit_class_name, "GravitySimulationUnit", false);
    constructors.string_name_new_with_latin1_chars(&parent_class_name, "Node2D", false);

    class_info.create_instance_func = gravity_simulation_unit_class_create_instance;
    class_info.free_instance_func = gravity_simulation_unit_class_free_instance;
    class_info.get_virtual_call_data_func = gravity_simulation_unit_class_get_virtual_with_data;
    class_info.call_virtual_with_data_func = gravity_simulation_unit_class_call_virtual_with_data;

    api.classdb_register_extension_class2(class_library, &unit_class_name, &parent_class_name, &class_info);

    destructors.string_name_destructor(&parent_class_name);
    destructors.string_name_destructor(&simulation_class_name);

    // Bind methods.
    gravity_simulation_class_bind_methods();
    gravity_simulation_unit_class_bind_methods();
    bind_api();
    // Destruct things.
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