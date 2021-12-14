use ffi::state_accessor::BytesDataSpan;
use types::fundamental::{Symbol, Token};
// use std::error::Error;
use std::io::Error;

pub trait StateAccessorTrait {
    fn withdraw(&self, property_name: &str, amount: u64, symbol: &Symbol) -> Result<Token, Error>;
    fn get_property_bytes(&self, property_name: &str) -> Result<BytesDataSpan, Error>;
    fn set_property_bytes(&self, property_name: &str, bytes: &BytesDataSpan) -> Result<(), Error>;
}