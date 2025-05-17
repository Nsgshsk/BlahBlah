#pragma once
#include <cstdint>

constexpr uint8_t HASH_SIZE = 16;

class Hashable
{
protected:
    uint8_t hash_[HASH_SIZE];
    virtual void generate_hash();

public:
    Hashable();
    virtual ~Hashable();
    const uint8_t* getHash() const;
    bool operator==(const Hashable& other) const;
};

bool operator!=(const Hashable& left, const Hashable& right);
