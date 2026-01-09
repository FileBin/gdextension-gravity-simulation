#ifndef C94C2966_B644_4B19_B0E3_CDE07BA11608
#define C94C2966_B644_4B19_B0E3_CDE07BA11608

#include "chunk_allocator.h"
#include "gdextension_interface.h"
#include "sys/types.h"
#include <stdbool.h>
#include <stdint.h>

// Struct to hold the node data.
typedef struct {
    // Metadata.
    GDExtensionObjectPtr object; // Stores the underlying Godot object.

    bool visit_tree;

    double G;

    // acceleration structure to not go through sub node every frame
    struct gravity_clusters {
        // memory allocation acceleration strucutre
        chunk_allocator memory;
        uint clusters_count;
        uint all_units_count;
        uint32_t clusters_list_offset;// array of GravitySimulationUnits grouped by RigidBody they belong to
    } gravity_clusters;

} GravitySimulation;

// Bindings.
void gravity_simulation_class_bind_methods();
GDExtensionObjectPtr gravity_simulation_class_create_instance(void *p_class_userdata);
void gravity_simulation_class_free_instance(void *p_class_userdata, GDExtensionClassInstancePtr p_instance);

void *gravity_simulation_class_get_virtual_with_data(void *p_class_userdata, GDExtensionConstStringNamePtr p_name);
void gravity_simulation_class_call_virtual_with_data(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, void *p_virtual_call_userdata, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret);

// Constructor for the node.
void gravity_simulation_class_constructor(GravitySimulation *self);

void gravity_simulation_class_set_G(GravitySimulation* self, double G);
double gravity_simulation_class_get_G(GravitySimulation* self);

// Destructor for the node.
void gravity_simulation_class_destructor(GravitySimulation *self);

// Methods
void gravity_simulation_class_ready(GravitySimulation *self);
void gravity_simulation_class_on_tree_changed(GravitySimulation *self);
void gravity_simulation_class_process(GravitySimulation *self, double delta);

#endif /* C94C2966_B644_4B19_B0E3_CDE07BA11608 */
