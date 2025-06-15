#pragma once
#include "Chat.h"

class GroupChat : public Chat
{
public:
    GroupChat(const List<UserBase>& participants, const String& name);
};
