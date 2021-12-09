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

uint64_t withdraw(void * const state_accessor_handle, char const * const property_name, uint64_t const amount, char const * const symbol, int * ec);
void deposit(void * const state_accessor_handle, char const * const property_name, uint64_t amount, char const * const symbol, int * ec);

void create_property(char const * const property_name, top::state_accessor::properties::xproperty_type_t property_type, int * ec);

#if defined(__cplusplus)
}
#endif

#endif //TOP_WASM_DEMO_XSTATE_ACCESSOR_H
