use std::os::raw::c_void;

extern "C" {
    pub fn withdraw(
        state_accessor_handle: *mut c_void,
        property_name: *const ::std::os::raw::c_char,
        amount: u64,
        symbol: *const ::std::os::raw::c_char,
        ec: *mut ::std::os::raw::c_int,
    ) -> u64;
}