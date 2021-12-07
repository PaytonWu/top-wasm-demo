#[repr(C)]
#[derive(Debug)]
pub struct StateAccessor {
    pub unused_handle: *mut ::std::os::raw::c_void,
}
pub type StateAccessorHandle = *mut StateAccessor;
