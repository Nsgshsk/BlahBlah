#pragma once
#include <cstdint>

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

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
    bool operator==(const uint8_t hash[HASH_SIZE]) const;
};

bool operator!=(const Hashable& left, const Hashable& right);
bool operator!=(const Hashable& left, const uint8_t right[HASH_SIZE]);
