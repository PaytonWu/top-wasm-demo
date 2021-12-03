// Copyright (c) 2017-2021 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <inttypes.h>

#include "xdata/xtransaction_v2.h"
#include "xbase/xmem.h"
#include "xbase/xutl.h"
#include "xdata/xaction_parse.h"
#include "xcrypto/xckey.h"
#include "xdata/xdata_defines.h"
#include "xdata/xdata_error.h"
#include "xbasic/xmodule_type.h"
#include "xbase/xcontext.h"

#include <iostream>

namespace top { namespace data {

using namespace top::base;

xtransaction_v2_t::xtransaction_v2_t() {
}

xtransaction_v2_t::~xtransaction_v2_t() {
}

void xtransaction_v2_t::construct_tx(enum_xtransaction_type tx_type, const uint16_t expire_duration, const uint32_t deposit, const uint32_t nonce, const std::string & memo, const xtx_action_info & info) {
    set_tx_type(tx_type);
    set_expire_duration(expire_duration);
    set_deposit(deposit);
    set_premium_price(0);
    set_last_nonce(nonce);
    set_fire_timestamp(get_gmttime_s());

    set_source(info.m_source_addr, info.m_source_action_name, info.m_source_action_para);
    set_target(info.m_target_addr, info.m_target_action_name, info.m_target_action_para);
}

int32_t xtransaction_v2_t::do_write_without_hash_signature(base::xstream_t & out) const {
    const int32_t begin_pos = out.size();
    out.write_compact_var(static_cast<uint16_t>(m_transaction_type));
    out.write_compact_var(m_expire_duration);
    out.write_compact_var(m_fire_timestamp);

    std::string compact_source_account = xvaccount_t::compact_address_to(m_source_addr);
    out << compact_source_account;
    std::string compact_target_account = xvaccount_t::compact_address_to(m_target_addr);
    out << compact_target_account;
    out.write_compact_var(m_edge_nodeid);

    out.write_compact_var(m_amount);
    out.write_compact_var(m_token_name);
    out.write_compact_var(m_last_trans_nonce);
    out.write_compact_var(m_deposit);
    out.write_compact_var(m_premium_price);
    out.write_compact_var(m_memo);
    out.write_compact_var(m_ext);

    if(m_transaction_type != xtransaction_type_transfer) {
        out.write_compact_var(m_source_action_name);
        out.write_compact_var(m_source_action_para);
        out.write_compact_var(m_target_action_name);
        out.write_compact_var(m_target_action_para);
    }

    const int32_t end_pos = out.size();
    return (end_pos - begin_pos);
}

int32_t xtransaction_v2_t::do_uncompact_write_without_hash_signature(base::xstream_t & stream) const {
    const int32_t begin_pos = stream.size();
    stream << static_cast<uint16_t>(m_transaction_type);
    stream << m_expire_duration;
    stream << m_fire_timestamp;
    stream << m_source_addr;
    stream << m_target_addr;
    stream << m_edge_nodeid;

    stream << m_amount;
    stream << m_token_name;
    stream << m_last_trans_nonce;
    stream << m_deposit;
    stream << m_premium_price;
    stream << m_memo;
    stream << m_ext;

    if(m_transaction_type != xtransaction_type_transfer) {
        stream << m_source_action_name;
        stream << m_source_action_para;
        stream << m_target_action_name;
        stream << m_target_action_para;
    }

    const int32_t end_pos = stream.size();
    return (end_pos - begin_pos);
}

int32_t xtransaction_v2_t::do_read_without_hash_signature(base::xstream_t & in) {
    const int32_t begin_pos = in.size();
    uint16_t tmp_transaction_type;
    in.read_compact_var(tmp_transaction_type);
    m_transaction_type = static_cast<enum_xtransaction_type>(tmp_transaction_type);
    in.read_compact_var(m_expire_duration);
    in.read_compact_var(m_fire_timestamp);
    
    std::string compact_source_account;
    in >> compact_source_account;
    m_source_addr = xvaccount_t::compact_address_from(compact_source_account);
    std::string compact_target_account;
    in >> compact_target_account;
    m_target_addr = xvaccount_t::compact_address_from(compact_target_account);
    in.read_compact_var(m_edge_nodeid);
    
    in.read_compact_var(m_amount);
    in.read_compact_var(m_token_name);
    in.read_compact_var(m_last_trans_nonce);
    in.read_compact_var(m_deposit);
    in.read_compact_var(m_premium_price);
    in.read_compact_var(m_memo);
    in.read_compact_var(m_ext);
        
    if(m_transaction_type != xtransaction_type_transfer) {
        in.read_compact_var(m_source_action_name);
        in.read_compact_var(m_source_action_para);
        in.read_compact_var(m_target_action_name);
        in.read_compact_var(m_target_action_para);
    }

    const int32_t end_pos = in.size();
    return (begin_pos - end_pos);
}

int xtransaction_v2_t::do_write(base::xstream_t & out) {
    const int32_t begin_pos = out.size();
    do_write_without_hash_signature(out);
    out.write_compact_var(m_authorization);
    const int32_t end_pos = out.size();
    return (end_pos - begin_pos);
}

int xtransaction_v2_t::do_read(base::xstream_t & in) {
    return 0;
}

#ifdef XENABLE_PSTACK  // tracking memory
int32_t xtransaction_v2_t::add_ref() {
    MEMCHECK_ADD_TRACE(this, "tx_add");
    return base::xdataunit_t::add_ref();
}
int32_t xtransaction_v2_t::release_ref() {
    MEMCHECK_ADD_TRACE(this, "tx_release");
    xdbgassert(get_refcount() > 0);
    return base::xdataunit_t::release_ref();
}
#endif

void xtransaction_v2_t::adjust_target_address(uint32_t table_id) {
}

void xtransaction_v2_t::set_digest() {
    if (m_transaction_hash == uint256_t()) {
        base::xstream_t stream(base::xcontext_t::instance());
        do_uncompact_write_without_hash_signature(stream);
        m_transaction_hash = utl::xsha2_256_t::digest((const char*)stream.data(), stream.size());
    }
}

bool xtransaction_v2_t::transaction_len_check() const {
    if (get_memo().size() > MAX_TRANSACTION_MEMO_SIZE) {
        xwarn("xtransaction_v2_t::transaction_len_check memo size too long.size:%d", get_memo().size());
        return false;
    }

    base::xstream_t stream(base::xcontext_t::instance());
    const int32_t begin_pos = stream.size();
    do_write_without_hash_signature(stream);
    stream.write_compact_var(m_authorization);

    uint16_t len = stream.size() - begin_pos;
    if (len != get_tx_len()) {
        xwarn("xtransaction_v2_t::transaction_len_check tx length not match. %d %d", len, get_tx_len());
        return false;
    }
    return true;
}

int32_t xtransaction_v2_t::make_tx_create_user_account(const std::string & addr) {
    set_tx_type(xtransaction_type_create_user_account);
    m_target_addr = addr;
    return xsuccess;
}

int32_t xtransaction_v2_t::make_tx_transfer(const data::xproperty_asset & asset) {
    set_tx_type(xtransaction_type_transfer);
    set_amount(asset.m_amount);
    return xsuccess;
}

int32_t xtransaction_v2_t::make_tx_run_contract(const data::xproperty_asset & asset_out, const std::string& function_name, const std::string& para) {
    set_tx_type(xtransaction_type_run_contract);

    xstream_t stream(xcontext_t::instance());
    stream << asset_out.m_token_name;
    stream << asset_out.m_amount;
    m_source_action_para = std::string((char *)stream.data(), stream.size());

    m_target_action_name = function_name;
    m_target_action_para = para;
    return xsuccess;
}

int32_t xtransaction_v2_t::make_tx_run_contract(std::string const & function_name, std::string const & param) {
    data::xproperty_asset asset_out{0};
    return make_tx_run_contract(asset_out, function_name, param);
}

int32_t xtransaction_v2_t::set_different_source_target_address(const std::string & src_addr, const std::string & dst_addr) {
    m_source_addr = src_addr;
    m_target_addr = dst_addr;
    return xsuccess;
}

int32_t xtransaction_v2_t::set_same_source_target_address(const std::string & addr) {
    return set_different_source_target_address(addr, addr);
}

void xtransaction_v2_t::set_last_trans_hash_and_nonce(uint256_t last_hash, uint64_t last_nonce) {
    set_last_nonce(last_nonce);
}

void xtransaction_v2_t::set_source(const std::string & addr, const std::string & action_name, const std::string & para) {
    m_source_addr = addr;
    m_source_action_name = action_name;
    m_source_action_para = para;
}

void xtransaction_v2_t::set_target(const std::string & addr, const std::string & action_name, const std::string & para) {
    m_target_addr = addr;
    m_target_action_name = action_name;
    m_target_action_para = para;
}

void xtransaction_v2_t::set_len() {
    base::xstream_t stream(base::xcontext_t::instance());
    const int32_t begin_pos = stream.size();
    do_write_without_hash_signature(stream);
    stream.write_compact_var(m_authorization);
    set_tx_len(stream.size() - begin_pos);
}

bool xtransaction_v2_t::digest_check() const {
    return true;
}

std::string xtransaction_v2_t::get_digest_hex_str() const {
    if (m_transaction_hash_str.empty()) {
        m_transaction_hash_str = to_hex_str(m_transaction_hash);
    }
    return m_transaction_hash_str;
}

bool xtransaction_v2_t::check_last_nonce(uint64_t account_nonce) {
    return (get_last_nonce() == account_nonce);
}

void xtransaction_v2_t::set_fire_and_expire_time(uint16_t expire_duration) {
    struct timeval val;
    base::xtime_utl::gettimeofday(&val);
    set_fire_timestamp((uint64_t)val.tv_sec);
    set_expire_duration(expire_duration);
}

bool xtransaction_v2_t::sign_check() const {
    return false;
}

bool xtransaction_v2_t::pub_key_sign_check(xpublic_key_t const & pub_key) const {
    return false;
}

size_t xtransaction_v2_t::get_serialize_size() const {
    base::xstream_t stream(base::xcontext_t::instance());
    xassert(stream.size() == 0);
    const_cast<xtransaction_v2_t*>(this)->serialize_to(stream);
    return stream.size();
}

std::string xtransaction_v2_t::dump() const {
    char local_param_buf[256];
    xprintf(local_param_buf,    sizeof(local_param_buf),
    "{transaction:hash=%s,type=%u,from=%s,to=%s,nonce=%" PRIu64 ",refcount=%d,this=%p}",
    get_digest_hex_str().c_str(), (uint32_t)get_tx_type(), get_source_addr().c_str(), get_target_addr().c_str(),
    get_tx_nonce(), get_refcount(), this);
    return std::string(local_param_buf);
}

xaction_t & xtransaction_v2_t::get_source_action() { 
    m_source_action.set_account_addr(m_source_addr); 
    m_source_action.set_action_name(m_source_action_name);

    auto param = m_source_action_para;
    if (get_tx_type() == xtransaction_type_transfer) {
        data::xproperty_asset asset_out{m_amount};
        data::xaction_t action;
        xaction_asset_out::serialze_to(action, asset_out);
        param = action.get_action_param();
    }

    m_source_action.set_action_param(param);
    set_action_type();
    return m_source_action;
}

xaction_t & xtransaction_v2_t::get_target_action() { 
    m_target_action.set_account_addr(m_target_addr); 
    m_target_action.set_action_name(m_target_action_name); 

    auto param = m_target_action_para;
    if (get_tx_type() == xtransaction_type_transfer) {
        data::xproperty_asset asset_out{m_amount};
        data::xaction_t action;
        xaction_asset_out::serialze_to(action, asset_out);
        param = action.get_action_param();
    }

    m_target_action.set_action_param(param);
    set_action_type();
    return m_target_action;
}

void xtransaction_v2_t::set_action_type() {
    xtransaction_t::set_action_type_by_tx_type(m_source_action, m_target_action, m_transaction_type);
}

int32_t xtransaction_v2_t::parse(enum_xaction_type source_type, enum_xaction_type target_type, xtx_parse_data_t & tx_parse_data) {
#ifdef ENABLE_CREATE_USER  // debug use
    if (target_type == xaction_type_create_user_account) {
        tx_parse_data.m_new_account = m_source_addr;
    }
#endif

    if (source_type == xaction_type_asset_out) {
        std::string token_name = m_token_name;
        uint64_t amount = m_amount;
        if (get_tx_type() != xtransaction_type_transfer) {
            try {
                if (m_source_action_para.empty()) {
                    return xchain_error_action_param_empty;
                }
                xstream_t stream(xcontext_t::instance(), (uint8_t *)m_source_action_para.data(), m_source_action_para.size());
                stream >> token_name;
                stream >> amount;
            } catch (...) {
                return xchain_error_action_param_empty;
            }
        }
        tx_parse_data.m_asset = xproperty_asset(amount);
    }

    if (target_type == xaction_type_run_contract) {
        tx_parse_data.m_function_name = m_target_action_name;
        tx_parse_data.m_function_para = m_target_action_para;
    }

    try {
        if (target_type == xaction_type_pledge_token) {
            if (m_target_action_para.empty()) {
                return xchain_error_action_param_empty;
            }
            base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)m_target_action_para.data(), m_target_action_para.size());
            stream >> tx_parse_data.m_asset.m_token_name;
            stream >> tx_parse_data.m_asset.m_amount;
        }

        if (target_type == xaction_type_redeem_token) {
            if (m_target_action_para.empty()) {
                return xchain_error_action_param_empty;
            }
            base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)m_target_action_para.data(), m_target_action_para.size());
            stream >> tx_parse_data.m_asset.m_token_name;
            stream >> tx_parse_data.m_asset.m_amount;
        }

        if (target_type == xaction_type_pledge_token_vote) {
            if (m_target_action_para.empty()) {
                return xchain_error_action_param_empty;
            }
            base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)m_target_action_para.data(), m_target_action_para.size());
            stream >> tx_parse_data.m_vote_num;
            stream >> tx_parse_data.m_lock_duration;
        }

        if (target_type == xaction_type_redeem_token_vote) {
            if (m_target_action_para.empty()) {
                return xchain_error_action_param_empty;
            }
            base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)m_target_action_para.data(), m_target_action_para.size());
            stream >> tx_parse_data.m_vote_num;
        }
    } catch (...) {
        return xchain_error_action_param_empty;
    }

    return 0;
}

}  // namespace data
}  // namespace top
