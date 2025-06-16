#pragma once
#include "Chat.h"

class GroupChat : public Chat
{
public:
    GroupChat(const List<UserBase>& participants, const UserBase& owner, const String& name);
};
