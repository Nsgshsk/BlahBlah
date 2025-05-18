#include "Chat.h"
#include "HashUtility.h"
#include <ctime>

void Chat::generate_hash()
{
    char timebuff[DATE_TIME_MAX_SIZE]{'\0'};
    std::time_t now = time(nullptr);
    ctime_s(timebuff, DATE_TIME_MAX_SIZE, &now);

    String message_representation = timebuff;
    for (size_t i = 0; i < participants_.getSize(); i++)
    {
        message_representation += participants_[i].getName();
        message_representation += HashUtility::hash_to_str(participants_[i].getHash());
    }

    const uint8_t* temp = HashUtility::hash_chat(message_representation.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

Chat::Chat() = default;

Chat::Chat(const SerializableList<UserBase>& participants)
{
    participants_ = participants;
    generate_hash();
}

const UserBase& Chat::getParticipant(int index) const
{
    return participants_[index];
}

const Message& Chat::getMessage(int index) const
{
    return messages_[index];
}

void Chat::addParticipant(const UserBase& participant)
{
    participants_.add(participant);
}

void Chat::removeParticipant(const UserBase& participant)
{
    for (size_t i = 0; i < participants_.getSize(); i++)
        if (participants_[i] == participant)
        {
            participants_.removeAt(i);
            return;
        }

    throw std::invalid_argument("Participant not found");
}

void Chat::sentMessage(const UserBase& sender, const String& message)
{
    messages_.add(Message(sender.getName(), message));
}

void Chat::deleteMessage(const Message& message)
{
    for (size_t i = 0; i < messages_.getSize(); i++)
        if (messages_[i] == message)
        {
            messages_.removeAt(i);
            return;
        }

    throw std::invalid_argument("Message not found");
}

void Chat::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)&hash_, HASH_SIZE);
    this->participants_.serialize(ofs);
    this->messages_.serialize(ofs);
}

void Chat::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)&hash_, HASH_SIZE);
    this->participants_.deserialize(ifs);
    this->messages_.deserialize(ifs);
}

void Chat::serialize_debug(std::ofstream& ofs) const
{
    HashUtility::serialize_hash_text(ofs, hash_);
    this->participants_.serialize_debug(ofs);
    this->messages_.serialize_debug(ofs);
}

void Chat::deserialize_debug(std::ifstream& ifs)
{
    HashUtility::deserialize_hash_text(ifs, hash_);
    this->participants_.deserialize_debug(ifs);
    this->messages_.deserialize_debug(ifs);
}

std::ostream& operator<<(std::ostream& os, const Chat& chat)
{
    os << chat.participants_ << '\n';
    os << chat.messages_ << '\n';

    return os;
}
