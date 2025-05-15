#pragma once

#include <cstdint>

#include "ISerializable.h"

constexpr uint8_t HASH_LENGTH = 16;

class UserBase : public ISerializable, public ISerializableDebug
{
    uint8_t hash_[HASH_LENGTH];
    char* name_;

    void copyFrom(const uint8_t* hash, const char* name);
    void copyFrom(const UserBase& other);
    void moveFrom(UserBase&& other);
    void free();
public:
    UserBase();
    UserBase(const uint8_t* hash, const char* name);
    UserBase(const UserBase& other);
    UserBase& operator=(const UserBase& other);
    ~UserBase() override;
    
    UserBase(UserBase&& other) noexcept;
    UserBase& operator=(UserBase&& other) noexcept;

    bool operator==(const UserBase& other) const;

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;
};

bool operator!=(const UserBase& lhs, const UserBase& rhs);


