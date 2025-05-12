#pragma once
#include <iosfwd>

#include "ISerializable.h"
#include "LinkedList.hpp"
#include "Message.h"

template <>
class SerializableList<Message> : public LinkedList<Message>, public ISerializableDebug
{
public:
    void serialize(std::ofstream& ofs) const final
    {
        // Writes the size of the list at the beginning of the file
        size_t temp = getSize();
        ofs.write((const char*)&temp, sizeof(size_t));

        const Node* current = peekNode();
        while (current != nullptr)
        {
            current->value.serialize(ofs);
            current = current->next;
        }
    }

    void deserialize(std::ifstream& ifs) final
    {
        clear();
        size_t temp;

        ifs.read((char*)&temp, sizeof(size_t));
        for (size_t i = 0; i < temp; i++)
        {
            Message message;
            message.deserialize(ifs);
            add(message);
        }
    }

    void serialize_debug(std::ofstream& ofs) const final
    {
        ofs << getSize() << '\n';
        const Node* current = peekNode();
        while (current != nullptr)
        {
            current->value.serialize_debug(ofs);
            current = current->next;
        }
    }

    void deserialize_debug(std::ifstream& ifs) final
    {
        clear();

        size_t temp;
        ifs >> temp;
        ifs.ignore();
        for (size_t i = 0; i < temp; i++)
        {
            Message message;
            message.deserialize_debug(ifs);
            add(message);
        }
    }
};
