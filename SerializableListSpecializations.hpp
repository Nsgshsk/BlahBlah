#pragma once
#include "Chat.h"
#include "SerializableList.hpp"

template <>
class SerializableList<UserBase> : public List<UserBase>
{
public:
    void serialize(std::ofstream& ofs) const;
    void deserialize(std::ifstream& ifs);
    void serialize_debug(std::ofstream& ofs) const;
    void deserialize_debug(std::ifstream& ifs);
};

inline void SerializableList<UserBase>::serialize(std::ofstream& ofs) const
{
    size_t size = this->getSize();
    ofs.write((const char*)&size, sizeof(size_t));
    for (size_t i = 0; i < size; i++)
        (*this)[i].UserBase::serialize(ofs);
}

inline void SerializableList<UserBase>::deserialize(std::ifstream& ifs)
{
    this->clear();

    size_t size = this->getSize();
    ifs.read((char*)&size, sizeof(size_t));
    this->reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        UserBase value;
        value.UserBase::deserialize(ifs);
        this->add(value);
    }
}

inline void SerializableList<UserBase>::serialize_debug(std::ofstream& ofs) const
{
    ofs << this->getSize() << '\n';
    for (size_t i = 0; i < this->getSize(); i++)
        (*this)[i].UserBase::serialize_debug(ofs);
}

inline void SerializableList<UserBase>::deserialize_debug(std::ifstream& ifs)
{
    this->clear();

    size_t size;
    ifs >> size;
    this->reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        UserBase value;
        value.UserBase::deserialize_debug(ifs);
        this->add(value);
    }
}

template <>
class SerializableList<Chat> : public List<Chat>
{
public:
    void serialize(std::ofstream& ofs) const;
    void deserialize(std::ifstream& ifs);
    void serialize_debug(std::ofstream& ofs) const;
    void deserialize_debug(std::ifstream& ifs);
};

inline void SerializableList<Chat>::serialize(std::ofstream& ofs) const
{
    size_t size = this->getSize();
    ofs.write((const char*)&size, sizeof(size_t));
    for (size_t i = 0; i < size; i++)
        (*this)[i].serialize(ofs);
}

inline void SerializableList<Chat>::deserialize(std::ifstream& ifs)
{
    this->clear();

    size_t size = this->getSize();
    ifs.read((char*)&size, sizeof(size_t));;
    this->reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        Chat value;
        value.deserialize(ifs);
        this->add(value);
    }
}

inline void SerializableList<Chat>::serialize_debug(std::ofstream& ofs) const
{
    ofs << this->getSize() << '\n';
    for (size_t i = 0; i < this->getSize(); i++)
        (*this)[i].serialize_debug(ofs);
}

inline void SerializableList<Chat>::deserialize_debug(std::ifstream& ifs)
{
    this->clear();

    size_t size;
    ifs >> size;
    this->reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        Chat value;
        value.deserialize_debug(ifs);
        this->add(value);
    }
}
