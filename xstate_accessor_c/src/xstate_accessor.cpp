//
// Created by payto on 12/4/2021.
//

#include "xstate_accessor_c/xstate_accessor.h"

#include "xstate_accessor/xerror/xerror.h"
#include "xstate_accessor/xproperties/xproperty_type.h"
#include "xstate_accessor/xstate_accessor.h"

#include <cassert>
#include <cstring>

using namespace top;

bool empty(xbytes_data_t const * data) noexcept {
    return data == nullptr || data->size == 0;
}

void free_bytes(xbytes_data_t * data) {
    if (data == nullptr) {
        return;
    }

    delete[] data;
}

template <typename ToErrorCodeT, typename FromErrorCodeT = std::error_code>
void convert_to(FromErrorCodeT const & from, ToErrorCodeT & to);

template <>
void convert_to(std::error_code const & from, xerror_code_t & to) {
    assert(from);
    assert(to.value == 0);

    to.value = from.value();
    to.category = from.category().name();

    to.message = new char[from.message().size() + 1];
    auto * ptr = const_cast<char *>(to.message);
    std::memcpy(ptr, from.message().data(), from.message().size());
    ptr[from.message().size()] = '\0';
}

void free_error_code(xerror_code_t * ec) {
    if (ec == nullptr) {
        return;
    }

    ec->category = nullptr;

    delete[] ec->message;
    ec->message = nullptr;
}

uint64_t withdraw(void * state_accessor_handle, char const * property_name, uint64_t const amount, char const * symbol, xerror_code_t * ec) {
    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle), *ec);
        }

        return 0;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_name), *ec);
        }

        return 0;
    }

    if (amount == 0) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_value), *ec);
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
            convert_to(e, *ec);
        }

        return 0;
    }

    auto ret = token.amount();
    token.clear();
    return ret;
}

void deposit(void * state_accessor_handle, char const * property_name, uint64_t amount, char const * symbol, xerror_code_t * ec) {
    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle), *ec);
        }

        return;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_name), *ec);
        }

        return;
    }

    if (amount == 0) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_value), *ec);
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
            convert_to(e, *ec);
        }

        return;
    }
}

void create_property(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, xerror_code_t * ec) {
    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle), *ec);
        }

        return;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_name), *ec);
        }

        return;
    }

    if (property_type == top::state_accessor::properties::xproperty_type_t::invalid) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_type), *ec);
        }

        return;
    }

    auto * state_accessor = reinterpret_cast<state_accessor::xstate_accessor_t *>(state_accessor_handle);
    state_accessor::properties::xproperty_identifier_t property_id{property_name, property_type, state_accessor::properties::xproperty_category_t::user};
    std::error_code e;

    state_accessor->create_property(property_id, e);
    if (e) {
        if (ec != nullptr) {
            convert_to(e, *ec);
        }
    }
}

#define DEFINE_GET_INT_PROPERTY(INT) \
INT##_t get_property_##INT(void * state_accessor_handle, char const * property_name, xerror_code_t * ec) { \
    using namespace top::state_accessor;                                                                   \
    INT##_t ret{0};                  \
                                     \
    if (state_accessor_handle == nullptr) {                                                                \
        if (ec != nullptr) {         \
            convert_to(make_error_code(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle), *ec); \
        }                            \
        return ret;                  \
    }                                \
                                     \
    if (property_name == nullptr) {  \
        if (ec != nullptr) {         \
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_name), *ec);       \
        }                            \
        return ret;                  \
    }                                \
                                     \
    auto * state_accessor = reinterpret_cast<top::state_accessor::xstate_accessor_t *>(state_accessor_handle);    \
    state_accessor::properties::xtypeless_property_identifier_t property_id{property_name, state_accessor::properties::xproperty_category_t::user}; \
    std::error_code e;               \
                                     \
    ret = state_accessor->get_property<properties::xproperty_type_t::INT>(property_id, e);                 \
    if (e) {                         \
        if (ec != nullptr) {         \
            convert_to(e, *ec);      \
        }                            \
        return ret;                  \
    }                                \
                                     \
    return ret;                      \
}

DEFINE_GET_INT_PROPERTY(int64)
DEFINE_GET_INT_PROPERTY(uint64)

#undef DEFINE_GET_INT_PROPERTY

xbytes_data_t get_property_bytes(void * state_accessor_handle, char const * property_name, xerror_code_t * ec) {
    using namespace top::state_accessor;

    xbytes_data_t ret;
    ret.ptr = nullptr;
    ret.size = 0;

    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle), *ec);
        }
        return ret;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_name), *ec);
        }
        return ret;
    }

    auto * state_accessor = reinterpret_cast<top::state_accessor::xstate_accessor_t *>(state_accessor_handle);
    state_accessor::properties::xtypeless_property_identifier_t property_id{property_name, state_accessor::properties::xproperty_category_t::user};
    std::error_code e;

    auto bytes = state_accessor->get_property<properties::xproperty_type_t::bytes>(property_id, e);
    if (e) {
        if (ec != nullptr) {
            convert_to(e, *ec);
        }
        return ret;
    }

    ret.ptr = new xbyte_t[bytes.size()];
    std::memcpy(ret.ptr, bytes.data(), bytes.size());
    ret.size = bytes.size();

    return ret;
}

void set_property_bytes(void * state_accessor_handle, char const * property_name, xbytes_data_t const * property_data, xerror_code_t * ec) {
    using namespace top::state_accessor;

    if (state_accessor_handle == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(top::state_accessor::error::xerrc_t::invalid_state_accessor_handle), *ec);
        }
        return;
    }

    if (property_name == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_name), *ec);
        }
        return;
    }

    if (property_data == nullptr) {
        if (ec != nullptr) {
            convert_to(make_error_code(state_accessor::error::xerrc_t::invalid_property_value), *ec);
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
            convert_to(e, *ec);
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