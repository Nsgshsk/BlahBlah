#include "GroupChat.h"

GroupChat::GroupChat(const List<UserBase>& participants, const UserBase& owner, const String& name)
    : Chat(participants, ChatType::GROUP, owner, name)
{
}
