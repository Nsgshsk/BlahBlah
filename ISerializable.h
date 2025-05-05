#pragma once
#include <iosfwd>

class ISerializable
{
public:
    virtual ~ISerializable() = default;
    virtual void serialize(std::ofstream& ofs) const = 0;
    virtual void deserialize(std::ifstream& ifs) = 0;
};
