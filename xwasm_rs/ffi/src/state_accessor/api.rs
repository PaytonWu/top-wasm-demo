use std::os::raw::c_void;

extern "C" {
    pub fn withdraw(
        state_accessor_handle: *mut c_void,
        property_name: *const ::std::os::raw::c_char,
        amount: u64,
        symbol: *const ::std::os::raw::c_char,
        ec: *mut ::std::os::raw::c_int,
    ) -> u64;

    pub fn xproperty_object_t * get_property(void * state_accessor_handle, char const * property_name, top::state_accessor::properties::xproperty_type_t property_type, int * ec);
}