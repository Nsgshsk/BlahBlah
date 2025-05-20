#pragma once

#include <cstdint>

#include "Hashable.h"
#include "ISerializable.h"
#include "String.h"

class UserBase : public Hashable
{
protected:
    String name_;

public:
    UserBase();
    UserBase(String name);

    const String& getName() const;

    void serialize_base(std::ofstream& ofs) const;
    void deserialize_base(std::ifstream& ifs);

    void serialize_base_debug(std::ofstream& ofs) const;
    void deserialize_base_debug(std::ifstream& ifs);

    friend std::ostream& operator<<(std::ostream& os, const UserBase& user);
};
