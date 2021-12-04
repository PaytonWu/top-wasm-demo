#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct StateAccessor {
    pub unused_handle: *mut ::std::os::raw::c_void,
}
pub type StateAccessorHandle = *mut StateAccessor;

extern "C" {
    pub fn withdraw(
        state_accessor_handle: StateAccessorHandle,
        property_name: *const ::std::os::raw::c_char,
        amount: u64,
        symbol: *const ::std::os::raw::c_char,
        ec: *mut ::std::os::raw::c_int,
    ) -> u64;
}
