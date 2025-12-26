#ifndef B2358859_E34B_4F6F_914C_BC78C2EF1274
#define B2358859_E34B_4F6F_914C_BC78C2EF1274

#include "defs.h"

#include "gdextension_interface.h"

void initialize_gdexample_module(void *p_userdata, GDExtensionInitializationLevel p_level);
void deinitialize_gdexample_module(void *p_userdata, GDExtensionInitializationLevel p_level);
GDExtensionBool GDE_EXPORT gdexample_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization);


#endif /* B2358859_E34B_4F6F_914C_BC78C2EF1274 */
