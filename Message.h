#pragma once
#include <cstdint>
#include <iosfwd>

#include "ISerializable.h"
#include "String.h"

constexpr uint8_t DATE_TIME_MAX_SIZE = 32;

class Message final : public ISerializable, public ISerializableDebug
{
    String sender_; // Sender's name
    // *Note: DateTime string ends with a newline character before terminating zero
    char dateTime_[DATE_TIME_MAX_SIZE + 1]; // DateTime string
    String message_; // Message text

public:
    Message();
    Message(const char* sender, const char* message);
    Message(const String& sender, const String& message);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Message& message);
};
