use std::os::raw::{c_char, c_int, c_void};
use super::Bytes;

extern "C" {
    pub fn free_bytes(bytes: *const Bytes);

    pub fn withdraw(
        state_accessor_handle: *mut c_void,
        property_name: *const ::std::os::raw::c_char,
        amount: u64,
        symbol: *const ::std::os::raw::c_char,
        ec: *mut ::std::os::raw::c_int,
    ) -> u64;

    pub fn get_property_bytes(state_accessor_handle: *mut c_void, property_name: *const c_char, ec: *mut c_int) -> Bytes;
    pub fn set_property_bytes(state_accessor_handle: *mut c_void, property_name: *const c_char, property_data: *const Bytes, ec: *mut c_int);
}
