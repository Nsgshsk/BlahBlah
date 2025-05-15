#pragma once
#include <fstream>
#include <iostream>
#include "ISerializable.h"
#include "List.hpp"

template<class T>
class SerializableList : public List<T*>, public ISerializable, public ISerializableDebug
{
public:
    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;
    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;
};

template <class T>
void SerializableList<T>::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)&this->size_, sizeof(size_t));
    for (size_t i = 0; i < this->size_; i++)
        (*this)[i].serialize(ofs);
}

template <class T>
void SerializableList<T>::deserialize(std::ifstream& ifs)
{
    this->clear();
    
    ifs.read((char*)&this->size_, sizeof(size_t));;
    this->reserve(this->size_);
    for (size_t i = 0; i < this->size_; i++)
    {
        T value;
        value.deserialize(ifs);
        this->add(value);
    }
}

template <class T>
void SerializableList<T>::serialize_debug(std::ofstream& ofs) const
{
    ofs << this->size_ << '\n';
    for (size_t i = 0; i < this->size_; i++)
        (*this)[i].serialize_debug(ofs);
}

template <class T>
void SerializableList<T>::deserialize_debug(std::ifstream& ifs)
{
    this->clear();
    
    ifs >> this->size_;
    this->reserve(this->size_);
    for (size_t i = 0; i < this->size_; i++)
    {
        T value;
        value.deserialize(ifs);
        this->add(value);
    }
}
