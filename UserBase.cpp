#include "UserBase.h"
#include <iostream>
#include <fstream>
#include "HashUtility.h"

UserBase::UserBase() = default;

UserBase::UserBase(String name) : name_(std::move(name))
{
}

const String& UserBase::getName() const
{
    return name_;
}

void UserBase::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)hash_, HASH_SIZE);

    size_t temp = name_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_.c_str(), temp + 1);
}

void UserBase::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)hash_, HASH_SIZE);

    size_t temp;
    ifs.read((char*)&temp, sizeof(size_t));
    char* str = new char[temp + 1];
    ifs.read(str, temp + 1);
    name_ = str;
    delete[] str;
}

void UserBase::serialize_debug(std::ofstream& ofs) const
{
    HashUtility::serialize_hash_text(ofs, hash_);
    ofs << name_.length() << '\n';
    ofs << name_ << '\n';
}

void UserBase::deserialize_debug(std::ifstream& ifs)
{
    HashUtility::deserialize_hash_text(ifs, hash_);
    ifs >> name_;
}

std::ostream& operator<<(std::ostream& os, const UserBase& user)
{
    return os << user.name_;
}
