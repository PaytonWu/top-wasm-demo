use super::properties::PropertyType;
use std::os::raw::{c_void, c_uchar, c_ulonglong};

#[repr(C)]
pub struct PropertyObject{
    property_value: * mut c_void,
    property_type: PropertyType
}

#[repr(C)]
pub struct DataSpan {
    ptr: *mut c_uchar,
    size: c_ulonglong
}