#include "GroupChat.h"

GroupChat::GroupChat(const List<UserBase>& participants, const String& name = "")
    : Chat(participants, ChatType::GROUP, name)
{
}
