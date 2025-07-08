#pragma once
#include "Chat.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

class DirectChat : public Chat
{
public:
    DirectChat(const UserBase& user, const UserBase& owner);
};
