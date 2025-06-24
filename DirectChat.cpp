#include "DirectChat.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

DirectChat::DirectChat(const UserBase& user, const UserBase& owner)
    : Chat(user, ChatType::DIRECT, owner, owner.getName() + ", " + user.getName())
{
}
