#pragma once

class User;
class DataRepository;

class MemberManager
{
    User* user_;
    DataRepository* data_;

public:
    MemberManager(User* user, DataRepository* data);
};
