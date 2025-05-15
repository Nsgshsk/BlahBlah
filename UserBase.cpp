#include "UserBase.h"
#include <iostream>
#include <fstream>

#include "HashManager.h"

void UserBase::copyFrom(const uint8_t* hash, const char* name)
{
    HashManager::copyHash(this->hash_, hash);
    size_t temp = strlen(name);
    this->name_ = new char[temp + 1];
    strcpy_s(this->name_, temp + 1, name);
}

void UserBase::copyFrom(const UserBase& other)
{
    copyFrom(other.hash_, other.name_);
}

void UserBase::moveFrom(UserBase&& other)
{
    HashManager::copyHash(this->hash_, other.hash_);
    this->name_ = other.name_;
    other.name_ = nullptr;
}

void UserBase::free()
{
    hash_ = {0};
    delete[] name_;
}

UserBase::UserBase() : hash_{0}, name_(nullptr) 
{
}

UserBase::UserBase(const uint8_t* hash, const char* name)
{
    copyFrom(hash, name);
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

bool UserBase::operator==(const UserBase& other) const
{
    for (size_t i = 0; i < HASH_LENGTH; i++)
        if (this->hash_[i] != other.hash_[i])
            return false;

    return true;
}

void UserBase::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)&hash_, HASH_LENGTH);
    
    size_t temp = strlen(name_);
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_, temp + 1);
}

void UserBase::deserialize(std::ifstream& ifs)
{
    free();
    ifs.read((char*)&hash_, HASH_LENGTH);

    size_t temp;
    ifs.read((char*)&temp, sizeof(size_t));
    name_ = new char[temp + 1];
    ifs.read(name_, temp + 1);
}

void UserBase::serialize_debug(std::ofstream& ofs) const
{
    for (uint8_t i = 0; i < HASH_LENGTH - 1; i++)
        ofs << this->hash_[i] << ' ';
    ofs << this->hash_[HASH_LENGTH - 1] << '\n';
    ofs << strlen(name_) << '\n';
    ofs << name_ << '\n';
}

void UserBase::deserialize_debug(std::ifstream& ifs)
{
    for (uint8_t i = 0; i < HASH_LENGTH - 1; i++)
        ifs >> this->hash_[i];
    
    size_t temp;
    ifs >> temp;
    name_ = new char[temp + 1];
    ifs >> name_;
}

bool operator!=(const UserBase& lhs, const UserBase& rhs)
{
    return !(lhs == rhs);
}
