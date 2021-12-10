// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "xcommon/xip.h"

#include "xbase/xlog.h"

#include <cassert>
#include <iomanip>
#include <ios>
#include <sstream>
#include <utility>

NS_BEG2(top, common)

static void reset_raw_xip2_address_domain(xvip2_t & xip2) noexcept {
    reset_address_domain_to_xip2(xip2);
#if !defined NDEBUG
    auto const domain = static_cast<xaddress_domain_t>(get_address_domain_from_xip2(xip2));
    assert(domain == xaddress_domain_t::enum_xaddress_domain_xip);
#endif
}

static void reset_raw_xip2_network_type(xvip2_t & xip2) noexcept {
    reset_network_type_to_xip2(xip2);
#if !defined NDEBUG
    auto const network_type = static_cast<xnetwork_type_t>(get_network_type_from_xip2(xip2));
    assert(network_type == xnetwork_type_t::enum_xnetwork_type_any);
#endif
}

static void reset_raw_xip2_network_id(xvip2_t & xip2) noexcept {
    reset_network_id_to_xip2(xip2);
#if !defined NDEBUG
    auto const network_id = static_cast<xnetwork_id_t::value_type>(get_network_id_from_xip2(xip2));
    assert(network_id == 0);
#endif
}

static void reset_raw_xip2_zone_id(xvip2_t & xip2) noexcept {
    reset_zone_id_to_xip2(xip2);
#if !defined NDEBUG
    auto const zone_id = static_cast<xzone_id_t::value_type>(get_zone_id_from_xip2(xip2));
    assert(zone_id == 0);
#endif
}

static void reset_raw_xip2_cluster_id(xvip2_t & xip2) noexcept {
    reset_cluster_id_to_xip2(xip2);
#if !defined NDEBUG
    auto const cluster_id = static_cast<xcluster_id_t::value_type>(get_cluster_id_from_xip2(xip2));
    assert(cluster_id == 0);
#endif
}

static void set_raw_xip2_address_domain(xvip2_t & xip2, xaddress_domain_t const domain) noexcept {
    reset_raw_xip2_address_domain(xip2);
    set_address_domain_to_xip2(xip2, domain);
#if !defined NDEBUG
    auto const value = static_cast<xaddress_domain_t>(get_address_domain_from_xip2(xip2));
    assert(value == domain);
#endif
}

static void set_raw_xip2_network_type(xvip2_t & xip2, xnetwork_type_t const type) noexcept {
    reset_raw_xip2_network_type(xip2);
    set_network_type_to_xip2(xip2, type);
}

static void set_raw_xip2_network_id(xvip2_t & xip2, xnetwork_id_t::value_type const nid) noexcept {
    reset_raw_xip2_network_id(xip2);
    set_network_id_to_xip2(xip2, nid);
#if !defined NDEBUG
    auto const network_id = static_cast<xnetwork_id_t::value_type>(get_network_id_from_xip2(xip2));
    assert(nid == network_id);
#endif
}

static void set_raw_xip2_zone_id(xvip2_t & xip2, xzone_id_t::value_type const zid) noexcept {
    reset_raw_xip2_zone_id(xip2);
    set_zone_id_to_xip2(xip2, zid);
#if !defined NDEBUG
    auto const zone_id = static_cast<xzone_id_t::value_type>(get_zone_id_from_xip2(xip2));
    assert(zid == zone_id);
#endif
}

static void set_raw_xip2_cluster_id(xvip2_t & xip2, xcluster_id_t::value_type const cid) noexcept {
    reset_raw_xip2_cluster_id(xip2);
    set_cluster_id_to_xip2(xip2, cid);
#if !defined NDEBUG
    auto const cluster_id = static_cast<xcluster_id_t::value_type>(get_cluster_id_from_xip2(xip2));
    assert(cid == cluster_id);
#endif
}

