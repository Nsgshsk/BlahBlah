#pragma once

#include <cstdint>

#include "ISerializable.h"
#include "String.h"

constexpr uint8_t HASH_LENGTH = 16;

class UserBase : public ISerializable, public ISerializableDebug
{
    uint8_t hash_[HASH_LENGTH];
    String name_;

public:
    UserBase();
    UserBase(const uint8_t* hash, const String& name);
    UserBase(const uint8_t* hash, const char* name);

    const String& getName() const;
    const uint8_t* getHash() const;
    bool operator==(const UserBase& other) const;

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const UserBase& user);
};

bool operator!=(const UserBase& lhs, const UserBase& rhs);
