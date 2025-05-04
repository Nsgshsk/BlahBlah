#pragma once
#include "Message.h"

class Chat
{
    char* id_;
    char** participants_;
    Message* messages_;

    void generateId();

    void copyFrom(const Chat& chat);
    void moveFrom(Chat&& chat);
    void free();

public:
    Chat();
    Chat(const char* const* participants);
    Chat(const Chat& other);
    Chat& operator=(const Chat& other);
    ~Chat();

    Chat(Chat&& other);
    Chat& operator=(Chat&& other);

    const char* getId() const;
    const char* getParticipants() const;

    friend std::ostream& operator<<(std::ostream& os, const Chat& chat);
};
