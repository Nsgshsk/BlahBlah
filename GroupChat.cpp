#include "GroupChat.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

GroupChat::GroupChat(const List<UserBase>& participants, const UserBase& owner, const String& name)
    : Chat(participants, ChatType::GROUP, owner, name)
{
}
