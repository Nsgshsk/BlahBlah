#pragma once
#include <cstdint>
#include <iosfwd>

constexpr uint8_t DATE_TIME_MAX_SIZE = 32;

class Message
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
    ~Message();

    Message(Message&& other) noexcept;
    Message& operator=(Message&& other) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Message& message);
};
