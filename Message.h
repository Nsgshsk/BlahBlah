#pragma once
#include <cstdint>
#include <iosfwd>

#include "ISerializable.h"

constexpr uint8_t DATE_TIME_MAX_SIZE = 32;

class Message final : public ISerializable, public ISerializableDebug
{
    char* sender_; // Sender's name
    char dateTime_[DATE_TIME_MAX_SIZE + 1]; // DateTime string
    char* message_; // Message text

    void copyFrom(const Message& message);
    void moveFrom(Message&& message);
    void free();

public:
    Message();
    Message(const char* sender, const char* message);
    Message(const Message& other);
    Message& operator=(const Message& other);
    ~Message() override;

    Message(Message&& other) noexcept;
    Message& operator=(Message&& other) noexcept;

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Message& message);
};
