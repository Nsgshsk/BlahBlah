#include "Message.h"
#include <iostream>
#include <fstream>

#include "HashUtility.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

void Message::generate_hash()
{
    String represent = sender_;
    represent += dateTime_;
    represent += message_;

    const uint8_t* temp = HashUtility::hash_message(represent.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

Message::Message() = default;

Message::Message(String sender, String message) : sender_(std::move(sender)), message_(std::move(message))
{
    // Setting current DateTime of message
    std::time_t now = time(nullptr);
    ctime_s(dateTime_, DATE_TIME_MAX_SIZE, &now);

    // Generates message's hash
    generate_hash();
}

// Serializes message into a binary file
void Message::serialize(std::ofstream& ofs) const
{
    sender_.serialize(ofs);
    ofs.write(dateTime_, DATE_TIME_MAX_SIZE);
    message_.serialize(ofs);
}

// Deserializes message from a binary file
void Message::deserialize(std::ifstream& ifs)
{
    // Sender string deserialization from binary
    sender_.deserialize(ifs);

    // Datetime string deserialization from binary
    ifs.read(dateTime_, DATE_TIME_MAX_SIZE);

    // Message string deserialization from text
    message_.deserialize(ifs);

    generate_hash();
}

// Serializes message into a text file
void Message::serialize_debug(std::ofstream& ofs) const
{
    // Serializes sender string into text
    ofs << sender_ << '\n';

    // Serializes datetime string into text
    ofs << strlen(dateTime_) << '\n'; // Datetime string length
    ofs << dateTime_;

    // Serializes message into text
    ofs << message_.length() << '\n'; // Message string length
    ofs << message_ << '\n';
}

// Deserializes message from text file
void Message::deserialize_debug(std::ifstream& ifs)
{
    ifs >> sender_;

    size_t temp; // Temporary variable for lengths

    // Datetime string deserialization from text
    ifs >> temp; // Datetime string length
    ifs.ignore(); // Ignores the newline character
    ifs.getline(dateTime_, temp + 1);
    dateTime_[temp - 1] = '\n'; // Puts a newline character at the end of datetime string

    // Message string deserialization from text
    ifs >> temp; // Message string length
    char* str = new char[temp + 1];
    ifs.ignore(); // Ignores the newline character
    ifs.read(str, temp + 1);
    message_ = str;
    delete[] str;

    generate_hash();
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << message.sender_ << '\n';
    os << message.dateTime_ << '\n';
    os << message.message_ << '\n';
    return os;
}
