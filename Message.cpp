#include "Message.h"
#include <iostream>
#pragma warning(disable:4996)

// Deep copies the provided message into current message
void Message::copyFrom(const Message& message)
{
    size_t length = strlen(message.sender_);
    this->sender_ = new char[length + 1];
    strcpy_s(this->sender_, length + 1, message.sender_);
    
    strcpy_s(this->dateTime_, message.dateTime_);
    
    length = strlen(message.message_);
    this->message_ = new char[length + 1];
    strcpy_s(this->message_, length + 1, message.message_);
}

// Moves fields from provided message to current
void Message::moveFrom(Message&& message)
{
    this->sender_ = message.sender_;
    message.sender_ = nullptr;
    
    strcpy_s(this->dateTime_, message.dateTime_);
    
    this->message_ = message.message_;
    message.message_ = nullptr;
}

// Frees dynamic memory allocated for fields
void Message::free()
{
    delete[] this->sender_;
    delete[] this->message_;
    this->sender_ = nullptr;
    this->message_ = nullptr;
    this->dateTime_[0] = '\0';
}

Message::Message(): dateTime_{}
{
    this->sender_ = nullptr;
    this->message_ = nullptr;
}

Message::Message(const char* sender, const char* message)
{
    // Setting message's sender
    size_t length = strlen(sender);
    this->sender_ = new char[length + 1];
    strcpy_s(this->sender_, length + 1, sender);

    // Setting current DateTime of message
    std::time_t now = time(nullptr);
    strcpy_s(this->dateTime_, ctime(&now));

    // Setting current message text
    length = strlen(message);
    this->message_ = new char[length + 1];
    strcpy_s(this->message_, length + 1, message);
}

Message::Message(const Message& other)
{
    copyFrom(other);
}

Message& Message::operator=(const Message& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

Message::~Message()
{
    free();
}

Message::Message(Message&& other) noexcept
{
    moveFrom(std::move(other));
}

Message& Message::operator=(Message&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << message.sender_ << '\n';
    os << message.dateTime_ << '\n';
    os << message.message_ << '\n';
    return os;
}
