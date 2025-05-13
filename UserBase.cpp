#include "UserBase.h"

#include <algorithm>
#include <cstring>

void UserBase::copyFrom(const char* id, const char* name)
{
    strcpy_s(this->id_, id);
    size_t temp = strlen(name);
    this->name_ = new char[temp + 1];
    strcpy_s(this->name_, temp + 1, name);
}

void UserBase::copyFrom(const UserBase& other)
{
    copyFrom(other.id_, other.name_);
}

void UserBase::moveFrom(UserBase&& other)
{
    strcpy_s(this->id_, other.id_);
    this->name_ = other.name_;
    other.name_ = nullptr;
}

void UserBase::free()
{
    id_[0] = '\0';
    delete[] name_;
}

UserBase::UserBase() : id_{}, name_(nullptr) 
{
}

UserBase::UserBase(const char* id, const char* name)
{
    copyFrom(id, name);
}

UserBase::UserBase(const UserBase& other)
{
    copyFrom(other);
}

UserBase& UserBase::operator=(const UserBase& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

UserBase::~UserBase()
{
    free();
}

UserBase::UserBase(UserBase&& other)
{
    moveFrom(std::move(other));
}

UserBase& UserBase::operator=(UserBase&& other)
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}
