use std::os::raw::c_void;
use std::os::raw::c_int;
use types::fundamental::{Symbol, Token};
use crate::StateAccessorTrait;
use std::ffi::{CString};

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
    fn withdraw(&self, property_name: &str, amount: u64, symbol: &Symbol) -> Token {
        unsafe {
            let mut ec: c_int = 0;
            let property_name_string = CString::new(property_name).unwrap();
            let symbol_string = CString::new(symbol.value().as_str()).unwrap();

            let amount = ffi::state_accessor::withdraw(self.handle, property_name_string.as_ptr(), amount, symbol_string.as_ptr(), &mut ec);
            if ec != 0 {
                Token::new(0u64, symbol.clone())
            } else {
                Token::new(amount, symbol.clone())
            }
        }
    }
}
