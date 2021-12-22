use ffi::state_accessor::{Bytes, CBytesSlice};
use types::fundamental::{Symbol, Token};

pub trait StateAccessorTrait {
    fn withdraw(&self, property_name: &str, amount: u64, symbol: &Symbol) -> Result<Token, types::fundamental::Error>;
    fn deposit(&self, property_name: &str, token: Token) -> Result<(), types::fundamental::Error>;
    fn get_property_bytes(&self, property_name: &str) -> Result<CBytesSlice, types::fundamental::Error>;
    fn set_property_bytes(&self, property_name: &str, bytes: &Bytes) -> Result<(), types::fundamental::Error>;
    // fn get_property_u64(&self, property_name: &str) -> Result<u64, types::fundamental::Error>;
    // fn set_property_u64(&self, property_name: &str, value: u64) -> Result<(), types::fundamental::Error>;
}