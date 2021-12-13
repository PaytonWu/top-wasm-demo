#[derive(Debug, Clone)]
pub struct ErrorCode {
    value: i32,
    message: String,
    category: String,
}

impl ErrorCode {
    pub fn new(value: i32, message: &str, category: &str) -> Self {
        ErrorCode{ value, message: message.to_string(), category: category.to_string() }
    }

    pub fn value(&self) -> i32 {
        self.value
    }

    pub fn message(&self) -> &str {
        self.message.as_str()
    }

    pub fn category(&self) -> &str {
        self.category.as_str()
    }
}
