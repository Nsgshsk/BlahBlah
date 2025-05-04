#pragma once

class Message
{
    char* sender_;
    char dateTime_[32];
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

    const char* getSender() const;
    const char* getDateTime() const;
    const char* getMessage() const;
};
