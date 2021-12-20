use std::os::raw::{c_char, c_void};
use super::{Bytes, CBytesSlice,CErrorCode};

extern "C" {
    pub fn free_bytes(bytes: *const CBytesSlice);
    pub fn free_error_code(ec: *mut CErrorCode);

    pub fn withdraw(
        state_accessor_handle: *mut c_void,
        property_name: *const c_char,
        amount: u64,
        symbol: *const c_char,
        ec: *mut CErrorCode,
    ) -> u64;

    pub fn deposit(state_accessor_handle: *mut c_void, property_name: *const c_char, amount: u64, symbol: *const c_char, ec: *mut CErrorCode);

    pub fn get_property_bytes(state_accessor_handle: *mut c_void, property_name: *const c_char, ec: *mut CErrorCode) -> CBytesSlice;
    pub fn set_property_bytes(state_accessor_handle: *mut c_void, property_name: *const c_char, property_data: *const Bytes, ec: *mut CErrorCode);
}
