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
    uint8_t * ptr;
    uint64_t size;
} xdata_t;

bool empty(xdata_t const * data) noexcept;

typedef struct {
    xdata_t property_value;
    top::state_accessor::properties::xproperty_type_t property_type;
} xproperty_data_t;

void free_property(xproperty_data_t * property);

typedef struct {
    xdata_t index;
    xdata_t value;
    top::state_accessor::properties::xproperty_type_t index_type;
    top::state_accessor::properties::xproperty_type_t value_type;
} xproperty_pair_data_t;

/// @brief Withdraw token.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the token property.
/// @param amount The amount to be withdrawn.
/// @param symbol The symbol of the token.
/// @param ec Error code.
/// @return The amount withdrawn.
uint64_t withdraw(void * state_accessor_handle, char const * property_name, uint64_t amount, char const * symbol, int * ec);

/// @brief Deposit token.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the token property.
/// @param amount The amount to be deposited.
/// @param symbol The symbol of the token.
/// @param ec Error code.
void deposit(void * state_accessor_handle, char const * property_name, uint64_t amount, char const * symbol, int * ec);

/// @brief Create the property.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the property to be created.
/// @param property_type Type of the property to be created.
/// @param ec Error code.
void create_property(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, int * ec);

/// @brief Get the property data in the serialized form.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the property to be queried.
/// @param ec Error code.
/// @return The property data.
xdata_t get_property_serialized(void * state_accessor_handle, char const * property_name, int * ec);

/// @brief Update property.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the property to be updated.
/// @param property_data Data to be set on the property.
/// @param ec Error code.
void set_property_serialized(void * state_accessor_handle, char const * property_name, xdata_t const * property_data, int * ec);

/// @brief Update value at specified index.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the property to be updated.
/// @param property_data Data to be set on the property.
/// @param ec Error code.
// void set_property_indexed_value(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, xproperty_pair_data_t const * property_data, int * ec);

#if defined(__cplusplus)
}
#endif

#endif //TOP_WASM_DEMO_XSTATE_ACCESSOR_H
