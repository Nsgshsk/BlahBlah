#pragma once
#include <fstream>
#include <iostream>
#include "List.hpp"

template<class T>
class SerializableList : public List<T>
{
public:
    void serialize(std::ofstream& ofs) const;
    void deserialize(std::ifstream& ifs);
    void serialize_debug(std::ofstream& ofs) const;
    void deserialize_debug(std::ifstream& ifs);
};

template <class T>
void SerializableList<T>::serialize(std::ofstream& ofs) const
{
    size_t size = this->getSize();
    ofs.write((const char*)&size, sizeof(size_t));
    for (size_t i = 0; i < size; i++)
        (*this)[i].serialize(ofs);
}

template <class T>
void SerializableList<T>::deserialize(std::ifstream& ifs)
{
    this->clear();

    size_t size = this->getSize();
    ifs.read((char*)&size, sizeof(size_t));;
    this->reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        T value;
        value.deserialize(ifs);
        this->add(value);
    }
}

template <class T>
void SerializableList<T>::serialize_debug(std::ofstream& ofs) const
{
    ofs << this->getSize() << '\n';
    for (size_t i = 0; i < this->getSize(); i++)
        (*this)[i].serialize_debug(ofs);
}

template <class T>
void SerializableList<T>::deserialize_debug(std::ifstream& ifs)
{
    this->clear();

    size_t size;
    ifs >> size;
    this->reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        T value;
        value.deserialize_debug(ifs);
        this->add(value);
    }
}
