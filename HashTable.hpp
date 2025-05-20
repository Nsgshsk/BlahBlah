#pragma once
#include "Hashable.h"
#include "SerializableList.hpp"

constexpr uint8_t BYTE_BITS = 8;
constexpr uint8_t BUCKET_BYTES = 2;
constexpr int BUCKETS_NUM = 1 << BUCKET_BYTES * BYTE_BITS;

template<class T>
class HashTable
{
    struct Bucket
    {
        SerializableList<T> list;
    };

    Bucket buckets[BUCKETS_NUM];
public:
    static size_t getBucketIndex(const uint8_t hash[HASH_SIZE])
    {
        size_t index = 0;

        for (size_t i = 0; i < BUCKET_BYTES; i++)
            index = (index << BYTE_BITS) | hash[i];

        return index % BUCKETS_NUM;
    }

    void add(const T& value)
    {
        size_t index = getBucketIndex(value.getHash());
        buckets[index].list.add(value);
    }

    void remove(const uint8_t hash[HASH_SIZE])
    {
        size_t index = getBucketIndex(hash);
        Bucket& bucket = buckets[index];
        for (size_t i = 0; i < bucket.list.getSize(); i++)
            if (bucket.list[i] == hash)
                bucket.list.removeAt(i);
    }
    
    T* find(const uint8_t hash[HASH_SIZE]) const
    {
        size_t index = getBucketIndex(hash);
        Bucket& bucket = buckets[index];
        for (size_t i = 0; i < bucket.list.getSize(); i++)
            if (bucket.list[i] == hash)
                return &bucket.list[i];

        return nullptr;
    }

    void serialize(std::ofstream& ofs) const
    {
        for (size_t i = 0; i < BUCKETS_NUM; i++)
            buckets[i].list.serialize(ofs);
    }

    void serialize_debug(std::ofstream& ofs) const
    {
        for (size_t i = 0; i < BUCKETS_NUM; i++)
            buckets[i].list.serialize_debug(ofs);
    }
};
