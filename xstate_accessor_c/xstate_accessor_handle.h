//
// Created by payto on 12/4/2021.
//

#ifndef TOP_WASM_DEMO_XSTATE_ACCESSOR_HANDLE_H
#define TOP_WASM_DEMO_XSTATE_ACCESSOR_HANDLE_H

#include <cstdint>

#if defined(__cplusplus)
extern "C" {
#endif

struct xtop_state_accessor_handle {
    void * const unused_handle;
};
using xstate_accessor_handle_t = xtop_state_accessor_handle *;

uint64_t withdraw(xstate_accessor_handle_t const state_accessor_handle, char const * const property_name, uint64_t const amount, char const * const symbol, int * ec);

#if defined(__cplusplus)
}
#endif

#endif //TOP_WASM_DEMO_XSTATE_ACCESSOR_HANDLE_H
