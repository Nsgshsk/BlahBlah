#include "Message.h"
#include <iostream>
#include <fstream>

#include "HashUtility.h"
#pragma warning(disable:4996)

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

Message::Message(const String& sender, const String& message)
{
    // Setting message's sender
    sender_ = sender;

    // Setting current DateTime of message
    std::time_t now = time(nullptr);
    strcpy_s(this->dateTime_, ctime(&now));

    // Setting current message text
    message_ = message;

    // Generates message's hash
    generate_hash();
}

// Serializes message into a binary file
void Message::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)&hash_, HASH_SIZE);

    size_t temp = sender_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(sender_.c_str(), temp + 1);

    ofs.write(dateTime_, sizeof(dateTime_));

    temp = message_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(message_.c_str(), temp + 1);
}

// Deserializes message from a binary file
void Message::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)&hash_, HASH_SIZE);

    size_t temp; // Temporary variable for lengths
    char* str; // Temporary variable for strings

    // Sender string deserialization from binary
    ifs.read((char*)&temp, sizeof(size_t)); // Sender string length
    str = new char[temp + 1];
    ifs.read(str, temp + 1);
    sender_ = str;
    delete[] str;

    // Datetime string deserialization from binary
    ifs.read(dateTime_, sizeof(dateTime_));

    // Message string deserialization from text
    ifs.read((char*)&temp, sizeof(size_t)); // Message string length
    str = new char[temp + 1];
    ifs.read(str, temp + 1);
    message_ = str;
    delete[] str;
}

// Serializes message into a text file
void Message::serialize_debug(std::ofstream& ofs) const
{
    // Serializes hash
    HashUtility::serialize_hash_text(ofs, hash_);

    // Serializes sender string into text
    ofs << sender_.length() << '\n'; // Sender string length
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
    //Deserializes hash
    HashUtility::deserialize_hash_text(ifs, hash_);

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
    ifs.getline(str, temp + 1);
    message_ = str;
    delete[] str;
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    os << message.sender_ << '\n';
    os << message.dateTime_ << '\n';
    os << message.message_ << '\n';
    return os;
}
