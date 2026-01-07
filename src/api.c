#define API_NO_EXTERN
#include "api.h"
#include "gdextension_interface.h"
#include "defs.h"

GDExtensionClassLibraryPtr class_library = NULL;

struct API api;
struct Constructors constructors;
struct Destructors destructors;
struct Methods methods;
struct Operators operators;

void load_api(GDExtensionInterfaceGetProcAddress p_get_proc_address)
{
    // Get helper functions first.
    GDExtensionInterfaceVariantGetPtrDestructor variant_get_ptr_destructor = (GDExtensionInterfaceVariantGetPtrDestructor)p_get_proc_address("variant_get_ptr_destructor");
    GDExtensionInterfaceVariantGetPtrOperatorEvaluator variant_get_ptr_operator_evaluator = (GDExtensionInterfaceVariantGetPtrOperatorEvaluator)p_get_proc_address("variant_get_ptr_operator_evaluator");

    // API.
    api.classdb_register_extension_class2 = (GDExtensionInterfaceClassdbRegisterExtensionClass2)p_get_proc_address("classdb_register_extension_class2");
    api.classdb_register_extension_class_method = p_get_proc_address("classdb_register_extension_class_method");
    api.classdb_register_extension_class_property = p_get_proc_address("classdb_register_extension_class_property");
    api.classdb_get_method_bind = (GDExtensionInterfaceClassdbGetMethodBind)p_get_proc_address("classdb_get_method_bind");
    api.classdb_get_class_tag = (GDExtensionInterfaceClassdbGetClassTag)p_get_proc_address("classdb_get_class_tag");
    api.object_method_bind_call = (GDExtensionInterfaceObjectMethodBindCall)p_get_proc_address("object_method_bind_call");
    api.global_get_singleton = (GDExtensionInterfaceGlobalGetSingleton)p_get_proc_address("global_get_singleton");
    api.classdb_construct_object = (GDExtensionInterfaceClassdbConstructObject)p_get_proc_address("classdb_construct_object");
    api.get_variant_from_type_constructor = (GDExtensionInterfaceGetVariantFromTypeConstructor)p_get_proc_address("get_variant_from_type_constructor");
    api.get_variant_to_type_constructor = (GDExtensionInterfaceGetVariantToTypeConstructor)p_get_proc_address("get_variant_to_type_constructor");
    
    api.variant_get_type = (GDExtensionInterfaceVariantGetType)p_get_proc_address("variant_get_type");
    api.variant_get_object_instance_id = (GDExtensionInterfaceVariantGetObjectInstanceId)p_get_proc_address("variant_get_object_instance_id");

    api.variant_iter_init = (GDExtensionInterfaceVariantIterInit)p_get_proc_address("variant_iter_init");
    api.variant_iter_next = (GDExtensionInterfaceVariantIterNext)p_get_proc_address("variant_iter_next");
    api.variant_iter_get = (GDExtensionInterfaceVariantIterGet)p_get_proc_address("variant_iter_get");

    api.object_set_instance = (GDExtensionInterfaceObjectSetInstance)p_get_proc_address("object_set_instance");
    api.object_set_instance_binding = (GDExtensionInterfaceObjectSetInstanceBinding)p_get_proc_address("object_set_instance_binding");
    api.object_get_instance_binding = (GDExtensionInterfaceObjectGetInstanceBinding)p_get_proc_address("object_get_instance_binding");
    api.object_cast_to = (GDExtensionInterfaceObjectCastTo)p_get_proc_address("object_cast_to");
    api.object_get_instance_from_id = (GDExtensionInterfaceObjectGetInstanceFromId)p_get_proc_address("object_get_instance_from_id");
    api.mem_alloc = (GDExtensionInterfaceMemAlloc)p_get_proc_address("mem_alloc");
    api.mem_free = (GDExtensionInterfaceMemFree)p_get_proc_address("mem_free");

    // Constructors.
    constructors.string_name_new_with_latin1_chars = (GDExtensionInterfaceStringNameNewWithLatin1Chars)p_get_proc_address("string_name_new_with_latin1_chars");
    
    constructors.variant_from_string_name_constructor = (GDExtensionVariantFromTypeConstructorFunc)api.get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);
    constructors.variant_from_object_constructor = (GDExtensionVariantFromTypeConstructorFunc)api.get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_OBJECT);
    constructors.variant_from_vector2_constructor = (GDExtensionVariantFromTypeConstructorFunc)api.get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_VECTOR2);
    constructors.variant_from_float_constructor = (GDExtensionVariantFromTypeConstructorFunc)api.get_variant_from_type_constructor(GDEXTENSION_VARIANT_TYPE_FLOAT);

    constructors.float_from_variant_constructor = (GDExtensionTypeFromVariantConstructorFunc)api.get_variant_to_type_constructor(GDEXTENSION_VARIANT_TYPE_FLOAT);
    constructors.vector2_from_variant_constructor = (GDExtensionTypeFromVariantConstructorFunc)api.get_variant_to_type_constructor(GDEXTENSION_VARIANT_TYPE_VECTOR2);
    constructors.bool_from_variant_constructor = (GDExtensionTypeFromVariantConstructorFunc)api.get_variant_to_type_constructor(GDEXTENSION_VARIANT_TYPE_BOOL);
    
    constructors.string_new_with_utf8_chars = (GDExtensionInterfaceStringNewWithUtf8Chars)p_get_proc_address("string_new_with_utf8_chars");
    constructors.variant_construct = (GDExtensionInterfaceVariantConstruct)p_get_proc_address("variant_construct");
    
    // Destructors.
    destructors.string_name_destructor = variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);
    destructors.string_destructor = variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING);
    destructors.variant_destroy = p_get_proc_address("variant_destroy");
    
    // Operators
    operators.string_name_equal = variant_get_ptr_operator_evaluator(GDEXTENSION_VARIANT_OP_EQUAL, GDEXTENSION_VARIANT_TYPE_STRING_NAME, GDEXTENSION_VARIANT_TYPE_STRING_NAME);


    StringName engine_name;
    constructors.string_name_new_with_latin1_chars(&engine_name, "Engine", false);
    api.engine = api.global_get_singleton(&engine_name);
    destructors.string_name_destructor(&engine_name);
}

