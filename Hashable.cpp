#include "Hashable.h"

#include <functional>
#include <stdexcept>

void Hashable::generate_hash()
{
    throw std::logic_error("Hash generation not implemented");
}

Hashable::Hashable() = default;

Hashable::~Hashable() = default;

const uint8_t* Hashable::getHash() const
{
    return hash_;
}

bool Hashable::operator==(const Hashable& other) const
{
    for (uint8_t i = 0; i < HASH_SIZE; i++)
        if (hash_[i] != other.hash_[i])
            return false;

    return true;
}

bool operator!=(const Hashable& left, const Hashable& right)
{
    return !(left == right);
}
