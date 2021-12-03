// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "xbasic/xns_macro.h"

#include <cassert>
#include <cstdint>
#include <string>
#include <utility>

NS_BEG1(top)

enum class xenum_crypto_key_type : std::uint8_t { invalid = 0, pub_key = 1, pri_key = 2 };
using xcrypto_key_type_t = xenum_crypto_key_type;

XINLINE_CONSTEXPR xcrypto_key_type_t pub = xcrypto_key_type_t::pub_key;
XINLINE_CONSTEXPR xcrypto_key_type_t pri = xcrypto_key_type_t::pri_key;

template <xcrypto_key_type_t KeyT>
class xtop_crypto_key;

template <xcrypto_key_type_t KeyT>
class xtop_crypto_key final {
private:
    std::string m_key{};

public:
    xtop_crypto_key() = default;
    xtop_crypto_key(xtop_crypto_key const &) = default;
    xtop_crypto_key & operator=(xtop_crypto_key const &) = default;
    xtop_crypto_key(xtop_crypto_key &&) = default;
    xtop_crypto_key & operator=(xtop_crypto_key &&) = default;
    ~xtop_crypto_key() = default;

    explicit xtop_crypto_key(std::string raw_key) noexcept : m_key{std::move(raw_key)} {}

    void swap(xtop_crypto_key & other) { std::swap(m_key, other.m_key); }

    bool empty() const noexcept { return m_key.empty(); }

    std::string to_string() const { return m_key; }

    bool operator==(xtop_crypto_key const & other) const noexcept { return m_key == other.m_key; }

    bool operator!=(xtop_crypto_key const & other) const noexcept { return !(*this == other); }
};

template <xcrypto_key_type_t KeyT>
using xcrypto_key_t = xtop_crypto_key<KeyT>;

using xpublic_key_t = xcrypto_key_t<pub>;   // base64 presentation
using xprivate_key_t = xcrypto_key_t<pri>;

NS_END1
