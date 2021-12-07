use super::symbol::Symbol;

#[derive(Debug)]
pub struct Token {
    amount_: u64,
    symbol_: Symbol
}

impl Token {
    pub fn new(amount : u64) -> Self {
        Token{amount_: amount, symbol_: Symbol::new()}
    }

    pub fn amount(&self) -> u64 {
        self.amount_
    }

    pub fn symbol(&self) -> &Symbol {
        &self.symbol_
    }
}
