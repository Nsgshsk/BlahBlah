#include "DirectChat.h"

#include "UserBase.h"

DirectChat::DirectChat(const UserBase& user) : Chat(user, ChatType::DIRECT, "")
{
}
