#include "Chat.h"
#include <stdexcept>

namespace
{
    size_t participantsCount(const char* const* participants)
    {
        size_t count = 0;
        while (*participants != nullptr)
            count++;

        return count;
    }

    size_t messagesCount(const Message* messages)
    {
        size_t count = 0;
        while (messages != nullptr)
            count++;

        return count;
    }
}

void Chat::generateId()
{
    throw std::exception("Not implemented");
}

void Chat::copyParticipants(const char* const* participants)
{
    size_t count = participantsCount(participants);
    this->participants_ = new char*[count + 1]{nullptr};
    for (size_t i = 0; i < count; i++)
    {
        const char* temp = participants[i]; // This is a participant's name
        size_t tempLength = strlen(temp);
        this->participants_[i] = new char[tempLength + 1];
        strcpy_s(this->participants_[i], tempLength, temp);
    }
}

void Chat::copyMessages(const Message* messages)
{
    size_t count = messagesCount(messages);
    throw std::exception("Not implemented");
}

void Chat::deleteParticipants()
{
}

void Chat::deleteMessages()
{
}

void Chat::copyFrom(const Chat& chat)
{
    strcpy_s(this->id_, chat.id_);
    copyParticipants(chat.participants_);
}

void Chat::moveFrom(Chat&& chat)
{
}

void Chat::free()
{
}

Chat::Chat()
{
}

Chat::Chat(const char* const* participants)
{
}

Chat::Chat(const Chat& other)
{
}

Chat& Chat::operator=(const Chat& other)
{
}

Chat::~Chat()
{
}

Chat::Chat(Chat&& other)
{
}

Chat& Chat::operator=(Chat&& other)
{
}

const char* Chat::getId() const
{
}

const char* Chat::getParticipants() const
{
}

std::ostream& operator<<(std::ostream& os, const Chat& chat)
{
}
