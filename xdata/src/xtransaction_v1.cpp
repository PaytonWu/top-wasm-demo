// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "xbase/xutl.h"
#include "xbase/xcontext.h"
#include "xutility/xhash.h"
#include "xcrypto/xckey.h"

#include "xdata/xdata_error.h"
#include "xdata/xaction_parse.h"
#include "xdata/xtransaction_v1.h"
#include "xdata/xdata_defines.h"

#include <cinttypes>
namespace top { namespace data {

int32_t xtransaction_v1_t::serialize_write(base::xstream_t & stream, bool is_write_without_len) const {
    const int32_t begin_pos = stream.size();
    stream << m_transaction_type;
    if (is_write_without_len) {
        uint16_t transaction_len = 0;
        stream << transaction_len;
    } else {
        stream << m_transaction_len;
    }
    stream << m_version;
    stream << m_to_ledger_id;
    stream << m_from_ledger_id;
    stream << m_deposit;
    stream << m_expire_duration;
    stream << m_fire_timestamp;
    stream << m_trans_random_nonce;
    stream << m_premium_price;
    stream << m_last_trans_nonce;
    stream << m_last_trans_hash;
    stream << m_challenge_proof;
    stream << m_ext;
    stream << m_memo;
    const int32_t end_pos = stream.size();
    return (end_pos - begin_pos);
}
int32_t xtransaction_v1_t::serialize_read(base::xstream_t & stream) {
    const int32_t begin_pos = stream.size();
    stream >> m_transaction_type;
    stream >> m_transaction_len;
    stream >> m_version;
    stream >> m_to_ledger_id;
    stream >> m_from_ledger_id;
    stream >> m_deposit;
    stream >> m_expire_duration;
    stream >> m_fire_timestamp;
    stream >> m_trans_random_nonce;
    stream >> m_premium_price;
    stream >> m_last_trans_nonce;
    stream >> m_last_trans_hash;
    stream >> m_challenge_proof;
    stream >> m_ext;
    stream >> m_memo;
    const int32_t end_pos = stream.size();
    return (begin_pos - end_pos);
}

xtransaction_v1_t::xtransaction_v1_t() {
}

xtransaction_v1_t::~xtransaction_v1_t() {
}

void xtransaction_v1_t::construct_tx(enum_xtransaction_type tx_type, const uint16_t expire_duration, const uint32_t deposit, const uint32_t nonce, const std::string & memo, const xtx_action_info & info) {
    set_tx_type(tx_type);
    set_expire_duration(expire_duration);
    set_deposit(deposit);
    set_premium_price(0);
    set_last_nonce(nonce);
    set_fire_timestamp(get_gmttime_s());
    set_from_ledger_id(0);
    set_to_ledger_id(0);

    m_source_action.set_account_addr(info.m_source_addr);
    m_source_action.set_action_name(info.m_source_action_name);
    m_source_action.set_action_param(info.m_source_action_para);

    m_target_action.set_account_addr(info.m_target_addr);
    m_target_action.set_action_name(info.m_target_action_name);
    m_target_action.set_action_param(info.m_target_action_para);

    xtransaction_t::set_action_type_by_tx_type(m_source_action, m_target_action, static_cast<enum_xtransaction_type>(m_transaction_type));
}

int32_t xtransaction_v1_t::do_write_without_hash_signature(base::xstream_t & stream, bool is_write_without_len) const {
    const int32_t begin_pos = stream.size();
    serialize_write(stream, is_write_without_len);
    m_source_action.serialize_write(stream, is_write_without_len);
    m_target_action.serialize_write(stream, is_write_without_len);
    const int32_t end_pos = stream.size();
    return (end_pos - begin_pos);
}

int32_t xtransaction_v1_t::do_read_without_hash_signature(base::xstream_t & stream) {
    const int32_t begin_pos = stream.size();
    serialize_read(stream);
    m_source_action.serialize_read(stream);
    m_target_action.serialize_read(stream);
    const int32_t end_pos = stream.size();
    return (begin_pos - end_pos);
}

int32_t xtransaction_v1_t::do_write(base::xstream_t & stream) {
    const int32_t begin_pos = stream.size();
    do_write_without_hash_signature(stream, false);
    stream << m_transaction_hash;
    stream << m_authorization;
    stream << m_edge_nodeid;
    stream << m_target_addr;
    const int32_t end_pos = stream.size();
    return (end_pos - begin_pos);
}
int32_t xtransaction_v1_t::do_read(base::xstream_t & stream) {
    const int32_t begin_pos = stream.size();
    do_read_without_hash_signature(stream);
    stream >> m_transaction_hash;
    stream >> m_authorization;
    stream >> m_edge_nodeid;
    stream >> m_target_addr;
    const int32_t end_pos = stream.size();
    return (begin_pos - end_pos);
}

#ifdef XENABLE_PSTACK  // tracking memory
int32_t xtransaction_v1_t::add_ref() {
    MEMCHECK_ADD_TRACE(this, "tx_add");
    return base::xdataunit_t::add_ref();
}
int32_t xtransaction_v1_t::release_ref() {
    MEMCHECK_ADD_TRACE(this, "tx_release");
    xdbgassert(get_refcount() > 0);
    return base::xdataunit_t::release_ref();
}
#endif

void xtransaction_v1_t::adjust_target_address(uint32_t table_id) {
    if (m_target_addr.empty()) {
        xdbg("xtransaction_v1_t::adjust_target_address hash=%s,origin_addr=%s,new_addr=%s",
            get_digest_hex_str().c_str(), m_target_action.get_account_addr().c_str(), m_target_addr.c_str());        
    } else {
        xassert(false);
    }
}

void xtransaction_v1_t::set_digest() {
    base::xstream_t stream(base::xcontext_t::instance());
    do_write_without_hash_signature(stream, true);
    m_transaction_hash = utl::xsha2_256_t::digest((const char*)stream.data(), stream.size());
}

void xtransaction_v1_t::set_len() {
    base::xstream_t stream(base::xcontext_t::instance());
    const int32_t begin_pos = stream.size();
    serialize_write(stream, false);
    int32_t action_len = m_source_action.serialize_write(stream, false);
    m_source_action.set_action_size(action_len);
    action_len = m_target_action.serialize_write(stream, false);
    m_target_action.set_action_size(action_len);
    stream << m_transaction_hash;
    stream << m_authorization;
    set_tx_len(stream.size() - begin_pos);
}

bool xtransaction_v1_t::digest_check() const {
    base::xstream_t stream(base::xcontext_t::instance());
    do_write_without_hash_signature(stream, true);
    uint256_t hash = utl::xsha2_256_t::digest((const char*)stream.data(), stream.size());
    if (hash != m_transaction_hash) {
        xwarn("xtransaction_v1_t::digest_check fail. %s %s",
            to_hex_str(hash).c_str(), to_hex_str(m_transaction_hash).c_str());
        return false;
    }
    return true;
}

bool xtransaction_v1_t::unuse_member_check() const {
    if (get_tx_version() != 0) {
        return false;
    }
    if (get_to_ledger_id() != 0 || get_from_ledger_id() != 0) {
        return false;
    }
    if (get_random_nonce() != 0) {
        return false;
    }
    if (get_premium_price() != 0) {
        return false;
    }
    if (!get_challenge_proof().empty()) {
        return false;
    }
    if (!get_ext().empty()) {
        return false;
    }
    if (!m_edge_nodeid.empty()) {
        return false;
    }
    return true;
}

bool xtransaction_v1_t::transaction_len_check() const {
    // TODO(nathan):tx parameters validity check
    if (get_memo().size() > MAX_TRANSACTION_MEMO_SIZE) {
        xwarn("xtransaction_v1_t::transaction_len_check memo size too long.size:%d", get_memo().size());
        return false;
    }
    base::xstream_t stream(base::xcontext_t::instance());

    const int32_t begin_pos = stream.size();
    serialize_write(stream, true);
    int32_t action_len = m_source_action.serialize_write(stream, true);
    if (action_len != m_source_action.get_action_size()) {
        xwarn("xtransaction_v1_t::transaction_len_check src action_len not match:%d:%d", action_len, m_source_action.get_action_size());
        return false;
    }

    action_len = m_target_action.serialize_write(stream, true);
    if (action_len != m_target_action.get_action_size()) {
        xwarn("xtransaction_v1_t::transaction_len_check tgt action_len not match:%d:%d", action_len, m_target_action.get_action_size());
        return false;
    }

    stream << m_transaction_hash;
    stream << m_authorization;

    if (stream.size() - begin_pos != get_tx_len()) {
        xwarn("xtransaction_v1_t::transaction_len_check tx length not match. %d %d", stream.size() - begin_pos, get_tx_len());
        return false;
    }
    return true;
}

int32_t xtransaction_v1_t::make_tx_create_user_account(const std::string & addr) {
    set_tx_type(xtransaction_type_create_user_account);
    int32_t ret = xaction_source_null::serialze_to(m_source_action);
    if (ret) { return ret; }
    ret = xaction_create_user_account::serialze_to(m_target_action, addr);
    if (ret) { return ret; }

    return xsuccess;
}

int32_t xtransaction_v1_t::make_tx_transfer(const data::xproperty_asset & asset) {
    set_tx_type(xtransaction_type_transfer);
    int32_t ret = xaction_asset_out::serialze_to(m_source_action, asset);
    if (ret) { return ret; }
    ret = xaction_asset_in::serialze_to(m_target_action, asset, xaction_type_asset_in);
    if (ret) { return ret; }
    return xsuccess;
}

int32_t xtransaction_v1_t::make_tx_run_contract(const data::xproperty_asset & asset_out, const std::string& function_name, const std::string& para) {
    set_from_ledger_id(0);
    set_to_ledger_id(0);
    set_tx_type(xtransaction_type_run_contract);
    int32_t ret = xaction_asset_out::serialze_to(m_source_action, asset_out);
    if (ret) { return ret; }
    ret = xaction_run_contract::serialze_to(m_target_action, function_name, para);
    if (ret) { return ret; }
    return xsuccess;
}

int32_t xtransaction_v1_t::make_tx_run_contract(std::string const & function_name, std::string const & param) {
    data::xproperty_asset asset_out{0};
    return make_tx_run_contract(asset_out, function_name, param);
}

int32_t xtransaction_v1_t::set_different_source_target_address(const std::string & src_addr, const std::string & dst_addr) {
    m_source_action.set_account_addr(src_addr);
    m_target_action.set_account_addr(dst_addr);
    return xsuccess;
}

int32_t xtransaction_v1_t::set_same_source_target_address(const std::string & addr) {
    return set_different_source_target_address(addr, addr);
}

void xtransaction_v1_t::set_last_trans_hash_and_nonce(uint256_t last_hash, uint64_t last_nonce) {
    set_last_nonce(last_nonce);
    set_last_hash(utl::xxh64_t::digest(last_hash.data(), last_hash.size()));
}

bool xtransaction_v1_t::check_last_trans_hash(const uint256_t & account_last_hash) {
    bool ret = get_last_hash() == utl::xxh64_t::digest(account_last_hash.data(), account_last_hash.size());
    if (!ret) {
        xinfo("xtransaction_v1_t::check_last_trans_hash account_lasthash_256:%s account_lasthash_64:%lx, tx_lasthash:%lx",
            to_hex_str(account_last_hash).c_str(), utl::xxh64_t::digest(account_last_hash.data(), account_last_hash.size()), get_last_hash());
    }

    return ret;
}

std::string xtransaction_v1_t::get_digest_hex_str() const {
    if (m_transaction_hash_str.empty()) {
        m_transaction_hash_str = to_hex_str(m_transaction_hash);
    }
    return m_transaction_hash_str;
}

bool xtransaction_v1_t::check_last_nonce(uint64_t account_nonce) {
    return (get_last_nonce() == account_nonce);
}

void xtransaction_v1_t::set_fire_and_expire_time(uint16_t expire_duration) {
    struct timeval val;
    base::xtime_utl::gettimeofday(&val);
    set_fire_timestamp((uint64_t)val.tv_sec);
    set_expire_duration(expire_duration);
}

bool xtransaction_v1_t::sign_check() const {
    return false;
}

bool xtransaction_v1_t::pub_key_sign_check(xpublic_key_t const & pub_key) const {
    return false;
}

size_t xtransaction_v1_t::get_serialize_size() const {
    base::xstream_t stream(base::xcontext_t::instance());
    xassert(stream.size() == 0);
    const_cast<xtransaction_v1_t*>(this)->serialize_to(stream);
    return stream.size();
}

std::string xtransaction_v1_t::dump() const {
    char local_param_buf[256];
    xprintf(local_param_buf,    sizeof(local_param_buf),
    "{transaction:hash=%s,type=%u,from=%s,to=%s,nonce=%" PRIu64 ",refcount=%d,this=%p}",
    get_digest_hex_str().c_str(), (uint32_t)get_tx_type(), get_source_addr().c_str(), get_target_addr().c_str(),
    get_tx_nonce(), get_refcount(), this);
    return std::string(local_param_buf);
}

// should be deleted in the end
int32_t xtransaction_v1_t::parse(enum_xaction_type source_type, enum_xaction_type target_type, xtx_parse_data_t & tx_parse_data) {
    if (source_type == xaction_type_source_null) {
        data::xaction_source_null source_action;
        int32_t ret = source_action.parse(get_source_action());
        if (ret != xsuccess) {
            return ret;
        }
    }

    if (source_type == xaction_type_asset_out) {
        data::xaction_asset_out source_action;
        int32_t ret = source_action.parse(get_source_action());
        if (ret != xsuccess) {
            // should return error, but old topio won't be able to execute run_contract tx, because of source_action_param mistakenly set to ""
            if (get_tx_type() == xtransaction_type_run_contract) {
                xdbg("xtransaction_v1_t::parse empty source action param:%s, tx_hash:%s", get_source_action().get_action_param().c_str(), get_digest_hex_str().c_str());
            } else {
                return ret;
            }
        }
        tx_parse_data.m_asset = source_action.m_asset_out;
    }

#ifdef ENABLE_CREATE_USER  // debug use
    if (target_type == xaction_type_create_user_account) {
        data::xaction_create_user_account target_action;
        int32_t ret = target_action.parse(get_target_action());
        if (ret != xsuccess) {
            return ret;
        }
        tx_parse_data.m_new_account = target_action.m_address;
    }
#endif

    if (target_type == xaction_type_asset_in) {
        data::xaction_asset_in target_action;
        int32_t ret = target_action.parse(get_target_action());
        if (ret != xsuccess) {
            return ret;
        }
        if ((source_type == xaction_type_asset_out) && (target_action.m_asset.m_amount != tx_parse_data.m_asset.m_amount)) {
            return false;
        }
        tx_parse_data.m_asset = target_action.m_asset;
    }

    if (target_type == xaction_type_run_contract) {
        data::xaction_run_contract target_action;
        int32_t ret = target_action.parse(get_target_action());
        if (ret != xsuccess) {
            return ret;
        }
        tx_parse_data.m_function_name = target_action.m_function_name;
        tx_parse_data.m_function_para = target_action.m_para;
        xdbg("wish v1 target_action_name:%s", tx_parse_data.m_function_name.c_str());
    }

    if (target_type == xaction_type_pledge_token) {
        data::xaction_pledge_token target_action;
        int32_t ret = target_action.parse(get_target_action());
        if (ret != xsuccess) {
            return ret;
        }
        tx_parse_data.m_asset = target_action.m_asset;
    }

    if (target_type == xaction_type_redeem_token) {
        data::xaction_redeem_token target_action;
        int32_t ret = target_action.parse(get_target_action());
        if (ret != xsuccess) {
            return ret;
        }
        tx_parse_data.m_asset = target_action.m_asset;
    }

    if (target_type == xaction_type_pledge_token_vote) {
        data::xaction_pledge_token_vote target_action;
        int32_t ret = target_action.parse(get_target_action());
        if (ret != xsuccess) {
            return ret;
        }
        tx_parse_data.m_vote_num = target_action.m_vote_num;
        tx_parse_data.m_lock_duration = target_action.m_lock_duration;
    }

    if (target_type == xaction_type_redeem_token_vote) {
        data::xaction_redeem_token_vote target_action;
        int32_t ret = target_action.parse(get_target_action());
        if (ret != xsuccess) {
            return ret;
        }
        tx_parse_data.m_vote_num = target_action.m_vote_num;
    }

    return 0;
}

}  // namespace data
}  // namespace top
