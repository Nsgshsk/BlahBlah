#pragma once
#include "BaseManager.h"

class User;
class DataRepository;

class MemberManager : public BaseManager
{
protected:
    void help_command() override;
    void info_command() override;

public:
    MemberManager(User* user, DataRepository* data);
    void login() override;
};
