use std::fmt;

#[derive(Debug, Clone)]
pub struct Error {
    value: i32,
    message: String,
    category: String,
}

impl Error {
    pub fn new(value: i32, msg: &str, category: &str) -> Self {
        Error{ value, message: msg.to_string(), category: category.to_string() }
    }

    pub fn value(&self) -> i32 {
        self.value
    }

    pub fn message(&self) -> &str {
        &self.message
    }

    pub fn category(&self) -> &str {
        &self.category
    }
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "ec: {}, msg: {}, category: {}", self.value(), self.message(), self.category())
    }
}

impl std::error::Error for Error {
}