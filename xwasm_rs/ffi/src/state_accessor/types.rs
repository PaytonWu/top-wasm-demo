use super::properties::PropertyType;
use std::os::raw::{c_void, c_uchar};

#[repr(C)]
pub struct PropertyObject{
    property_value: * mut c_void,
    property_type: PropertyType
}

#[repr(C)]
pub struct BytesDataSpan {
    ptr: *mut c_uchar,
    size: u64
}