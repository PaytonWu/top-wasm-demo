use super::StateAccessorHandle;

extern "C" {
    pub fn withdraw(
        state_accessor_handle: StateAccessorHandle,
        property_name: *const ::std::os::raw::c_char,
        amount: u64,
        symbol: *const ::std::os::raw::c_char,
        ec: *mut ::std::os::raw::c_int,
    ) -> u64;
}

pub trait StateAccessorApi {
    fn
}