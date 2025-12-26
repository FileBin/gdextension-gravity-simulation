#define API_NO_EXTERN
#include "api.h"
#include "defs.h"
#include "gdextension_interface.h"
#include "gravity_simulation.h"

GDExtensionClassLibraryPtr class_library = NULL;

struct API api;
struct Constructors constructors;
struct Destructors destructors;
struct Methods methods;

void load_api(GDExtensionInterfaceGetProcAddress p_get_proc_address)
{
    // Get helper functions first.
    GDExtensionInterfaceVariantGetPtrDestructor variant_get_ptr_destructor = (GDExtensionInterfaceVariantGetPtrDestructor)p_get_proc_address("variant_get_ptr_destructor");

    // API.
    api.classdb_register_extension_class2 = (GDExtensionInterfaceClassdbRegisterExtensionClass2)p_get_proc_address("classdb_register_extension_class2");
    api.classdb_get_method_bind = (GDExtensionInterfaceClassdbGetMethodBind)p_get_proc_address("classdb_get_method_bind");
    api.classdb_get_class_tag = (GDExtensionInterfaceClassdbGetClassTag)p_get_proc_address("classdb_get_class_tag");
    api.object_method_bind_call = (GDExtensionInterfaceObjectMethodBindCall)p_get_proc_address("object_method_bind_call");
    api.global_get_singleton = (GDExtensionInterfaceGlobalGetSingleton)p_get_proc_address("global_get_singleton");
    api.classdb_construct_object = (GDExtensionInterfaceClassdbConstructObject)p_get_proc_address("classdb_construct_object");
    api.get_variant_from_type_constructor = (GDExtensionInterfaceGetVariantFromTypeConstructor)p_get_proc_address("get_variant_from_type_constructor");
    api.object_set_instance = p_get_proc_address("object_set_instance");
    api.object_set_instance_binding = p_get_proc_address("object_set_instance_binding");
    api.object_cast_to = (GDExtensionInterfaceObjectCastTo)p_get_proc_address("object_cast_to");
    api.mem_alloc = (GDExtensionInterfaceMemAlloc)p_get_proc_address("mem_alloc");
    api.mem_free = (GDExtensionInterfaceMemFree)p_get_proc_address("mem_free");

    // Constructors.
    constructors.string_name_new_with_latin1_chars = (GDExtensionInterfaceStringNameNewWithLatin1Chars)p_get_proc_address("string_name_new_with_latin1_chars");
    constructors.variant_from_string_name_constructor = (GDExtensionVariantFromTypeConstructorFunc)api.get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);
    constructors.variant_from_object_constructor = (GDExtensionVariantFromTypeConstructorFunc)api.get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_OBJECT);

    // Destructors.
    destructors.string_name_destructor = variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);
    destructors.variant_destroy = p_get_proc_address("variant_destroy");
}