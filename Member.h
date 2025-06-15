#pragma once
#include "User.h"

class Member final : public User
{
public:
    Member();
    Member(const String& username, const String& password);
};
