#include "UserBase.h"
#include <iostream>
#include <fstream>

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

UserBase::UserBase(UserBase&& other) noexcept
{
    moveFrom(std::move(other));
}

UserBase& UserBase::operator=(UserBase&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

void UserBase::serialize(std::ofstream& ofs) const
{
    size_t temp = strlen(id_);
    ofs.write(id_, temp + 1);
    
    temp = strlen(name_);
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_, temp + 1);
}

void UserBase::deserialize(std::ifstream& ifs)
{
    free();

    size_t temp = strlen(id_);
    ifs.read(id_, temp + 1);
    
    ifs.read((char*)&temp, sizeof(size_t));
    ifs.read(name_, temp + 1);
}

void UserBase::serialize_debug(std::ofstream& ofs) const
{
    ofs << id_ << ' ';
    ofs << strlen(name_) << ' ';
    ofs << name_ << '\n';
}

void UserBase::deserialize_debug(std::ifstream& ifs)
{
    size_t temp;
    ifs >> id_ >> temp;
    name_ = new char[temp + 1];
    ifs >> name_;
}
