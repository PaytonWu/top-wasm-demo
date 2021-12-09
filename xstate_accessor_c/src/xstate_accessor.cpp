//
// Created by payto on 12/4/2021.
//

#include "xstate_accessor_c/xstate_accessor.h"

#include "xstate_accessor/xstate_accessor.h"
#include "xstate_accessor/xerror/xerror.h"

#include <cassert>

using namespace top;

uint64_t withdraw(void * const state_accessor_handle, char const * const property_name, uint64_t const amount, char const * const symbol, int * ec) {
    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            *ec = static_cast<int>(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle);
        }

        return 0;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_name);
        }

        return 0;
    }

    if (amount == 0) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::property_value_out_of_range);
        }

        return 0;
    }

    assert(state_accessor_handle != nullptr);
    auto * state_accessor = reinterpret_cast<state_accessor::xstate_accessor_t *>(state_accessor_handle);

    std::error_code e;
    state_accessor::properties::xproperty_identifier_t property_id{property_name, state_accessor::properties::xproperty_type_t::token, state_accessor::properties::xproperty_category_t::system};
    auto token = state_accessor->withdraw(property_id, symbol == nullptr ? common::xsymbol_t{} : common::xsymbol_t{symbol}, amount, e);
    if (e) {
        assert(token.invalid());
        if (ec != nullptr) {
            *ec = e.value();
        }

        return 0;
    }

    auto ret = token.amount();
    token.clear();
    return ret;
}

void deposit(void * const state_accessor_handle, char const * const property_name, uint64_t const amount, char const * const symbol, int * ec) {
    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            *ec = static_cast<int>(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle);
        }

        return;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_name);
        }

        return;
    }

    if (amount == 0) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::property_value_out_of_range);
        }

        return;
    }

    assert(state_accessor_handle != nullptr);
    auto * state_accessor = reinterpret_cast<state_accessor::xstate_accessor_t *>(state_accessor_handle);

    std::error_code e;
    state_accessor::properties::xproperty_identifier_t property_id{property_name, state_accessor::properties::xproperty_type_t::token, state_accessor::properties::xproperty_category_t::system};
    state_accessor->deposit(property_id, state_accessor::xtoken_t{ amount, symbol == nullptr ? common::xsymbol_t{} : common::xsymbol_t{symbol} }, e);
    if (e) {
        if (ec != nullptr) {
            *ec = e.value();
        }

        return;
    }
}