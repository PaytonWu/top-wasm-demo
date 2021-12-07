use std::string::String;
use std::convert::From;

#[derive(Debug)]
pub struct Symbol(String);

impl Symbol {
    pub const fn new() -> Self {
        Symbol(String::new())
    }

    pub fn value(&self) -> &String {
        &self.0
    }

    pub fn is_top(&self) -> bool{
        self.0.is_empty()
    }
}

impl From<&str> for Symbol {
    fn from(input: &str) -> Self {
        Symbol(String::from(input))
    }
}