xtop_ip::xtop_ip(xnetwork_id_t const & network_id, xzone_id_t const & zone_id, xcluster_id_t const & cluster_id, xaddress_domain_t const domain) noexcept {
    // assert(network_id.has_value());
    // assert(zone_id.has_value());
    // assert(cluster_id.has_value());

    xvip2_t xip2{m_xip, 0};
    set_raw_xip2_address_domain(xip2, domain);
    set_raw_xip2_network_type(xip2, xnetwork_type_t::enum_xnetwork_type_xchain);
    set_raw_xip2_network_id(xip2, network_id.value());
    set_raw_xip2_zone_id(xip2, zone_id.value());
    set_raw_xip2_cluster_id(xip2, cluster_id.value());
    m_xip = xip2.low_addr;

    assert(static_cast<xnetwork_id_t::value_type>(get_network_id_from_xip2(xip2)) == (network_id.value() & xbroadcast_network_id_value));
    assert(static_cast<xzone_id_t::value_type>(get_zone_id_from_xip2(xip2)) == (zone_id.value() & xbroadcast_zone_id_value));
    assert(static_cast<xcluster_id_t::value_type>(get_cluster_id_from_xip2(xip2)) == (cluster_id.value() & xbroadcast_cluster_id_value));
}

xaddress_domain_t xtop_ip::address_domain() const noexcept {
    xvip2_t xip2{m_xip, 0};
    return static_cast<xaddress_domain_t>(get_address_domain_from_xip2(xip2));
}

bool xtop_ip::empty() const noexcept {
    return m_xip == 0 || (broadcast(network_id()) && broadcast(zone_id()) && broadcast(cluster_id()) && broadcast(group_id()));
}

void xtop_ip::swap(xtop_ip & other) noexcept {
    std::swap(m_xip, other.m_xip);
}

bool xtop_ip::operator==(xtop_ip const & other) const noexcept {
    return m_xip == other.m_xip;
}

bool xtop_ip::operator<(xtop_ip const & other) const noexcept {
    return m_xip < other.m_xip;
}

bool xtop_ip::operator!=(xtop_ip const & other) const noexcept {
    return !(*this == other);
}

common::xip_type_t xtop_ip::type() const noexcept {
    xvip2_t tmp_xip{m_xip, 0};
    return static_cast<common::xip_type_t>(get_xip_type_from_xip2(tmp_xip));
}

common::xnetwork_type_t xtop_ip::network_type() const noexcept {
    xvip2_t tmp_xip{m_xip, 0};
    return static_cast<common::xnetwork_type_t>(get_network_type_from_xip2(tmp_xip));
}

xnetwork_version_t xtop_ip::network_version() const noexcept {
    // xvip2_t tmp_xip{m_xip, 0};
    // auto ret = xnetwork_version_t{static_cast<xnetwork_version_t::value_type>(get_network_ver_from_xip2(tmp_xip))};
    // assert(ret == xdefault_network_version);
    // return ret;
    return xdefault_network_version;
}

xnetwork_id_t xtop_ip::network_id() const noexcept {
    xvip2_t tmp_xip{m_xip, 0};
    return xnetwork_id_t{static_cast<xnetwork_id_t::value_type>(get_network_id_from_xip2(tmp_xip))};
}

xzone_id_t xtop_ip::zone_id() const noexcept {
    xvip2_t tmp_xip{m_xip, 0};
    return xzone_id_t{static_cast<xzone_id_t::value_type>(get_zone_id_from_xip2(tmp_xip))};
}

xcluster_id_t xtop_ip::cluster_id() const noexcept {
    xvip2_t tmp_xip{m_xip, 0};
    return xcluster_id_t{static_cast<xcluster_id_t::value_type>(get_cluster_id_from_xip2(tmp_xip))};
}

xgroup_id_t xtop_ip::group_id() const noexcept {
    xvip2_t tmp_xip{m_xip, 0};
    return xgroup_id_t{static_cast<xgroup_id_t::value_type>(get_group_id_from_xip2(tmp_xip))};
}

xslot_id_t xtop_ip::slot_id() const noexcept {
    xvip2_t tmp_xip{m_xip, 0};
    return xslot_id_t{static_cast<xslot_id_t::value_type>(get_node_id_from_xip2(tmp_xip))};
}

std::string xtop_ip::to_string() const {
    std::ostringstream oss;
    oss << std::right << std::setfill('0') << std::setw(sizeof(m_xip) * 2) << std::hex << m_xip;
    return oss.str();
}

