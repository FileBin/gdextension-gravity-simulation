#ifndef C94C2966_B644_4B19_B0E3_CDE07BA11608
#define C94C2966_B644_4B19_B0E3_CDE07BA11608

#include "gdextension_interface.h"

// Struct to hold the node data.
typedef struct
{
    // Metadata.
    GDExtensionObjectPtr object; // Stores the underlying Godot object.
} GravitySimulation;

// Bindings.
void gravity_simulation_class_bind_methods();
GDExtensionObjectPtr gravity_simulation_class_create_instance(void *p_class_userdata);
void gravity_simulation_class_free_instance(void *p_class_userdata, GDExtensionClassInstancePtr p_instance);

// Constructor for the node.
void gravity_simulation_class_constructor(GravitySimulation *self);

// Destructor for the node.
void gravity_simulation_class_destructor(GravitySimulation *self);

// Bindings.
void gravity_simulation_class_bind_methods();


#endif /* C94C2966_B644_4B19_B0E3_CDE07BA11608 */
