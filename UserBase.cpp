#include "UserBase.h"
#include <iostream>
#include <fstream>
#include "HashUtility.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

UserBase::UserBase() = default;

UserBase::UserBase(String name) : name_(std::move(name))
{
}

const String& UserBase::getName() const
{
    return name_;
}

void UserBase::serialize_base(std::ofstream& ofs) const
{
    ofs.write((const char*)hash_, HASH_SIZE);

    name_.serialize(ofs);
}

void UserBase::deserialize_base(std::ifstream& ifs)
{
    ifs.read((char*)hash_, HASH_SIZE);

    name_.deserialize(ifs);
}

void UserBase::serialize_base_debug(std::ofstream& ofs) const
{
    HashUtility::serialize_hash_text(ofs, hash_);
    ofs << name_ << '\n';
}

void UserBase::deserialize_base_debug(std::ifstream& ifs)
{
    HashUtility::deserialize_hash_text(ifs, hash_);
    ifs >> name_;
}

std::ostream& operator<<(std::ostream& os, const UserBase& user)
{
    return os << user.name_;
}
