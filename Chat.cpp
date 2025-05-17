#include "Chat.h"
#include "HashUtility.h"

void Chat::generateHash()
{
    String message_representation;
    for (size_t i = 0; i < participants_.getSize(); i++)
    {
        message_representation += participants_[i].getName();
        message_representation += HashUtility::hash_to_str(participants_[i].getHash());
    }

    const uint8_t* temp = HashUtility::hash_chat(message_representation.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

Chat::Chat() : hash_{0}
{
}

Chat::Chat(const SerializableList<UserBase>& participants) : hash_{}
{
    participants_ = participants;
    generateHash();
}

const uint8_t* Chat::getHash() const
{
    return hash_;
}

void Chat::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)&hash_, HASH_LENGTH);
    this->participants_.serialize(ofs);
    this->messages_.serialize(ofs);
}

void Chat::deserialize(std::ifstream& ifs)
{
    this->participants_.deserialize(ifs);
    this->messages_.deserialize(ifs);
}

void Chat::serialize_debug(std::ofstream& ofs) const
{
    this->participants_.serialize_debug(ofs);
    this->messages_.serialize_debug(ofs);
}

void Chat::deserialize_debug(std::ifstream& ifs)
{
    this->participants_.deserialize_debug(ifs);
    this->messages_.deserialize_debug(ifs);
}

std::ostream& operator<<(std::ostream& os, const Chat& chat)
{
    os << chat.participants_ << '\n';
    os << chat.messages_ << '\n';

    return os;
}
