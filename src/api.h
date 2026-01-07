
/*
This file works as a collection of helpers to call the GDExtension API
in a less verbose way, as well as a cache for methods from the discovery API,
just so we don't have to keep loading the same methods again.
*/

#ifndef C71A4301_2BAF_4AD7_8964_BFC167D79A0E
#define C71A4301_2BAF_4AD7_8964_BFC167D79A0E

#include "gdextension_interface.h"
#include <stdbool.h>

extern GDExtensionClassLibraryPtr class_library;

// API methods.

struct Constructors
{
    GDExtensionInterfaceStringNameNewWithLatin1Chars string_name_new_with_latin1_chars;
    GDExtensionVariantFromTypeConstructorFunc variant_from_string_name_constructor;
    GDExtensionVariantFromTypeConstructorFunc variant_from_vector2_constructor;
    GDExtensionVariantFromTypeConstructorFunc variant_from_object_constructor;
    GDExtensionVariantFromTypeConstructorFunc variant_from_float_constructor;
    GDExtensionInterfaceStringNewWithUtf8Chars string_new_with_utf8_chars;
    
    GDExtensionTypeFromVariantConstructorFunc float_from_variant_constructor;
    GDExtensionTypeFromVariantConstructorFunc vector2_from_variant_constructor;
    GDExtensionTypeFromVariantConstructorFunc bool_from_variant_constructor;
    GDExtensionInterfaceVariantConstruct variant_construct;
};

struct Destructors
{
    GDExtensionPtrDestructor string_name_destructor;
    GDExtensionPtrDestructor string_destructor;
    GDExtensionPtrDestructor variant_destroy;
};

struct Methods
{
    GDExtensionMethodBindPtr engine_is_editor_hint;

    GDExtensionMethodBindPtr node_get_parent;
    GDExtensionMethodBindPtr node_get_children;
    GDExtensionMethodBindPtr node_get_tree;
    GDExtensionMethodBindPtr node2d_get_position;
    GDExtensionMethodBindPtr node2d_get_global_position;
    GDExtensionMethodBindPtr rigidbody2d_apply_force;
    
    GDExtensionMethodBindPtr object_connect;
};

struct Operators
{
    GDExtensionPtrOperatorEvaluator string_name_equal;
};

struct API
{
    GDExtensionInterfaceClassdbRegisterExtensionClass2 classdb_register_extension_class2;
    GDExtensionInterfaceClassdbRegisterExtensionClassMethod classdb_register_extension_class_method;
    GDExtensionInterfaceClassdbRegisterExtensionClassProperty classdb_register_extension_class_property;
    GDExtensionInterfaceClassdbGetMethodBind classdb_get_method_bind;
    GDExtensionInterfaceClassdbGetClassTag classdb_get_class_tag;
    GDExtensionInterfaceClassdbConstructObject classdb_construct_object;
    
    GDExtensionInterfaceGlobalGetSingleton global_get_singleton;
    
    GDExtensionInterfaceObjectSetInstance object_set_instance;
    GDExtensionInterfaceObjectMethodBindCall object_method_bind_call;
    GDExtensionInterfaceObjectSetInstanceBinding object_set_instance_binding;
    GDExtensionInterfaceObjectGetInstanceBinding object_get_instance_binding;
    GDExtensionInterfaceObjectCastTo object_cast_to;
    GDExtensionInterfaceObjectGetInstanceFromId object_get_instance_from_id;
    
    GDExtensionInterfaceGetVariantFromTypeConstructor get_variant_from_type_constructor;
    GDExtensionInterfaceGetVariantToTypeConstructor get_variant_to_type_constructor;
    GDExtensionInterfaceVariantGetType variant_get_type;
    GDExtensionInterfaceVariantGetObjectInstanceId variant_get_object_instance_id;
    GDExtensionInterfaceVariantIterInit variant_iter_init;
    GDExtensionInterfaceVariantIterNext variant_iter_next;
    GDExtensionInterfaceVariantIterGet variant_iter_get;

    GDExtensionInterfaceMemAlloc mem_alloc;
    GDExtensionInterfaceMemFree mem_free;

    GDExtensionObjectPtr engine;
};

#ifndef API_NO_EXTERN

extern struct API api;
extern struct Constructors constructors;
extern struct Destructors destructors;
extern struct Methods methods;
extern struct Operators operators;

#endif

void load_api(GDExtensionInterfaceGetProcAddress p_get_proc_address);
void bind_api();
GDExtensionBool is_editor();

void ptrcall_0_args_no_ret(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret);
void ptrcall_0_args_ret_float(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret);
void ptrcall_1_float_arg_no_ret(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret);

void call_0_args_ret_float(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error);
void call_1_float_arg_no_ret(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error);


void bind_method_0(
    const char *class_name,
    const char *method_name,
    void *function);
    
// Version for 0 arguments, with return.
void bind_method_0_r(
    const char *class_name,
    const char *method_name,
    void *function,
    GDExtensionVariantType return_type);

// Version for 1 argument, no return.
void bind_method_1(
    const char *class_name,
    const char *method_name,
    void *function,
    const char *arg1_name,
    GDExtensionVariantType arg1_type);


// Create a PropertyInfo struct.
GDExtensionPropertyInfo make_property(
    GDExtensionVariantType type,
    const char *name);

GDExtensionPropertyInfo make_property_full(
    GDExtensionVariantType type,
    const char *name,
    uint32_t hint,
    const char *hint_string,
    const char *class_name,
    uint32_t usage_flags);

void destruct_property(GDExtensionPropertyInfo *info);

void bind_property(
    const char *class_name,
    const char *name,
    GDExtensionVariantType type,
    const char *getter,
    const char *setter);


bool is_string_name_equal(GDExtensionConstStringNamePtr p_a, const char *p_b);

GDExtensionMethodBindPtr get_method_bind(const char* class_name, const char* method_name, long hash);
void *classdb_get_class_tag(const char* class_name);
#endif /* C71A4301_2BAF_4AD7_8964_BFC167D79A0E */
