//
// Created by payto on 12/4/2021.
//

#include "xstate_accessor_c/xstate_accessor.h"

#include "xstate_accessor/xerror/xerror.h"
#include "xstate_accessor/xproperties/xproperty_type.h"
#include "xstate_accessor/xstate_accessor.h"

#include <cassert>

using namespace top;

bool empty(xdata_t const * data) noexcept {
    return data == nullptr || data->size == 0;
}

void free_property(xproperty_data_t * property) {
    if (property == nullptr) {
        return;
    }
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

xdata_t get_property_serialized(void * state_accessor_handle, char const * property_name, int * ec) {
    using namespace top::state_accessor;

    xdata_t ret;
    ret.ptr = nullptr;
    ret.size = 0;

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

    auto * state_accessor = reinterpret_cast<top::state_accessor::xstate_accessor_t *>(state_accessor_handle);
    state_accessor::properties::xtypeless_property_identifier_t property_id{property_name, state_accessor::properties::xproperty_category_t::user};
    std::error_code e;

    auto bytes = state_accessor->get_property<properties::xproperty_type_t::bytes>(property_id, e);
    if (e) {
        if (ec != nullptr) {
            *ec = e.value();
        }
        return ret;
    }

    ret.ptr = new xbyte_t[bytes.size()];
    std::memcpy(ret.ptr, bytes.data(), bytes.size());
    ret.size = bytes.size();

    return ret;
}

void set_property(void * state_accessor_handle, char const * property_name, xdata_t const * property_data, int * ec) {
    using namespace top::state_accessor;

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

    if (property_data == nullptr) {
        if (ec != nullptr) {
            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_value);
        }
        return;
    }

    auto * state_accessor = reinterpret_cast<top::state_accessor::xstate_accessor_t *>(state_accessor_handle);
    state_accessor::properties::xtypeless_property_identifier_t property_id{property_name, state_accessor::properties::xproperty_category_t::user};
    std::error_code e;

    xbytes_t bytes{ property_data->ptr, property_data->ptr + property_data->size };
    state_accessor->set_property<properties::xproperty_type_t::bytes>(property_id, bytes, e);
    if (e) {
        if (ec != nullptr) {
            *ec = e.value();
        }
        return;
    }
}


//void set_property_indexed_value(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, xproperty_pair_data_t const * property_data, int * ec) {
//    if (state_accessor_handle == nullptr) {
//        if (ec != nullptr) {
//            *ec = static_cast<int>(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle);
//        }
//        return;
//    }
//
//    if (property_name == nullptr) {
//        if (ec != nullptr) {
//            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_name);
//        }
//        return;
//    }
//
//    if (property_type != top::state_accessor::properties::xproperty_type_t::map && property_type != top::state_accessor::properties::xproperty_type_t::deque) {
//        if (ec != nullptr) {
//            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_type);
//        }
//        return;
//    }
//
//    if (property_data == nullptr || property_data->index == nullptr || property_data->index_type == top::state_accessor::properties::xproperty_type_t::invalid || property_data->value_type == top::state_accessor::properties::xproperty_type_t::invalid) {
//        if (ec != nullptr) {
//            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_value);
//        }
//        return;
//    }
//
//    if (property_type == top::state_accessor::properties::xproperty_type_t::deque && property_data->index_type != top::state_accessor::properties::xproperty_type_t::uint64) {
//        if (ec != nullptr) {
//            *ec = static_cast<int>(state_accessor::error::xerrc_t::invalid_property_value);
//        }
//    }
//
//    auto * state_accessor = reinterpret_cast<top::state_accessor::xstate_accessor_t *>(state_accessor_handle);
//    state_accessor::properties::xtypeless_property_identifier_t property_id{property_name, state_accessor::properties::xproperty_category_t::user};
//    switch (property_type) {
//    case top::state_accessor::properties::xproperty_type_t::map: {
//        std::error_code e;
//        std::string key{reinterpret_cast<char const*>(property_data->index)};
//        std::vector<uint8_t> value{};
//        state_accessor->set_property_cell_value<top::state_accessor::properties::xproperty_type_t::map>(property_id, std::string{}, *(reinterpret_cast<uint8_t const *>(property_data->value)), e);
//        break;
//    }
//
//    case top::state_accessor::properties::xproperty_type_t::deque: {
//        break;
//    }
//
//    default: {
//        assert(false);
//        break;
//    }
//    }
//}