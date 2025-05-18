#pragma once
#include <cstdint>
#include <iosfwd>

#include "Hashable.h"
#include "ISerializable.h"
#include "String.h"

constexpr uint8_t DATE_TIME_MAX_SIZE = 26;

class Message final : public Hashable, public ISerializable, public ISerializableDebug
{
    String sender_; // Sender's name
    // *Note: DateTime string ends with a newline character before terminating zero
    char dateTime_[DATE_TIME_MAX_SIZE]; // DateTime string
    String message_; // Message text

    void generate_hash() override;

public:
    Message();
    Message(String sender, String message);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Message& message);
};
