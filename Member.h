#pragma once
#include "User.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

class Member final : public User
{
public:
    Member();
    Member(const String& username, const String& password);
};
