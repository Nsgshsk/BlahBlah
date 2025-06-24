#pragma once

#include <cstdint>

#include "Hashable.h"
#include "ISerializable.h"
#include "String.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

class UserBase : public Hashable
{
protected:
    String name_;
    UserBase(String name);

public:
    UserBase();

    const String& getName() const;

    void serialize_base(std::ofstream& ofs) const;
    void deserialize_base(std::ifstream& ifs);

    void serialize_base_debug(std::ofstream& ofs) const;
    void deserialize_base_debug(std::ifstream& ifs);

    friend std::ostream& operator<<(std::ostream& os, const UserBase& user);
};
