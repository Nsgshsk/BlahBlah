#pragma once

#include "ISerializable.h"

class UserBase : public ISerializable, public ISerializableDebug
{
    char id_[16];
    char* name_;

    void copyFrom(const char* id, const char* name);
    void copyFrom(const UserBase& other);
    void moveFrom(UserBase&& other);
    void free();
public:
    UserBase();
    UserBase(const char* id, const char* name);
    UserBase(const UserBase& other);
    UserBase& operator=(const UserBase& other);
    ~UserBase() override;
    
    UserBase(UserBase&& other) noexcept;
    UserBase& operator=(UserBase&& other) noexcept;

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;
};