void bind_api() {
    methods.engine_is_editor_hint = get_method_bind("Engine", "is_editor_hint", 36873697);
    methods.object_connect = get_method_bind("Object", "connect", 1518946055);
    methods.node2d_get_position = get_method_bind("Node2D", "get_position", 3341600327);
    methods.node2d_get_global_position = get_method_bind("Node2D", "get_global_position", 3341600327);
    methods.node_get_parent = get_method_bind("Node", "get_parent", 3160264692);
    methods.node_get_children = get_method_bind("Node", "get_children", 873284517);
    methods.node_get_tree = get_method_bind("Node", "get_tree", 2958820483);
    methods.rigidbody2d_apply_force = get_method_bind("RigidBody2D", "apply_force", 4288681949);
}

GDExtensionBool is_editor() {
    GDExtensionBool result;
    Variant ret;

    GDExtensionConstVariantPtr args[] = {};
    api.object_method_bind_call(methods.engine_is_editor_hint, api.engine, args,
                                0, &ret, NULL);
    
    constructors.bool_from_variant_constructor(&result, &ret);
    return result;
}

void ptrcall_0_args_ret_float(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret)
{
    // Call the function.
    double (*function)(void *) = method_userdata;
    *((double *)r_ret) = function(p_instance);
}

void ptrcall_0_args_no_ret(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret)
{
    // Call the function.
    double (*function)(void *) = method_userdata;
    function(p_instance);
}

void ptrcall_1_float_arg_no_ret(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret)
{
    // Call the function.
    void (*function)(void *, double) = method_userdata;
    function(p_instance, *((double *)p_args[0]));
}

void call_0_args_ret_float(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error)
{
    // Check argument count.
    if (p_argument_count != 0)
    {
        r_error->error = GDEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS;
        r_error->expected = 0;
        return;
    }

    // Call the function.
    double (*function)(void *) = method_userdata;
    double result = function(p_instance);
    // Set resulting Variant.
    constructors.variant_from_float_constructor(r_return, &result);
}


void call_0_args_no_ret(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error)
{
    // Check argument count.
    if (p_argument_count > 0)
    {
        r_error->error = GDEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS;
        r_error->expected = 1;
        return;
    }

    // Call the function.
    void (*function)(GDExtensionClassInstancePtr) = method_userdata;
    function(p_instance);
}

