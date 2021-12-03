// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "xbasic/xenable_to_string.h"
#include "xbasic/xhashable.hpp"

#include <atomic>
#include <cassert>
#include <exception>
#include <functional>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>

NS_BEG1(top)

/**
 * @brief The id class.  std::uint64_t is the back.
 *
 */
class xtop_bad_id_access : public std::exception {
public:
    const char *
    what() const noexcept override {
        return "Bad xnullable_id_t access";
    }
};
using xbad_id_access_t = xtop_bad_id_access;

template <typename TagT, typename IdT, IdT MinValue, IdT MaxValue>
class xtop_simple_id final {
    XSTATIC_ASSERT(std::is_integral<IdT>::value);

public:
    using tag_type = TagT;
    using value_type = IdT;

private:
    value_type m_id{MaxValue};

public:
    constexpr xtop_simple_id() = default;
    xtop_simple_id(xtop_simple_id const &) = default;
    xtop_simple_id & operator=(xtop_simple_id const &) = default;
    xtop_simple_id(xtop_simple_id &&) = default;
    xtop_simple_id & operator=(xtop_simple_id &&) = default;
    ~xtop_simple_id() = default;

    constexpr explicit xtop_simple_id(value_type const raw_value) noexcept
        : m_id{ raw_value } {
    }

    void swap(xtop_simple_id & other) noexcept {
        std::swap(m_id, other.m_id);
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    bool operator==(xtop_simple_id const & other) const noexcept {
        return m_id == other.m_id;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    bool operator!=(xtop_simple_id const & other) const noexcept {
        return !(*this == other);
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    bool operator<(xtop_simple_id const & other) const noexcept {
        return m_id < other.m_id;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    bool operator>(xtop_simple_id const & other) const noexcept {
        return other < *this;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    bool operator<=(xtop_simple_id const & other) const noexcept {
        return !(other < *this);
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    bool operator>=(xtop_simple_id const & other) const noexcept {
        return !(*this < other);
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id operator++(int) {
        xtop_simple_id ret{ *this };
        operator++();
        return ret;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id & operator++() {
        ++m_id;
        return *this;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id operator--(int) {
        xtop_simple_id ret{ *this };
        operator--();
        return ret;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id & operator--() {
        --m_id;
        return *this;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    explicit operator value_type() const {
        return value();
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    value_type value() const {
        return m_id;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id & operator|=(xtop_simple_id const & other) noexcept {
        m_id |= other.m_id;
        return *this;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id operator|(xtop_simple_id const & other) const noexcept {
        auto ret = *this;
        return ret |= other;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id & operator&=(xtop_simple_id const & other) noexcept {
        m_id &= other.m_id;
        return *this;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    xtop_simple_id operator&(xtop_simple_id const & other) const noexcept {
        auto ret = *this;
        return ret &= other;
    }

#if defined XCXX14_OR_ABOVE
    constexpr
#endif
    uint64_t hash() const {
        return static_cast<uint64_t>(m_id);
    }

    std::string to_string() const {
        return std::to_string(m_id);
    }

    static constexpr xtop_simple_id max() noexcept {
        return xtop_simple_id{ MaxValue };
    }

    static constexpr xtop_simple_id min() noexcept {
        return xtop_simple_id{MinValue};
    }
};

template <typename TagT, typename IdT, IdT MinValue, IdT MaxValue>
using xsimple_id_t = xtop_simple_id<TagT, IdT, MinValue, MaxValue>;

NS_END1

NS_BEG1(std)

template <typename TagT, typename IdT, IdT MinValue, IdT MaxValue>
struct hash<top::xsimple_id_t<TagT, IdT, MinValue, MaxValue>> final
{
    std::size_t
    operator()(top::xsimple_id_t<TagT, IdT, MinValue, MaxValue> const & id) const noexcept {
        return static_cast<std::size_t>(id.hash());
    }
};

template <typename TagT, typename IdT, IdT MinValue, IdT MaxValue>
struct numeric_limits<top::xsimple_id_t<TagT, IdT, MinValue, MaxValue>> final
{
    static
    constexpr
    top::xsimple_id_t<TagT, IdT, MinValue, MaxValue>
    min() noexcept {
        return top::xsimple_id_t<TagT, IdT, MinValue, MaxValue>::min();
    }

    static
    constexpr
    top::xsimple_id_t<TagT, IdT, MinValue, MaxValue>
    max() noexcept {
        return top::xsimple_id_t<TagT, IdT, MinValue, MaxValue>::max();
    }
};

//template <typename TagT, typename IdT>
//struct hash<top::xnullable_id_t<TagT, IdT>> final
//{
//    std::size_t
//    operator()(top::xnullable_id_t<TagT, IdT> const & id) const noexcept {
//        return static_cast<std::size_t>(id.hash());
//    }
//};
//
//template <typename TagT, typename IdT>
//struct numeric_limits<top::xnullable_id_t<TagT, IdT>> final
//{
//    static
//    constexpr
//    top::xnullable_id_t<TagT, IdT>
//    min() noexcept {
//        return top::xnullable_id_t<TagT, IdT>::min();
//    }
//
//    static
//    constexpr
//    top::xnullable_id_t<TagT, IdT>
//    max() noexcept {
//        return top::xnullable_id_t<TagT, IdT>::max();
//    }
//};

NS_END1

template <typename TagT, typename IdT, IdT MinValue, IdT MaxValue>
std::ostream &
operator<<(std::ostream & o, top::xsimple_id_t<TagT, IdT, MinValue, MaxValue> const & id) {
    o << id.to_string();
    return o;
}

//template <typename TagT, typename IdT>
//std::ostream &
//operator<<(std::ostream & o, top::xnullable_id_t<TagT, IdT> const & id) {
//    o << id.to_string();
//    return o;
//}
