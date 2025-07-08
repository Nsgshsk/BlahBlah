#pragma once
#include <iosfwd>

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

class ISerializable
{
public:
    virtual ~ISerializable() = default;
    virtual void serialize(std::ofstream& ofs) const = 0;
    virtual void deserialize(std::ifstream& ifs) = 0;
};

class ISerializableDebug
{
public:
    virtual ~ISerializableDebug() = default;
    virtual void serialize_debug(std::ofstream& ofs) const = 0;
    virtual void deserialize_debug(std::ifstream& ifs) = 0;
};
