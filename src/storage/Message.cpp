#include "storage/Message.h"

Message::Message()
    : offset(0),
      timestamp(0) {
}

Message::Message(
    uint64_t offset,
    uint64_t timestamp,
    const std::string& key,
    const std::string& value
)
    : offset(offset),
      timestamp(timestamp),
      key(key),
      value(value) {
}