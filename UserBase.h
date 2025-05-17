#pragma once

#include <cstdint>

#include "Hashable.h"
#include "ISerializable.h"
#include "String.h"

class UserBase : public Hashable, public ISerializable, public ISerializableDebug
{
    String name_;

public:
    UserBase();
    UserBase(const uint8_t* hash, const String& name);

    const String& getName() const;

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const UserBase& user);
};
