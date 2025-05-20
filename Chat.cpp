#include <ctime>

#include "Chat.h"
#include "User.h"
#include "HashUtility.h"

void Chat::generate_hash()
{
    char timebuff[DATE_TIME_MAX_SIZE]{'\0'};
    std::time_t now = time(nullptr);
    ctime_s(timebuff, DATE_TIME_MAX_SIZE, &now);

    String message_representation = timebuff;
    for (size_t i = 0; i < participants_.getSize(); i++)
    {
        const User& temp = getParticipant(i);
        message_representation += temp.getName();
        message_representation += HashUtility::hash_to_str(temp.getHash());
    }

    const uint8_t* temp = HashUtility::hash_chat(message_representation.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

Chat::Chat() = default;

Chat::Chat(const List<UserPtr>& participants)
{
    participants_ = participants;
    generate_hash();
}

bool Chat::isParticipantPresent(const UserPtr& user) const
{
    for (size_t i = 0; i < participants_.getSize(); i++)
        if (getParticipant(i) == *user)
            return true;

    return false;
}

const User& Chat::getParticipant(size_t index) const
{
    return *participants_[index];
}

const Message& Chat::getMessage(size_t index) const
{
    return messages_[index];
}

void Chat::addParticipant(const UserPtr& participant)
{
    if (participant == nullptr)
        throw std::invalid_argument("Participant is null");
    
    participants_.add(participant);
}

void Chat::removeParticipant(const UserPtr& participant)
{
    if (participant == nullptr)
        throw std::invalid_argument("participant is null");
    
    for (size_t i = 0; i < participants_.getSize(); i++)
        if (getParticipant(i) == *participant)
        {
            participants_.removeAt(i);
            return;
        }

    throw std::invalid_argument("Participant not found");
}

void Chat::sentMessage(const UserPtr& sender, const String& message)
{
    messages_.add(Message(sender->getName(), message));
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
    ofs.write((const char*)hash_, HASH_SIZE);

    for (size_t i = 0; i < participants_.getSize(); i++)
        ofs.write((const char*)getParticipant(i).getHash(), HASH_SIZE);
    
    this->messages_.serialize(ofs);
}

void Chat::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)hash_, HASH_SIZE);

    // Function to deserialize hashes and put correct users
    
    this->messages_.deserialize(ifs);
}

void Chat::serialize_debug(std::ofstream& ofs) const
{
    HashUtility::serialize_hash_text(ofs, hash_);

    for (size_t i = 0; i < participants_.getSize(); i++)
        HashUtility::serialize_hash_text(ofs, getParticipant(i).getHash());
    
    this->messages_.serialize_debug(ofs);
}

void Chat::deserialize_debug(std::ifstream& ifs)
{
    HashUtility::deserialize_hash_text(ifs, hash_);
    
    // Function to deserialize hashes and put correct users
    
    this->messages_.deserialize_debug(ifs);
}

std::ostream& operator<<(std::ostream& os, const Chat& chat)
{
    os << chat.participants_ << '\n';
    os << chat.messages_ << '\n';

    return os;
}
