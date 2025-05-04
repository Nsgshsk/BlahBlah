#pragma once
#include "Message.h"

class Chat
{
    char id_[16];
    char** participants_;
    Message* messages_;

    void generateId();

    void copyParticipants(const char* const* participants);
    void copyMessages(const Message* messages);

    void copyFrom(const Chat& chat);
    void moveFrom(Chat&& chat);

    void deleteParticipants();
    void deleteMessages();
    void free();

public:
    Chat();
    Chat(const char* const* participants);
    Chat(const Chat& other);
    Chat& operator=(const Chat& other);
    ~Chat();

    Chat(Chat&& other) noexcept;
    Chat& operator=(Chat&& other) noexcept;

    const char* getId() const;
    const char* getParticipants() const;

    friend std::ostream& operator<<(std::ostream& os, const Chat& chat);
};
