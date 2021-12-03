// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "xbasic/xutility.h"
#include "xcommon/xaddress.h"
#include "xdata/xblock.h"

#include <cstdint>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>

namespace top {
namespace data {

/// @brief Stores data read from tableblock object directly
struct xtop_block_data {
    uint32_t transaction_count{0};
    uint32_t block_count{0};

    std::string to_json_string() const;

    template <typename JsonObjectT>
    JsonObjectT to_json_object() const {
        JsonObjectT json_object;

        return json_object;
    }
};
using xblock_data_t = xtop_block_data;

/// @brief Stores consensus execution related data.
struct xtop_consensus_vote_data : public xserializable_based_on<void> {
    uint32_t block_count{0};
    uint32_t vote_count{0};

    std::string to_json_string() const;

    template <typename JsonObjectT>
    JsonObjectT to_json_object() const {
        JsonObjectT json_object;


        return json_object;
    }
};
using xconsensus_vote_data_t = xtop_consensus_vote_data;

/// @brief Account related statistics data.
struct xtop_account_related_statistics_data {
    xblock_data_t block_data;

    std::string to_json_string() const;

    template <typename JsonObjectT>
    JsonObjectT to_json_object() const {
        JsonObjectT json_object;

        return json_object;
    }
};
using xaccount_related_statistics_data_t = xtop_account_related_statistics_data;

/// @brief Statistics data hold by a group.
struct xtop_group_releated_statistics_data : public xserializable_based_on<void> {
    /// @brief Index is the account slot id.
    std::vector<xaccount_related_statistics_data_t> account_statistics_data;

    std::string to_json_string() const;

    template <typename JsonObjectT>
    JsonObjectT to_json_object() const {
        JsonObjectT json_object;

        return json_object;
    }
};
using xgroup_related_statistics_data_t = xtop_group_releated_statistics_data;

/// @brief Statistics data hold by all groups in one election round.
struct xtop_election_related_statistics_data : public xserializable_based_on<void> {
    /// @brief Statistics data grouped by the group id;
    std::map<common::xgroup_address_t, xgroup_related_statistics_data_t> group_statistics_data;

    std::string to_json_string() const;

    template <typename JsonObjectT>
    JsonObjectT to_json_object() const {
        JsonObjectT json_object;

        return json_object;
    }
};
using xelection_related_statistics_data_t = xtop_election_related_statistics_data;

/// @brief Statistics data grouped by election chain block height.
struct xtop_statistics_data : public xserializable_based_on<void> {
    std::map<uint64_t, xelection_related_statistics_data_t> detail;

    std::string to_json_string() const;

    template <typename JsonObjectT>
    JsonObjectT to_json_object() const {
        JsonObjectT json_object;

        return json_object;
    }
};
using xstatistics_data_t = xtop_statistics_data;

}  // namespace data
}  // namespace top
