#pragma once

class UserBase
{
    char id_[16];
    char* name_;

    void copyFrom(const char* id, const char* name);
    void copyFrom(const UserBase& other);
    void moveFrom(UserBase&& other);
    void free();
public:
    UserBase();
    UserBase(const char* id, const char* name);
    UserBase(const UserBase& other);
    UserBase& operator=(const UserBase& other);
    ~UserBase();
    
    UserBase(UserBase&& other) noexcept;
    UserBase& operator=(UserBase&& other) noexcept;
};
