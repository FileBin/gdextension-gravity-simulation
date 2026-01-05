#ifndef DFA6CF0F_50F8_453D_9AFF_D64DB11F716D
#define DFA6CF0F_50F8_453D_9AFF_D64DB11F716D
#include "defs.h"
#include "gdextension_interface.h"

// Struct to hold the node data.
typedef struct {
    // Metadata.
    GDExtensionObjectPtr node2d; // Stores the underlying Godot object.
    real mass;
} GravitySimulationUnit;

GDExtensionObjectPtr gravity_simulation_unit_class_create_instance(void *p_class_userdata);
void gravity_simulation_unit_class_free_instance(void *p_class_userdata, GDExtensionClassInstancePtr p_instance);

void gravity_simulation_unit_class_constructor(GravitySimulationUnit *self);
void gravity_simulation_unit_class_destructor(GravitySimulationUnit *self);

void gravity_simulation_unit_class_bind_methods();

void *gravity_simulation_unit_class_get_virtual_with_data(void *p_class_userdata, GDExtensionConstStringNamePtr p_name);
void gravity_simulation_unit_class_call_virtual_with_data(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, void *p_virtual_call_userdata, const GDExtensionConstTypePtr *p_args, GDExtensionTypePtr r_ret);

void gravity_simulation_unit_class_set_mass(GravitySimulationUnit* self, double mass);
double gravity_simulation_unit_class_get_mass(GravitySimulationUnit* self);

#endif /* DFA6CF0F_50F8_453D_9AFF_D64DB11F716D */
