#include "Hashable.h"

#include <functional>
#include <stdexcept>

#include "HashUtility.h"

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
    return *this == other.hash_;
}

bool Hashable::operator==(const uint8_t hash[16]) const
{
    return HashUtility::compare_hash(hash_, hash);
}

bool operator!=(const Hashable& left, const Hashable& right)
{
    return !(left == right);
}

bool operator!=(const Hashable& left, const uint8_t right[16])
{
    return !(left == right);
}
