//
// Created by payto on 12/4/2021.
//

#ifndef TOP_WASM_DEMO_XSTATE_ACCESSOR_H
#define TOP_WASM_DEMO_XSTATE_ACCESSOR_H

#include "xstate_accessor/xproperties/xproperty_type.h"

#include <cstdint>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    void * property_value;
    top::state_accessor::properties::xproperty_type_t property_type;
} xproperty_object_t;

void free_property(xproperty_object_t * property);

uint64_t withdraw(void * state_accessor_handle, char const * property_name, uint64_t amount, char const * symbol, int * ec);
void deposit(void * state_accessor_handle, char const * property_name, uint64_t amount, char const * symbol, int * ec);

void create_property(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, int * ec);

xproperty_object_t get_property(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, int * ec);

#if defined(__cplusplus)
}
#endif

#endif //TOP_WASM_DEMO_XSTATE_ACCESSOR_H
