#pragma once
#include "User.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

class Admin final : public User
{
    void generate_code();

public:
    Admin();
    Admin(const String& username, const String& password);
};
