use types::fundamental::Symbol;
use types::fundamental::Token;

pub trait StateAccessorTrait {
    fn withdraw(&self, property_name: &str, amount: u64, symbol: &Symbol) -> Token;
}