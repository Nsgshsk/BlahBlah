#include "Member.h"

Member::Member() = default;

Member::Member(const String& username, const String& password)
    : User(username, password, UserRole::MEMBER)
{
}
