#include "Message.h"
#include <iostream>
#include <fstream>
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

// Serializes message into a binary file
void Message::serialize(std::ofstream& ofs) const
{
    size_t temp = strlen(sender_);
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(sender_, temp + 1);

    ofs.write(dateTime_, sizeof(dateTime_));

    temp = strlen(message_);
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(message_, temp + 1);
}

// Deserializes message from a binary file
void Message::deserialize(std::ifstream& ifs)
{
    free(); // Clears message before deserializing new data

    size_t temp; // Temporary variable for lengths

    // Sender string deserialization from binary
    ifs.read((char*)&temp, sizeof(size_t)); // Sender string length
    sender_ = new char[temp + 1];
    ifs.read(sender_, temp + 1);

    // Datetime string deserialization from binary
    ifs.read(dateTime_, sizeof(dateTime_));

    // Message string deserialization from text
    ifs.read((char*)&temp, sizeof(size_t)); // Message string length
    message_ = new char[temp + 1];
    ifs.read(message_, temp + 1);
}

// Serializes message into a text file
void Message::serialize_debug(std::ofstream& ofs) const
{
    // Serializes sender string into text
    ofs << strlen(sender_) << '\n'; // Sender string length
    ofs << sender_ << '\n';

    // Serializes datetime string into text
    ofs << strlen(dateTime_) << '\n'; // Datetime string length
    ofs << dateTime_;

    // Serializes message into text
    ofs << strlen(message_) << '\n'; // Message string length
    ofs << message_ << '\n';
}

// Deserializes message from text file
void Message::deserialize_debug(std::ifstream& ifs)
{
    free(); // Clears message before deserializing new data

    size_t temp; // Temporary variable for lengths

    // Sender string deserialization from text
    ifs >> temp; // Sender string length
    sender_ = new char[temp + 1];
    ifs >> sender_;

    // Datetime string deserialization from text
    ifs >> temp; // Datetime string length
    ifs.ignore(); // Ignores the newline character
    ifs.getline(dateTime_, temp + 1);
    dateTime_[temp - 1] = '\n'; // Puts a newline character at the end of datetime string

    // Message string deserialization from text
    ifs >> temp; // Message string length
    message_ = new char[temp + 1];
    ifs.ignore(); // Ignores the newline character
    ifs.getline(message_, temp + 1);
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << message.sender_ << '\n';
    os << message.dateTime_ << '\n';
    os << message.message_ << '\n';
    return os;
}
