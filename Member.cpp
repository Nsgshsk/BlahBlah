#include "Member.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

Member::Member() = default;

Member::Member(const String& username, const String& password)
    : User(username, password, UserRole::MEMBER)
{
}
