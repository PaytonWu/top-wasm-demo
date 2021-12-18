use std::os::raw::c_void;
use types::fundamental::{Symbol, Token};
use crate::StateAccessorTrait;
use std::ffi::{CString};
use ffi::state_accessor::{Bytes, CBytes};

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
    fn withdraw(&self, property_name: &str, amount: u64, symbol: &Symbol) -> Result<Token, types::fundamental::Error> {
        let property_name_string = CString::new(property_name).map_err(|nul_error| types::fundamental::Error::new(nul_error.nul_position() as i32, nul_error.to_string().as_str(), "NulError"));
        let symbol_string = CString::new(symbol.value().as_str()).map_err(|nul_error| types::fundamental::Error::new(nul_error.nul_position() as i32, nul_error.to_string().as_str(), "NulError"));
        let mut ec = ffi::state_accessor::CErrorCode::ok();

        let amount = unsafe { ffi::state_accessor::withdraw(self.handle, property_name_string?.as_ptr(), amount, symbol_string?.as_ptr(), &mut ec) };
        if ec.value != 0 {
            Err(types::fundamental::Error::from(&ec))
        } else {
            Ok(Token::new(amount, symbol.clone()))
        }
    }

    fn get_property_bytes(&self, property_name: &str) -> Result<CBytes, types::fundamental::Error> {
        let property_name_string = CString::new(property_name).map_err(|nul_error| types::fundamental::Error::new(nul_error.nul_position() as i32, nul_error.to_string().as_str(),"NulError"));
        let mut ec = ffi::state_accessor::CErrorCode::ok();

        let bytes = unsafe { ffi::state_accessor::get_property_bytes(self.handle, property_name_string?.as_ptr(), &mut ec) };
        if ec.value != 0 {
            Err(types::fundamental::Error::from(&ec))
        } else {
            Ok(bytes)
        }
    }

    fn set_property_bytes(&self, property_name: &str, bytes: &Bytes) -> Result<(), types::fundamental::Error>{
        let property_name_string = CString::new(property_name).map_err(|nul_error| types::fundamental::Error::new(nul_error.nul_position() as i32, nul_error.to_string().as_str(), "NulError"));
        let mut ec = ffi::state_accessor::CErrorCode::ok();

        unsafe { ffi::state_accessor::set_property_bytes(self.handle, property_name_string?.as_ptr(), bytes as *const Bytes, &mut ec) };
        if ec.value != 0 {
            Err(types::fundamental::Error::from(&ec))
        } else {
            Ok(())
        }
    }
}
