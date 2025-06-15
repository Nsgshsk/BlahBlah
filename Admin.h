#pragma once
#include "User.h"

class Admin final : public User
{
    void generate_code();

public:
    Admin();
    Admin(const String& username, const String& password);
};