xtop_ip::value_type xtop_ip::value() const noexcept {
    return m_xip;
}

xtop_ip xtop_ip::group_xip() const noexcept {
    return {network_id(), zone_id(), cluster_id(), group_id()};
}

std::string to_string(xnetwork_version_t const network_version) {
    return std::to_string(static_cast<std::uint16_t>(network_version.value()));
}

std::string to_string(xnetwork_id_t const & nid) {
    return std::to_string(nid.value());
}

std::string to_string(xzone_id_t const & zid) {
    return std::to_string(static_cast<std::uint16_t>(zid.value()));
}

std::string to_string(xcluster_id_t const & cid) {
    return std::to_string(static_cast<std::uint16_t>(cid.value()));
}

std::string to_string(xgroup_id_t const & gid) {
    return std::to_string(static_cast<std::uint16_t>(gid.value()));
}

std::string to_string(xslot_id_t const & sid) {
    return std::to_string(static_cast<std::uint16_t>(sid.value()));
}

xtop_ip::operator xtop_ip::value_type() const noexcept {
    return m_xip;
}

std::size_t xtop_ip::hash() const {
    return m_xip;
}

NS_END2

NS_BEG1(top)

xtop_extended<common::xip_t>::xtop_extended(xvip2_t const & xvip2) noexcept : m_xip{xvip2.low_addr}, m_extent{xvip2.high_addr} {
}

xtop_extended<common::xip_t>::xtop_extended(common::xip_t const & xip, common::xaddress_domain_t const domain) noexcept : m_xip{xip, domain} {
}

void xtop_extended<common::xip_t>::swap(xtop_extended<common::xip_t> & other) noexcept {
    m_xip.swap(other.m_xip);
    std::swap(m_extent, other.m_extent);
}

bool xtop_extended<common::xip_t>::operator==(xtop_extended const & other) const noexcept {
    return m_xip == other.m_xip && m_extent == other.m_extent;
}

bool xtop_extended<common::xip_t>::operator!=(xtop_extended const & other) const noexcept {
    return !(*this == other);
}

common::xaddress_domain_t xtop_extended<common::xip_t>::address_domain() const noexcept {
    assert(common::xaddress_domain_t::enum_xaddress_domain_xip2 == m_xip.address_domain());
    return m_xip.address_domain();
}

common::xip_type_t xtop_extended<common::xip_t>::type() const noexcept {
    return m_xip.type();
}

common::xnetwork_type_t xtop_extended<common::xip_t>::network_type() const noexcept {
    assert(m_xip.network_type() == common::xnetwork_type_t::enum_xnetwork_type_xchain);
    return m_xip.network_type();
}

common::xnetwork_id_t xtop_extended<common::xip_t>::network_id() const noexcept {
    return m_xip.network_id();
}

common::xzone_id_t xtop_extended<common::xip_t>::zone_id() const noexcept {
    return m_xip.zone_id();
}

common::xcluster_id_t xtop_extended<common::xip_t>::cluster_id() const noexcept {
    return m_xip.cluster_id();
}

common::xgroup_id_t xtop_extended<common::xip_t>::group_id() const noexcept {
    return m_xip.group_id();
}

common::xslot_id_t xtop_extended<common::xip_t>::slot_id() const noexcept {
    return m_xip.slot_id();
}

uint16_t xtop_extended<common::xip_t>::size() const noexcept {
    xvip2_t xip2{0, m_extent};
    return static_cast<uint16_t>(get_group_nodes_count_from_xip2(xip2));
}

uint64_t xtop_extended<common::xip_t>::height() const noexcept {
    xvip2_t xip2{0, m_extent};
    return static_cast<uint64_t>(get_network_height_from_xip2(xip2));
}

common::xip_t xtop_extended<common::xip_t>::xip() const noexcept {
    return m_xip;
}

std::size_t xtop_extended<common::xip_t>::hash() const {
    return m_xip.hash() ^ m_extent;
}

