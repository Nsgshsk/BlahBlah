#pragma once
#include <fstream>
#include <stdexcept>

#include "ISerializable.h"

template <typename T>
class LinkedList : public ISerializable
{
protected:
    // Base Node class
    struct Node
    {
        T value;
        Node* prev;
        Node* next;

        Node(T& value, Node* prev = nullptr, Node* next = nullptr);
        Node(T&& value, Node* prev = nullptr, Node* next = nullptr);
    };

    const Node* peekNode() const;

private:
    // Structure of linked list
    Node* head_;
    Node* tail_;
    size_t size_;
    
    static void indexCheckError(size_t index, size_t size);
    static void emptyCheckError(Node* head);
    
    void copyFrom(const LinkedList& other);
    void moveFrom(LinkedList&& other);
    
    Node* popNode();
    Node* popNodeBack();
    
    void free();
    
    static void listIteration(size_t index, Node*& current);
    static void listIteration(size_t index, Node*& current, size_t size);
    
    Node* getNodeAt(size_t index) const;
    T& getValueAt(size_t index) const;

public:
    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);
    ~LinkedList() override;

    LinkedList(LinkedList&& other) noexcept;
    LinkedList& operator=(LinkedList&& other) noexcept;

    size_t getSize() const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    
    void clear();
    
    void add(const T& value);
    void insertAt(size_t index, const T& value);
    void removeAt(size_t index);
    
    void pop();
    T& peek();
    const T& peek() const;
    
    void popBack();
    T& peekBack();
    const T& peekBack() const;
    
    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

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

// Node constructor
template <typename T>
LinkedList<T>::Node::Node(T& value, Node* prev, Node* next): value(value), prev(prev), next(next)
{
}

// Rvalue Node constructor
template <typename T>
LinkedList<T>::Node::Node(T&& value, Node* prev, Node* next): prev(prev), next(next)
{
    this->value = std::move(value);
}

// Returns a pointer to head Node even if it's nullptr
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::peekNode() const
{
    return head_;
}

// Checks if index is correct and throws an exception if not
template <typename T>
void LinkedList<T>::indexCheckError(size_t index, size_t size)
{
    if (index >= size || index < 0)
        throw std::out_of_range("Index is out of range");
}

// Checks if node provided is empty if yes it throws an exception
template <typename T>
void LinkedList<T>::emptyCheckError(Node* head)
{
    if (head == nullptr)
        throw std::out_of_range("List is empty");
}

// Copies items from other to this list
template <typename T>
void LinkedList<T>::copyFrom(const LinkedList& other)
{
    Node* current = other.head_;
    while (current != nullptr)
    {
        add(current->value);
        current = current->next;
    }
}

// Moves the items head and tail of the rvalue list other to this
template <typename T>
void LinkedList<T>::moveFrom(LinkedList&& other)
{
    this->head_ = other.head_;
    this->tail_ = other.tail_;
    this->size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
}

// Removes first node from linked list and returns it
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::popNode()
{
    Node* current = head_;

    emptyCheckError(current);

    head_ = head_->next;
    if (head_ == nullptr)
        tail_ = nullptr;

    return current;
}

// Removes last node from linked list and returns it
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::popNodeBack()
{
    Node* current = tail_;

    emptyCheckError(current);

    tail_ = tail_->prev;
    if (tail_ == nullptr)
        head_ = nullptr;

    return current;
}

// Frees dynamic memory taken for linked list
template <typename T>
void LinkedList<T>::free()
{
    while (head_ != nullptr)
        delete popNode();

    tail_ = nullptr;
    size_ = 0;
}

// Iterates the list from head to tail
template <typename T>
void LinkedList<T>::listIteration(size_t index, Node*& current)
{
    size_t i = 0;
    while (i != index)
    {
        current = current->next;
        i++;
    }
}

// Iterates the list from tail to head
template <typename T>
void LinkedList<T>::listIteration(size_t index, Node*& current, size_t size)
{
    size_t i = size - 1;
    while (i != index)
    {
        current = current->prev;
        i--;
    }
}

// Returns node at specified index
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::getNodeAt(size_t index) const
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
template <typename T>
T& LinkedList<T>::getValueAt(size_t index) const
{
    return getNodeAt(index)->value;
}

template <typename T>
LinkedList<T>::LinkedList(): head_(nullptr), tail_(nullptr), size_(0)
{
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other)
{
    copyFrom(other);
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    free();
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template <typename T>
size_t LinkedList<T>::getSize() const
{
    return this->size_;
}

template <typename T>
T& LinkedList<T>::operator[](size_t index)
{
    indexCheckError(index, size_);
    return getValueAt(index);
}

template <typename T>
const T& LinkedList<T>::operator[](size_t index) const
{
    indexCheckError(index, size_);
    return getValueAt(index);
}

// Clears list
template <typename T>
void LinkedList<T>::clear()
{
    free();
}

// Adds a new Node to the end of the list
template <typename T>
void LinkedList<T>::add(const T& value)
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
template <typename T>
void LinkedList<T>::insertAt(size_t index, const T& value)
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
template <typename T>
void LinkedList<T>::removeAt(size_t index)
{
    indexCheckError(index, size_);
    if (index == 0) // Similar functionality as pop
        delete popNode();
    else if (index == size_ - 1) // Special case for end of list
        delete popNodeBack();
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
template <typename T>
void LinkedList<T>::pop()
{
    emptyCheckError(head_);
    removeAt(0);
}

// Returns mutable value from head of the list
template <typename T>
T& LinkedList<T>::peek()
{
    emptyCheckError(head_);
    return getValueAt(0);
}

// Returns a read-only value from head of the list
template <typename T>
const T& LinkedList<T>::peek() const
{
    emptyCheckError(head_);
    return getValueAt(0);
}

// Pops an item from the tail of the list
template <typename T>
void LinkedList<T>::popBack()
{
    emptyCheckError(tail_);
    removeAt(size_ - 1);
}

// Returns mutable value from head of the list
template <typename T>
T& LinkedList<T>::peekBack()
{
    emptyCheckError(tail_);
    return getValueAt(size_ - 1);
}

// Returns a read-only value from tail of the list
template <typename T>
const T& LinkedList<T>::peekBack() const
{
    emptyCheckError(tail_);
    return getValueAt(size_ - 1);
}

// Serializes a list into a binary file
template <typename T>
void LinkedList<T>::serialize(std::ofstream& ofs) const
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
template <typename T>
void LinkedList<T>::deserialize(std::ifstream& ifs)
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
