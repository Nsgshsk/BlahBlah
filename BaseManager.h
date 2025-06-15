#pragma once

class User;
class DataRepository;

class BaseManager
{
    User* user_;
    DataRepository* data_;

public:
    BaseManager(User* user, DataRepository* data);
    void login();
};