std::string xtop_extended<common::xip_t>::to_string() const {
    std::ostringstream oss;
    oss << m_xip.to_string() << ".";
    oss << std::right << std::setfill('0') << std::setw(sizeof(m_extent) * 2) << std::hex << m_extent;
    return oss.str();
}

xvip2_t xtop_extended<common::xip_t>::value() const noexcept {
    return xvip2_t{m_xip.value(), m_extent};
}

xtop_extended<common::xip_t>::operator xvip2_t() const noexcept {
    return value();
}

NS_END1

NS_BEG2(top, common)

xnode_type_t node_type_from(common::xzone_id_t const & zone_id) {
    if (zone_id == xcommittee_zone_id) {
        return xnode_type_t::committee;
    }

    if (zone_id == xconsensus_zone_id) {
        return xnode_type_t::consensus;
    }

    if (zone_id == xarchive_zone_id) {
        return xnode_type_t::storage;
    }

    if (zone_id == xedge_zone_id) {
        return xnode_type_t::edge;
    }

    if (zone_id == xzec_zone_id) {
        return xnode_type_t::zec;
    }

    if (zone_id == xfrozen_zone_id) {
        return xnode_type_t::frozen;
    }

    return xnode_type_t::invalid;
}

xnode_type_t node_type_from(common::xzone_id_t const & zone_id, xcluster_id_t const & /* cluster_id */) {
    return node_type_from(zone_id);
}

xnode_type_t node_type_from(common::xzone_id_t const & zone_id, common::xcluster_id_t const & /* cluster_id */, xgroup_id_t const & group_id) {
    auto node_type = node_type_from(zone_id);
    if (node_type == xnode_type_t::consensus) {
        if (xauditor_group_id_begin <= group_id && group_id < xauditor_group_id_end) {
            node_type |= xnode_type_t::auditor;
        } else if (xvalidator_group_id_begin <= group_id && group_id < xvalidator_group_id_end) {
            node_type |= xnode_type_t::validator;
        } else {
            assert(false);
            node_type = xnode_type_t::invalid;
        }
    } else if (node_type == xnode_type_t::storage) {
        if (xarchive_group_id == group_id) {
            node_type |= xnode_type_t::archive;
        } else if (xfull_node_group_id == group_id) {
            node_type |= xnode_type_t::full_node;
        } else {
            assert(false);
            node_type = xnode_type_t::invalid;
        }
    }

    return node_type;
}

template <>
bool broadcast<xnetwork_id_t>(xnetwork_id_t const & network_id) noexcept {
    return (network_id & xbroadcast_id_t::network) == xbroadcast_id_t::network;
}

template <>
bool broadcast<xzone_id_t>(xzone_id_t const & zone_id) noexcept {
    return (zone_id & xbroadcast_id_t::zone) == xbroadcast_id_t::zone;
}

template <>
bool broadcast<xcluster_id_t>(xcluster_id_t const & cluster_id) noexcept {
    return (cluster_id & xbroadcast_id_t::cluster) == xbroadcast_id_t::cluster;
}

template <>
bool broadcast<xgroup_id_t>(xgroup_id_t const & group_id) noexcept {
    return (group_id & xbroadcast_id_t::group) == xbroadcast_id_t::group;
}

template <>
bool broadcast<xslot_id_t>(xslot_id_t const & slot_id) noexcept {
    return (slot_id & xbroadcast_id_t::slot) == xbroadcast_id_t::slot;
}

template <>
bool broadcast<uint64_t>(uint64_t const & associated_blk_height) noexcept {
    return (associated_blk_height & xbroadcast_associated_blk_height) == xbroadcast_associated_blk_height;
}

#if !defined(XCXX14_OR_ABOVE)

xnetwork_id_t const xtop_broadcast_id::network{xbroadcast_network_id_value};
xzone_id_t const xtop_broadcast_id::zone{xbroadcast_zone_id_value};
xcluster_id_t const xtop_broadcast_id::cluster{xbroadcast_cluster_id_value};
xgroup_id_t const xtop_broadcast_id::group{xbroadcast_group_id_value};
xslot_id_t const xtop_broadcast_id::slot{xbroadcast_slot_id_value};

#endif

NS_END2
