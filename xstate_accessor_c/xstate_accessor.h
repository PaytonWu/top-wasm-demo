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
} xbytes_data_t;


bool empty(xbytes_data_t const * data) noexcept;

typedef struct {
    xbytes_data_t property_value;
    top::state_accessor::properties::xproperty_type_t property_type;
} xproperty_data_t;

void free_bytes(xbytes_data_t * data);

typedef struct {
    xbytes_data_t index;
    xbytes_data_t value;
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

#define DECLARE_GET_INT_PROPERTY(INT) \
INT##_t get_property_##INT(void * state_accessor_handle, char const * property_name, int * ec)

DECLARE_GET_INT_PROPERTY(uint8);
DECLARE_GET_INT_PROPERTY(uint16);
DECLARE_GET_INT_PROPERTY(uint32);
DECLARE_GET_INT_PROPERTY(uint64);
DECLARE_GET_INT_PROPERTY(int8);
DECLARE_GET_INT_PROPERTY(int16);
DECLARE_GET_INT_PROPERTY(int32);
DECLARE_GET_INT_PROPERTY(int64);

#undef DECLARE_GET_INT_PROPERTY

/// @brief Get the property data in the serialized form.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the property to be queried.
/// @param ec Error code.
/// @return The property data.
xbytes_data_t get_property_bytes(void * state_accessor_handle, char const * property_name, int * ec);

/// @brief Update property.
/// @param state_accessor_handle The pointer to the state accessor instance.
/// @param property_name Name of the property to be updated.
/// @param property_data Data to be set on the property.
/// @param ec Error code.
void set_property_bytes(void * state_accessor_handle, char const * property_name, xbytes_data_t const * property_data, int * ec);

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
