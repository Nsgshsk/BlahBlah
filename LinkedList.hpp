#pragma once
#include <fstream>
#include <stdexcept>

#include "ISerializable.h"

template <typename T>
class LinkedList : public ISerializable
{
    // Base Node class
    struct Node
    {
        T value;
        Node* prev;
        Node* next;

        Node(T value, Node* prev = nullptr, Node* next = nullptr) : value(value), prev(prev), next(next)
        {
        }
    };

    // Structure of linked list
    Node* head_;
    Node* tail_;
    size_t size_;

    // Checks if index is correct and throws an exception if not
    static void indexCheckError(size_t index, size_t size)
    {
        if (index >= size || index < 0)
            throw std::out_of_range("index is out of range");
    }

    // Copies items from other to this list
    void copyFrom(const LinkedList& other)
    {
        Node* current = other.head_;
        while (current != nullptr)
        {
            add(current->value);
            current = current->next;
        }
    }

    // Moves the items head and tail of the rvalue list other to this
    void moveFrom(LinkedList&& other)
    {
        this->head_ = other.head_;
        this->tail_ = other.tail_;
        this->size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
    }

    // Removes first node from linked list and returns it
    Node* popNode()
    {
        Node* current = head_;

        if (current == nullptr)
            throw std::out_of_range("Can't pop node from empty list");

        head_ = head_->next;
        if (head_ == nullptr)
            tail_ = nullptr;

        return current;
    }

    // Frees dynamic memory taken for linked list
    void free()
    {
        while (head_ != nullptr)
            delete popNode();

        tail_ = nullptr;
        size_ = 0;
    }

    // Iterates the list from head to tail
    static void listIteration(size_t index, Node*& current)
    {
        size_t i = 0;
        while (i != index)
        {
            current = current->next;
            i++;
        }
    }

    // Iterates the list from tail to head
    static void listIteration(size_t index, Node*& current, size_t size)
    {
        size_t i = size - 1;
        while (i != index)
        {
            current = current->prev;
            i--;
        }
    }

    // Returns node at specified index
    Node* getNodeAt(size_t index) const
    {
        Node* current;

        // Check which iteration is faster for provided index
        if (index <= size_ / 2)
        {
            current = head_;
            listIteration(index, current);
        }
        else
        {
            current = tail_;
            listIteration(index, current, size_);
        }

        return current;
    }

    // Returns value at specified index
    T& getValueAt(size_t index) const
    {
        return getNodeAt(index)->value;
    }

public:
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0)
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

    ~LinkedList() override
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
        return this->size_;
    }

    T& operator[](size_t index)
    {
        indexCheckError(index, size_);
        return getValueAt(index);
    }

    const T& operator[](size_t index) const
    {
        indexCheckError(index, size_);
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
        Node* node = new Node(value, this->tail_);

        if (head_ == nullptr)
            head_ = node;
        else
            this->tail_->next = node;

        this->tail_ = node;

        size_++;
    }

    // Inserts an item at given index and shifts elements to the right 
    void insertAt(size_t index, const T& value)
    {
        indexCheckError(index, size_);
        if (index == 0) // Special case for first item
        {
            Node* node = new Node(value, nullptr, this->head_);
            this->head_->prev = node;
            this->head_ = node;
        }
        else
        {
            Node* temp = getNodeAt(index);
            Node* node = new Node(value, temp->prev, temp);
            temp->prev->next = node;
            temp->prev = node;
        }
        size_++;
    }

    // Removes an item at a given index and shifts elements to the left
    void removeAt(size_t index)
    {
        indexCheckError(index, size_);
        if (index == 0) // Similar functionality as pop
            delete popNode();
        else if (index == size_ - 1) // Special case for end of list
        {
            Node* temp = this->tail_;
            this->tail_ = temp->prev;
            this->tail_->next = nullptr;
            delete temp;
        }
        else
        {
            Node* temp = getNodeAt(index);
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
        }
        size_--;
    }

    // Pops an item from the head of the list
    void popFront()
    {
        if (head_ == nullptr)
            throw std::out_of_range("Can't pop item from empty list");
        removeAt(0);
    }

    // Pops an item from the tail of the list
    void popBack()
    {
        if (tail_ == nullptr)
            throw std::out_of_range("Can't pop item from empty list");
        removeAt(size_ - 1);
    }

    // Serializes a list into a binary file
    void serialize(std::ofstream& ofs) const override
    {
        // Writes the size of the list at the beginning of the file
        ofs.write((const char*)&size_, sizeof(size_t));

        // Writes each value of the list into a file
        Node* current = head_;
        while (current != nullptr)
        {
            ofs.write((const char*)&current->value, sizeof(T));
            current = current->next;
        }
    }

    // Deserializes a binary file into the elements of the linked list
    void deserialize(std::ifstream& ifs) override
    {
        free();
        ifs.read((char*)&size_, sizeof(size_t));
        for (size_t i = 0; i < size_; i++)
        {
            T value;
            ifs.read((char*)&value, sizeof(T));
            this->add(value);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list)
    {
        Node* current = list.head_;
        while (current != nullptr)
        {
            os << current->value << '\n';
            current = current->next;
        }

        return os;
    }
};
