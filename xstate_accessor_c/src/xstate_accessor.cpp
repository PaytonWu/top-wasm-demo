//
// Created by payto on 12/4/2021.
//

#include "xstate_accessor_c/xstate_accessor.h"

#include "xstate_accessor/xerror/xerror.h"
#include "xstate_accessor/xproperties/xproperty_type.h"
#include "xstate_accessor/xstate_accessor.h"

#include <cassert>

using namespace top;

void free_property(xproperty_object_t * property) {
    if (property == nullptr) {
        return;
    }

    auto * value = property->property_value;
    delete value;
}

uint64_t withdraw(void * state_accessor_handle, char const * property_name, uint64_t const amount, char const * symbol, int * ec) {
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
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_value);
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

void deposit(void * state_accessor_handle, char const * property_name, uint64_t amount, char const * symbol, int * ec) {
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
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_value);
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

void create_property(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, int * ec) {
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

    if (property_type == top::state_accessor::properties::xproperty_type_t::invalid) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_type);
        }

        return;
    }

    auto * state_accessor = reinterpret_cast<state_accessor::xstate_accessor_t *>(state_accessor_handle);
    state_accessor::properties::xproperty_identifier_t property_id{property_name, property_type, state_accessor::properties::xproperty_category_t::user};
    std::error_code e;

    state_accessor->create_property(property_id, e);
    if (e) {
        if (ec != nullptr) {
            *ec = e.value();
        }
    }
}

#define CASE(PROPERTY_OBJ, PROPERTY_ID, EC_PTR, TYPE) \
    case properties::xproperty_type_t::TYPE: {        \
        std::error_code e;                            \
        auto value = state_accessor->get_property<properties::xproperty_type_t::TYPE>(PROPERTY_ID, e); \
        if (e) {                                      \
            if (EC_PTR != nullptr) {                  \
                *EC_PTR = e.value();                  \
            }                                         \
            break;                                    \
        }                                             \
        assert(!e);                                   \
        PROPERTY_OBJ.property_type = properties::xproperty_type_t::TYPE;                               \
        PROPERTY_OBJ.property_value = new properties::xtype_of_t<properties::xproperty_type_t::TYPE>::type{std::move(value)}; \
        break;                                        \
    }

#define DEFAULT()       \
    default: {          \
        assert(false);  \
        break;          \
    }

xproperty_object_t get_property(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, int * ec) {
    xproperty_object_t ret;
    ret.property_type = state_accessor::properties::xproperty_type_t::invalid;
    ret.property_value = nullptr;

    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            *ec = static_cast<int>(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle);
        }

        return ret;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_name);
        }

        return ret;
    }

    if (property_type == top::state_accessor::properties::xproperty_type_t::invalid) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_type);
        }

        return ret;
    }

    auto * state_accessor = reinterpret_cast<top::state_accessor::xstate_accessor_t *>(state_accessor_handle);
    state_accessor::properties::xtypeless_property_identifier_t property_id{property_name, state_accessor::properties::xproperty_category_t::user};

    using namespace top::state_accessor;
    switch (property_type) {
        CASE(ret, property_id, ec, int64)
        CASE(ret, property_id, ec, uint64)
        CASE(ret, property_id, ec, string)
        CASE(ret, property_id, ec, bytes)
        CASE(ret, property_id, ec, map)

        DEFAULT()
    }

    return ret;
}

#undef DEFAULT
#undef CASE