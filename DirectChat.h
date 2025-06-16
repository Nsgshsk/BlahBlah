#pragma once
#include "Chat.h"

class DirectChat : public Chat
{
public:
    DirectChat(const UserBase& user, const UserBase& owner);
};
