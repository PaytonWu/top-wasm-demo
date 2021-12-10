use super::properties::PropertyType;
use std::os::raw::c_void;

#[repr(C)]
pub struct PropertyObject{
    property_value: * mut c_void,
    property_type: PropertyType
}
