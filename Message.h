#pragma once
#include <iosfwd>

constexpr unsigned char DATE_TIME_SIZE = 32;

class Message
{
    char* sender_;
    char dateTime_[DATE_TIME_SIZE + 1];
    char* message_;

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
