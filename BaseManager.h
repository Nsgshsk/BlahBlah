#pragma once
#include "String.h"

class User;
class DataRepository;

class BaseManager
{
    void change_password_command(const String& old_password, const String& new_password) const;

protected:
    User* user_;
    DataRepository* data_;

    void change_password_input() const;

    virtual void help_command() = 0;
    virtual void info_command() = 0;

public:
    BaseManager(User* user, DataRepository* data);
    BaseManager(const BaseManager& other) = delete;
    BaseManager& operator=(const BaseManager& other) = delete;
    virtual ~BaseManager();

    BaseManager(BaseManager&& other) = delete;
    BaseManager& operator=(BaseManager&& other) = delete;

    virtual void login() = 0;
};
