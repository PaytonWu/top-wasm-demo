use std::os::raw::c_void;
use std::os::raw::c_int;
use types::fundamental::{Symbol, Token};
use crate::StateAccessorTrait;
use std::ffi::{CString};
use ffi::state_accessor::Bytes;
// use std::error::Error;
use std::io::{Error, ErrorKind};

#[repr(C)]
#[derive(Debug)]
pub struct StateAccessor {
    handle: *mut c_void,
}

impl From<*mut c_void> for StateAccessor {
    fn from(h: *mut c_void) -> Self {
        StateAccessor { handle: h }
    }
}

impl StateAccessorTrait for StateAccessor {
    fn withdraw(&self, property_name: &str, amount: u64, symbol: &Symbol) -> Result<Token, Error> {
            let mut ec: c_int = 0;
            let property_name_string = CString::new(property_name);
            let symbol_string = CString::new(symbol.value().as_str());

            let amount = unsafe { ffi::state_accessor::withdraw(self.handle, property_name_string?.as_ptr(), amount, symbol_string?.as_ptr(), &mut ec) };
            if ec != 0 {
                Err(Error::new(ErrorKind::Other, "ffi::state_accessor::withdraw"))
            } else {
                Ok(Token::new(amount, symbol.clone()))
            }
    }

    fn get_property_bytes(&self, property_name: &str) -> Result<Bytes, Error> {
        let mut ec: c_int = 0;
        let property_name_string = CString::new(property_name)?;
        let bytes = unsafe { ffi::state_accessor::get_property_bytes(self.handle, property_name_string.as_ptr(), &mut ec) };
        if ec != 0 {
            Err(Error::new(ErrorKind::Other, "ffi::state_accessor::get_property_bytes"))
        } else {
            Ok(bytes)
        }
    }

    fn set_property_bytes(&self, property_name: &str, bytes: &Bytes) -> Result<(), Error>{
        let mut ec: c_int = 0;
        let property_name_string = CString::new(property_name)?;
        unsafe { ffi::state_accessor::set_property_bytes(self.handle, property_name_string.as_ptr(), bytes as *const Bytes, &mut ec) };
        if ec != 0 {
            Err(Error::new(ErrorKind::Other, "ffi::state_accessor::set_property_bytes"))
        } else {
            Ok(())
        }
    }
}
