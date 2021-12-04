// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <string>
#include "xdata/xcons_transaction.h"
#include "xdata/xdata_common.h"

namespace top { namespace data {

xcons_transaction_t::xcons_transaction_t() {
}

xcons_transaction_t::xcons_transaction_t(xtransaction_t* tx) {
    tx->add_ref();
    m_tx.attach(tx);
    update_transation();
}

xcons_transaction_t::xcons_transaction_t(const base::xfull_txreceipt_t & full_txreceipt) {
    if (!full_txreceipt.get_tx_org_bin().empty()) {
        xtransaction_t::set_tx_by_serialized_data(m_tx, full_txreceipt.get_tx_org_bin());
    }
    m_receipt = full_txreceipt.get_txreceipt();
    update_transation();
}

xcons_transaction_t::~xcons_transaction_t() {
}

bool xcons_transaction_t::set_raw_tx(xtransaction_t* raw_tx) {
    if (m_tx != nullptr) { // already set raw tx
        xassert(false);
        return false;
    }
    if (m_receipt == nullptr) { // must be receipt tx
        xassert(false);
        return false;
    }
    if (raw_tx->get_digest_str() != m_receipt->get_tx_hash()) {
        xassert(false);
        return false;
    }
    raw_tx->add_ref();
    m_tx.attach(raw_tx);
    return true;
}

xobject_ptr_t<base::xvqcert_t> xcons_transaction_t::get_receipt_prove_cert_and_account(std::string & account) const {
    if (m_receipt == nullptr) {
        xerror("no receipt");
        return nullptr;
    }
    if (m_receipt->get_prove_cert() == nullptr) {
        xerror("commit prove null");
        return nullptr;
    }

    account = get_account_addr();
    if (m_receipt->get_prove_type() == base::enum_xprove_cert_type_table_justify) {
        // change to parent account
    }
    return m_receipt->get_prove_cert();
}

void xcons_transaction_t::update_transation() {
    if (m_receipt == nullptr) {
        if (m_tx->get_source_addr() == m_tx->get_target_addr()) {
            set_tx_subtype(enum_transaction_subtype_self);
        } else {
            set_tx_subtype(enum_transaction_subtype_send);
        }
    } else {
        if (m_receipt->get_tx_subtype() == enum_transaction_subtype_send) {
            set_tx_subtype(enum_transaction_subtype_recv);
        } else if (m_receipt->get_tx_subtype() == enum_transaction_subtype_recv) {
            set_tx_subtype(enum_transaction_subtype_confirm);
        } else {
            xassert(0);
        }
    }
}

int32_t xcons_transaction_t::do_write(base::xstream_t & stream) {
    KEEP_SIZE();
    uint8_t has_tx = m_tx != nullptr;
    stream << has_tx;
    if (m_tx != nullptr) {
        m_tx->serialize_to(stream);
    }
    uint8_t has_receipt = m_receipt != nullptr;
    stream << has_receipt;
    if (m_receipt != nullptr) {
        m_receipt->serialize_to(stream);
    }
    return CALC_LEN();
}

int32_t xcons_transaction_t::do_read(base::xstream_t & stream) {
    KEEP_SIZE();
    uint8_t has_tx;
    stream >> has_tx;
    if (has_tx) {
        auto _tx = base::xdataunit_t::read_from(stream);
        xassert(_tx != nullptr);
        auto tx = dynamic_cast<xtransaction_t*>(_tx);
        m_tx.attach(tx);
    }
    uint8_t has_receipt;
    stream >> has_receipt;
    if (has_receipt) {
        base::xtx_receipt_t* _receipt = dynamic_cast<base::xtx_receipt_t*>(base::xdataunit_t::read_from(stream));
        xassert(_receipt != nullptr);
        m_receipt.attach(_receipt);
    }

    update_transation();

    return CALC_LEN();
}

bool xcons_transaction_t::verify_cons_transaction() {
    bool ret = true;
    return ret;
}

uint64_t xcons_transaction_t::get_dump_receipt_id() const {
    return 0;
}

std::string xcons_transaction_t::dump(bool detail) const {
    xassert(m_receipt != nullptr || m_tx != nullptr);// should not both null

    std::stringstream ss;

    return ss.str();
}

uint32_t xcons_transaction_t::get_last_action_used_tgas() const {

    return 0;
}
uint32_t xcons_transaction_t::get_last_action_used_deposit() const {

    return 0;
}

uint32_t xcons_transaction_t::get_last_action_send_tx_lock_tgas() const {

    return 0;
}
enum_xunit_tx_exec_status xcons_transaction_t::get_last_action_exec_status() const {

    return (enum_xunit_tx_exec_status)0;
}

uint32_t xcons_transaction_t::get_last_action_recv_tx_use_send_tx_tgas() const {

    return 0;
}
uint64_t xcons_transaction_t::get_last_action_receipt_id() const {

    return 0;
}

uint64_t xcons_transaction_t::get_last_action_sender_confirmed_receipt_id() const {

    return 0;
}
base::xtable_shortid_t xcons_transaction_t::get_last_action_self_tableid() const {

    return 0;
}

base::xtable_shortid_t xcons_transaction_t::get_last_action_peer_tableid() const {

    return 0;
}

std::string xcons_transaction_t::get_tx_hash() const {
    if (m_receipt != nullptr) {
        return m_receipt->get_tx_hash();
    } else if (get_transaction() != nullptr) {
        return get_transaction()->get_digest_str();
    } else {
        xassert(false);
        return {};
    }
}

uint256_t xcons_transaction_t::get_tx_hash_256() const {  // TODO(jimmy) always use string type hash
    if (get_transaction() != nullptr) {
        return get_transaction()->digest();
    } else if (m_receipt != nullptr) {
        return uint256_t((uint8_t*)m_receipt->get_tx_hash().c_str());
    } else {
        xassert(false);
        return {};
    }
}

std::string xcons_transaction_t::get_account_addr() const {
    return {};
}

}  // namespace data
}  // namespace top
