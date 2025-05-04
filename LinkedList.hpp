#pragma once
#include <stdexcept>

template <typename T>
class LinkedList
{
    struct Node
    {
        T value;
        Node* prev;
        Node* next;

        Node(T value, Node* prev = nullptr, Node* next = nullptr) : value(value), prev(prev), next(next)
        {
        }
    };

    Node* head;
    Node* tail;
    size_t size;

    void copyFrom(const LinkedList& other)
    {
        Node* current = other.head;
        while (current != nullptr)
        {
        }
        throw std::exception("Not implemented");
    }

    void moveFrom(LinkedList&& other)
    {
        this->head = other.head;
        this->tail = other.tail;
        other.head = nullptr;
        other.tail = nullptr;
    }

    void free()
    {
        while (head != nullptr)
        {
            Node* current = head;
            head = head->next;
            delete current;
        }
        tail = nullptr;
        size = 0;
    }

    // Checks if index is correct and throws an exception if not
    void indexCheckError(size_t index)
    {
        if (index >= size || index < 0)
            throw std::out_of_range("index is out of range");
    }

    T& getValueAt(size_t index)
    {
        Node* current = head;
        size_t i = 0;
        while (i != index)
        {
            current = current->next;
            i++;
        }
        return current->value;
    }

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0)
    {
    }

    LinkedList(const LinkedList& other)
    {
        copyFrom(other);
    }

    LinkedList& operator=(const LinkedList& other)
    {
        if (this != &other)
        {
            free();
            copyFrom(other);
        }
        return *this;
    }

    ~LinkedList()
    {
        free();
    }

    LinkedList(LinkedList&& other) noexcept
    {
        moveFrom(std::move(other));
    }

    LinkedList& operator=(LinkedList&& other) noexcept
    {
        if (this != &other)
        {
            free();
            moveFrom(std::move(other));
        }
        return *this;
    }

    size_t getSize() const
    {
        return this->size;
    }

    T& operator[](size_t index)
    {
        indexCheckError(index);

        return getValueAt(index);
    }

    const T& operator[](size_t index) const
    {
        indexCheckError(index);

        return getValueAt(index);
    }

    // Clears list
    void clear()
    {
        free();
    }

    // Adds a new Node to the end of the list
    void add(const T& value)
    {
        Node* node = new Node(value, this->tail);
        this->tail->next = node;
        if (head == nullptr)
            head = node;
    }

    void insert(size_t index, const T& value)
    {
        indexCheckError(index);
        throw std::exception("Not implemented");
    }
};