void call_1_float_arg_no_ret(void *method_userdata, GDExtensionClassInstancePtr p_instance, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error)
{
    // Check argument count.
    if (p_argument_count < 1)
    {
        r_error->error = GDEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS;
        r_error->expected = 1;
        return;
    }
    else if (p_argument_count > 1)
    {
        r_error->error = GDEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS;
        r_error->expected = 1;
        return;
    }

    // Check the argument type.
    GDExtensionVariantType type = api.variant_get_type(p_args[0]);
    if (type != GDEXTENSION_VARIANT_TYPE_FLOAT)
    {
        r_error->error = GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT;
        r_error->expected = GDEXTENSION_VARIANT_TYPE_FLOAT;
        r_error->argument = 0;
        return;
    }

    // Extract the argument.
    double arg1;
    constructors.float_from_variant_constructor(&arg1, (GDExtensionVariantPtr)p_args[0]);

    // Call the function.
    void (*function)(void *, double) = method_userdata;
    function(p_instance, arg1);
}

// Version for 0 arguments, with return.
void bind_method_0_r(
    const char *class_name,
    const char *method_name,
    void *function,
    GDExtensionVariantType return_type)
{
    StringName method_name_string;
    constructors.string_name_new_with_latin1_chars(&method_name_string, method_name, false);

    GDExtensionClassMethodCall call_func = call_0_args_ret_float;
    GDExtensionClassMethodPtrCall ptrcall_func = ptrcall_0_args_ret_float;

    GDExtensionPropertyInfo return_info = make_property(return_type, "");

    GDExtensionClassMethodInfo method_info = {
        .name = &method_name_string,
        .method_userdata = function,
        .call_func = call_func,
        .ptrcall_func = ptrcall_func,
        .method_flags = GDEXTENSION_METHOD_FLAGS_DEFAULT,
        .has_return_value = true,
        .return_value_info = &return_info,
        .return_value_metadata = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE,
        .argument_count = 0,
    };

    StringName class_name_string;
    constructors.string_name_new_with_latin1_chars(&class_name_string, class_name, false);

    api.classdb_register_extension_class_method(class_library, &class_name_string, &method_info);

    // Destruct things.
    destructors.string_name_destructor(&method_name_string);
    destructors.string_name_destructor(&class_name_string);
    destruct_property(&return_info);
}


// Version for 0 argument, no return.
void bind_method_0(
    const char *class_name,
    const char *method_name,
    void *function)
{

    StringName method_name_string;
    constructors.string_name_new_with_latin1_chars(&method_name_string, method_name, false);

    GDExtensionClassMethodCall call_func = call_0_args_no_ret;
    GDExtensionClassMethodPtrCall ptrcall_func = ptrcall_0_args_no_ret;

    GDExtensionPropertyInfo args_info[] = {};
    GDExtensionClassMethodArgumentMetadata args_metadata[] = {
        GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE,
    };

    GDExtensionClassMethodInfo method_info = {
        .name = &method_name_string,
        .method_userdata = function,
        .call_func = call_func,
        .ptrcall_func = ptrcall_func,
        .method_flags = GDEXTENSION_METHOD_FLAGS_DEFAULT,
        .has_return_value = false,
        .argument_count = 0,
        .arguments_info = args_info,
        .arguments_metadata = args_metadata,
    };

    StringName class_name_string;
    constructors.string_name_new_with_latin1_chars(&class_name_string, class_name, false);

    api.classdb_register_extension_class_method(class_library, &class_name_string, &method_info);

    // Destruct things.
    destructors.string_name_destructor(&method_name_string);
    destructors.string_name_destructor(&class_name_string);
}

// Version for 1 argument, no return.
void bind_method_1(
    const char *class_name,
    const char *method_name,
    void *function,
    const char *arg1_name,
    GDExtensionVariantType arg1_type)
{

    StringName method_name_string;
    constructors.string_name_new_with_latin1_chars(&method_name_string, method_name, false);

    GDExtensionClassMethodCall call_func = call_1_float_arg_no_ret;
    GDExtensionClassMethodPtrCall ptrcall_func = ptrcall_1_float_arg_no_ret;

    GDExtensionPropertyInfo args_info[] = {
        make_property(arg1_type, arg1_name),
    };
    GDExtensionClassMethodArgumentMetadata args_metadata[] = {
        GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE,
    };

    GDExtensionClassMethodInfo method_info = {
        .name = &method_name_string,
        .method_userdata = function,
        .call_func = call_func,
        .ptrcall_func = ptrcall_func,
        .method_flags = GDEXTENSION_METHOD_FLAGS_DEFAULT,
        .has_return_value = false,
        .argument_count = 1,
        .arguments_info = args_info,
        .arguments_metadata = args_metadata,
    };

    StringName class_name_string;
    constructors.string_name_new_with_latin1_chars(&class_name_string, class_name, false);

    api.classdb_register_extension_class_method(class_library, &class_name_string, &method_info);

    // Destruct things.
    destructors.string_name_destructor(&method_name_string);
    destructors.string_name_destructor(&class_name_string);
    destruct_property(&args_info[0]);
}

