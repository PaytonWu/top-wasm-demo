use std::ops::Deref;
use std::slice;
use crate::state_accessor::free_bytes;
use super::properties::PropertyType;
use std::os::raw::{c_void};

#[repr(C)]
pub struct PropertyObject{
    property_value: * mut c_void,
    property_type: PropertyType
}

#[repr(C)]
pub struct Bytes {
    pub ptr: *const u8,
    pub size: u64
}

impl Drop for Bytes {
    fn drop(&mut self) {
        unsafe { free_bytes(self)}
    }
}

impl Deref for Bytes {
    type Target = [u8];

    fn deref(&self) -> &[u8] {
        unsafe { slice::from_raw_parts(self.ptr, self.size as usize) }
    }
}
