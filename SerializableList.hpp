#pragma once
#include <iosfwd>

#include "ISerializable.h"
#include "LinkedList.hpp"

template <typename T,
std::enable_if_t<std::is_base_of_v<ISerializable, T>, int> = 0>
class SerializableList : public LinkedList<T>
{
public:
    void serialize(std::ofstream& ofs) const
    {
        // Writes the size of the list at the beginning of the file
        size_t temp = this->getSize();
        ofs.write((const char*)&temp, sizeof(size_t));

        const typename LinkedList<T>::Node* current = this->peekNode();
        while (current != nullptr)
        {
            current->value.serialize(ofs);
            current = current->next;
        }
    }

    void deserialize(std::ifstream& ifs)
    {
        this->clear();
        size_t temp;

        ifs.read((char*)&temp, sizeof(size_t));
        for (size_t i = 0; i < temp; i++)
        {
            T value;
            value.deserialize(ifs);
            this->add(value);
        }
    }

    void serialize_debug(std::ofstream& ofs) const
    {
        ofs << this->getSize() << '\n';
        const typename LinkedList<T>::Node* current = this->peekNode();
        while (current != nullptr)
        {
            current->value.serialize_debug(ofs);
            current = current->next;
        }
    }

    void deserialize_debug(std::ifstream& ifs)
    {
        this->clear();

        size_t temp;
        ifs >> temp;
        ifs.ignore();
        for (size_t i = 0; i < temp; i++)
        {
            T value;
            value.deserialize_debug(ifs);
            this->add(value);
        }
    }
};