GDExtensionPropertyInfo make_property(
    GDExtensionVariantType type,
    const char *name)
{

    return make_property_full(type, name, PROPERTY_HINT_NONE, "", "", PROPERTY_USAGE_DEFAULT);
}

GDExtensionPropertyInfo make_property_full(
    GDExtensionVariantType type,
    const char *name,
    uint32_t hint,
    const char *hint_string,
    const char *class_name,
    uint32_t usage_flags)
{

    StringName *prop_name = api.mem_alloc(sizeof(StringName));
    constructors.string_name_new_with_latin1_chars(prop_name, name, false);
    String *prop_hint_string = api.mem_alloc(sizeof(String));
    constructors.string_new_with_utf8_chars(prop_hint_string, hint_string);
    StringName *prop_class_name = api.mem_alloc(sizeof(StringName));
    constructors.string_name_new_with_latin1_chars(prop_class_name, class_name, false);

    GDExtensionPropertyInfo info = {
        .name = prop_name,
        .type = type,
        .hint = hint,
        .hint_string = prop_hint_string,
        .class_name = prop_class_name,
        .usage = usage_flags,
    };

    return info;
}

void destruct_property(GDExtensionPropertyInfo *info)
{
    destructors.string_name_destructor(info->name);
    destructors.string_destructor(info->hint_string);
    destructors.string_name_destructor(info->class_name);
    api.mem_free(info->name);
    api.mem_free(info->hint_string);
    api.mem_free(info->class_name);
}

void bind_property(
    const char *class_name,
    const char *name,
    GDExtensionVariantType type,
    const char *getter,
    const char *setter)
{
    StringName class_string_name;
    constructors.string_name_new_with_latin1_chars(&class_string_name, class_name, false);
    GDExtensionPropertyInfo info = make_property(type, name);
    StringName getter_name;
    constructors.string_name_new_with_latin1_chars(&getter_name, getter, false);
    StringName setter_name;
    constructors.string_name_new_with_latin1_chars(&setter_name, setter, false);

    api.classdb_register_extension_class_property(class_library, &class_string_name, &info, &setter_name, &getter_name);

    // Destruct things.
    destructors.string_name_destructor(&class_string_name);
    destruct_property(&info);
    destructors.string_name_destructor(&getter_name);
    destructors.string_name_destructor(&setter_name);
}

bool is_string_name_equal(GDExtensionConstStringNamePtr p_a, const char *p_b)
{
    // Create a StringName for the C string.
    StringName string_name;
    constructors.string_name_new_with_latin1_chars(&string_name, p_b, false);

    // Compare both StringNames.
    bool is_equal = false;
    operators.string_name_equal(p_a, &string_name, &is_equal);

    // Destroy the created StringName.
    destructors.string_name_destructor(&string_name);

    // Return the result.
    return is_equal;
}

GDExtensionMethodBindPtr get_method_bind(const char* class, const char* method, long hash) {
    StringName class_name, method_name;
    GDExtensionMethodBindPtr result;
    constructors.string_name_new_with_latin1_chars(&class_name, class, false);
    constructors.string_name_new_with_latin1_chars(&method_name, method, false);

    result = api.classdb_get_method_bind(&class_name, &method_name, hash);

    destructors.string_name_destructor(&class_name);
    destructors.string_name_destructor(&method_name);

    return result;
}

void *classdb_get_class_tag(const char* class) {
    StringName class_name;
    void *result;
    constructors.string_name_new_with_latin1_chars(&class_name, class, false);

    result = api.classdb_get_class_tag(&class_name);

    destructors.string_name_destructor(&class_name);

    return result;
}