use std::os::raw::{c_char, c_int, c_void};
use super::DataSpan;

extern "C" {
    pub fn withdraw(
        state_accessor_handle: *mut c_void,
        property_name: *const ::std::os::raw::c_char,
        amount: u64,
        symbol: *const ::std::os::raw::c_char,
        ec: *mut ::std::os::raw::c_int,
    ) -> u64;

    pub fn get_property_serialized(state_accessor_handle: *mut c_void, property_name: *const c_char, ec: *mut c_int) -> DataSpan;
}