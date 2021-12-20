use std::ops::Deref;
use std::slice;
use crate::state_accessor::{free_bytes, free_error_code};
use super::properties::PropertyType;
use std::os::raw::{c_char, c_void};

#[repr(C)]
pub struct PropertyObject{
    property_value: * mut c_void,
    property_type: PropertyType
}

#[repr(C)]
pub struct CBytesSlice {
    pub ptr: *const u8,
    pub size: u64,
}

impl Drop for CBytesSlice {
    fn drop(&mut self) {
        unsafe { free_bytes(self)}
    }
}

impl Deref for CBytesSlice {
    type Target = [u8];

    fn deref(&self) -> &[u8] {
        unsafe { slice::from_raw_parts(self.ptr, self.size as usize) }
    }
}

#[repr(C)]
pub struct Bytes {
    pub ptr: *const u8,
    pub size: u64,
}

#[repr(C)]
pub struct CStringSlice {
    pub ptr: *const c_char,
    pub size: u64,
}

#[repr(C)]
pub struct CErrorCode {
    pub value: i32,
    pub message: *const c_char,
    pub category: *const c_char,
}

impl CErrorCode {
    pub fn ok() -> Self {
        CErrorCode{value: 0, message: 0 as *const c_char, category: 0 as *const c_char}
    }
}

impl Drop for CErrorCode {
    fn drop(&mut self) {
        unsafe { free_error_code(self) }
    }
}

// impl Deref for CErrorCode {
//     type Target = String;
//
//     fn deref(&self) -> String {
//
//     }
// }