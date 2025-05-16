#include "UserBase.h"
#include <iostream>
#include <fstream>

#include "HashManager.h"

UserBase::UserBase() : hash_{0}, name_(nullptr)
{
}

UserBase::UserBase(const uint8_t* hash, const String& name)
{
    HashManager::copy_hash(this->hash_, hash);
    name_ = name;
}

UserBase::UserBase(const uint8_t* hash, const char* name)
{
    HashManager::copy_hash(this->hash_, hash);
    name_ = name;
}

const String& UserBase::getName() const
{
    return name_;
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

    size_t temp = name_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_.c_str(), temp + 1);
}

void UserBase::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)&hash_, HASH_LENGTH);

    size_t temp;
    ifs.read((char*)&temp, sizeof(size_t));
    char* str = new char[temp + 1];
    ifs.read(str, temp + 1);
    name_ = str;
    delete[] str;
}

void UserBase::serialize_debug(std::ofstream& ofs) const
{
    ofs << std::hex;
    for (uint8_t i = 0; i < HASH_LENGTH - 1; i++)
        ofs << this->hash_[i] << ' ';
    ofs << this->hash_[HASH_LENGTH - 1] << std::dec << '\n';

    ofs << name_.length() << '\n';
    ofs << name_ << '\n';
}

void UserBase::deserialize_debug(std::ifstream& ifs)
{
    ifs >> std::hex;
    for (uint8_t i = 0; i < HASH_LENGTH; i++)
        ifs >> this->hash_[i];
    ifs >> std::dec;

    ifs >> name_;
}

bool operator!=(const UserBase& lhs, const UserBase& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const UserBase& user)
{
    return os << user.name_;
}
