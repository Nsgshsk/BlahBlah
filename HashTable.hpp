#pragma once
#include "Hashable.h"
#include "SerializableList.hpp"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

constexpr uint8_t BYTE_BITS = 8;
constexpr uint8_t BUCKET_BYTES = 1;
constexpr int BUCKETS_NUM = 1 << BUCKET_BYTES * BYTE_BITS;

template <class T>
class HashTable
{
    struct Bucket
    {
        SerializableList<T> list;
    };

    Bucket buckets_[BUCKETS_NUM];

public:
    static size_t getBucketIndex(const uint8_t hash[HASH_SIZE]);

    void add(const T& value);

    void remove(const uint8_t hash[HASH_SIZE]);

    const T* find(const uint8_t hash[HASH_SIZE]) const;

    T* find(const uint8_t hash[HASH_SIZE]);

    List<T> getAll() const;

    void serialize(std::ofstream& ofs) const;

    void deserialize(std::ifstream& ifs);

    void serialize_debug(std::ofstream& ofs) const;

    void deserialize_debug(std::ifstream& ifs);
};

template <class T>
size_t HashTable<T>::getBucketIndex(const uint8_t hash[16])
{
    size_t index = 0;

    for (size_t i = 0; i < BUCKET_BYTES; i++)
        index = (index << BYTE_BITS) | hash[i];

    return index % BUCKETS_NUM;
}

template <class T>
void HashTable<T>::add(const T& value)
{
    size_t index = getBucketIndex(value.getHash());
    buckets_[index].list.add(value);
}

template <class T>
void HashTable<T>::remove(const uint8_t hash[16])
{
    size_t index = getBucketIndex(hash);
    Bucket& bucket = buckets_[index];
    for (size_t i = 0; i < bucket.list.getSize(); i++)
        if (bucket.list[i] == hash)
            bucket.list.removeAt(i);
}

template <class T>
const T* HashTable<T>::find(const uint8_t hash[16]) const
{
    size_t index = getBucketIndex(hash);
    const Bucket& bucket = buckets_[index];
    for (size_t i = 0; i < bucket.list.getSize(); i++)
        if (bucket.list[i] == hash)
            return &bucket.list[i];

    return nullptr;
}

template <class T>
T* HashTable<T>::find(const uint8_t hash[16])
{
    size_t index = getBucketIndex(hash);
    Bucket& bucket = buckets_[index];
    for (size_t i = 0; i < bucket.list.getSize(); i++)
        if (bucket.list[i] == hash)
            return &bucket.list[i];

    return nullptr;
}

template <class T>
List<T> HashTable<T>::getAll() const
{
    List<T> list;
    for (size_t i = 0; i < BUCKETS_NUM; i++)
    {
        const Bucket& bucket = buckets_[i];
        for (size_t j = 0; j < bucket.list.getSize(); j++)
            list.add(bucket.list[j]);
    }
    return list;
}

template <class T>
void HashTable<T>::serialize(std::ofstream& ofs) const
{
    for (size_t i = 0; i < BUCKETS_NUM; i++)
        buckets_[i].list.serialize(ofs);
}

template <class T>
void HashTable<T>::deserialize(std::ifstream& ifs)
{
    for (size_t i = 0; i < BUCKETS_NUM; i++)
        buckets_[i].list.deserialize(ifs);
}

template <class T>
void HashTable<T>::serialize_debug(std::ofstream& ofs) const
{
    for (size_t i = 0; i < BUCKETS_NUM; i++)
        buckets_[i].list.serialize_debug(ofs);
}

template <class T>
void HashTable<T>::deserialize_debug(std::ifstream& ifs)
{
    for (size_t i = 0; i < BUCKETS_NUM; i++)
        buckets_[i].list.deserialize_debug(ifs);
}
