#include "DirectChat.h"

DirectChat::DirectChat(const UserBase& user, const UserBase& owner)
    : Chat(user, ChatType::DIRECT, owner, user.getName())
{
}
