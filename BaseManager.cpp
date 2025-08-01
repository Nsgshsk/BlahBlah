﻿#include "BaseManager.h"

#include <iostream>
#include <stdexcept>

#include "String.h"
#include "User.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

void BaseManager::change_password_command(const String& old_password, const String& new_password) const
{
    try
    {
        user_->change_password(old_password, new_password);
        std::cout << "Password changed successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void BaseManager::change_password_input() const
{
    String old_password, new_password;
    std::cin >> old_password >> new_password;

    change_password_command(old_password, new_password);
}

BaseManager::BaseManager(User* user, DataRepository* data)
{
    if (!user || !data)
        throw std::invalid_argument("Invalid user or data");

    user_ = user;
    data_ = data;
}

BaseManager::~BaseManager() = default;
