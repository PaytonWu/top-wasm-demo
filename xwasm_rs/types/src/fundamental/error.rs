#[derive(Debug, Clone)]
pub struct Error {
    value: i32,
    message: String,
}

impl Error {
    pub fn new(value: i32, msg: &str) -> Self {
        Error{ value, message: msg.to_string() }
    }

    pub fn value(&self) -> i32 {
        self.value
    }

    pub fn message(&self) -> &str {
        &self.message
    }
}
