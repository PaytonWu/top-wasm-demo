use ffi::state_accessor::Bytes;
use types::fundamental::{Symbol, Token};
// use std::error::Error;
use std::io::Error;

pub trait StateAccessorTrait {
    fn withdraw(&self, property_name: &str, amount: u64, symbol: &Symbol) -> Result<Token, Error>;
    fn get_property_bytes(&self, property_name: &str) -> Result<Bytes, Error>;
    fn set_property_bytes(&self, property_name: &str, bytes: &Bytes) -> Result<(), Error>;
}