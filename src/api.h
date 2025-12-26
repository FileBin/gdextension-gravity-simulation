
/*
This file works as a collection of helpers to call the GDExtension API
in a less verbose way, as well as a cache for methods from the discovery API,
just so we don't have to keep loading the same methods again.
*/

#ifndef C71A4301_2BAF_4AD7_8964_BFC167D79A0E
#define C71A4301_2BAF_4AD7_8964_BFC167D79A0E

#include "gdextension_interface.h"

extern GDExtensionClassLibraryPtr class_library;

// API methods.

struct Constructors
{
    GDExtensionInterfaceStringNameNewWithLatin1Chars string_name_new_with_latin1_chars;
    GDExtensionVariantFromTypeConstructorFunc variant_from_string_name_constructor;
    GDExtensionVariantFromTypeConstructorFunc variant_from_object_constructor;
};

struct Destructors
{
    GDExtensionPtrDestructor string_name_destructor;
    GDExtensionPtrDestructor variant_destroy;
};

struct Methods
{
    GDExtensionMethodBindPtr engine_register_singleton;
    GDExtensionMethodBindPtr engine_unregister_singleton;
};

struct API
{
    GDExtensionInterfaceClassdbRegisterExtensionClass2 classdb_register_extension_class2;
    GDExtensionInterfaceClassdbGetMethodBind classdb_get_method_bind;
    GDExtensionInterfaceClassdbGetClassTag classdb_get_class_tag;
    GDExtensionInterfaceGlobalGetSingleton global_get_singleton;
    GDExtensionInterfaceClassdbConstructObject classdb_construct_object;
    GDExtensionInterfaceObjectSetInstance object_set_instance;
    GDExtensionInterfaceObjectMethodBindCall object_method_bind_call;
    GDExtensionInterfaceObjectSetInstanceBinding object_set_instance_binding;
    GDExtensionInterfaceObjectCastTo object_cast_to;
    GDExtensionInterfaceGetVariantFromTypeConstructor get_variant_from_type_constructor;
    GDExtensionInterfaceMemAlloc mem_alloc;
    GDExtensionInterfaceMemFree mem_free;
};

#ifndef API_NO_EXTERN

extern struct API api;
extern struct Constructors constructors;
extern struct Destructors destructors;
extern struct Methods methods;

#endif

void load_api(GDExtensionInterfaceGetProcAddress p_get_proc_address);


#endif /* C71A4301_2BAF_4AD7_8964_BFC167D79A0E */
