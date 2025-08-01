﻿#pragma once

template<class T>
class List<T*>
{
    T** data_;
    size_t size_;
    size_t capacity_;

    void reorder_left(size_t pos);
    void reorder_right(size_t pos);

    void copyFrom(const List& other);
    void moveFrom(List&& other);
    void free();
    void resize(size_t newSize);
    void resize();

public:
    List();
    List(const List& other);
    List& operator=(const List& other);
    ~List();

    List(List&& other) noexcept;
    List& operator=(List&& other) noexcept;

    const T* operator[](size_t index) const;
    T*& operator[](size_t index);

    size_t getSize() const;
    size_t getCapacity() const;

    void add(T* item);
    //void remove(const T& item);

    void insertAt(size_t index, T* item);
    void removeAt(size_t index);

    void pop();
    void clear();

    void append(const List& list);
    void reverse();

    void reserve(size_t newCapacity);
    void fitToSize();

    friend std::ostream& operator<<(std::ostream& os, const List& other)
    {
        for (size_t i = 0; i < other.size_; i++)
            os << *other[i] << '\n';

        return os;
    }
};

template <class T>
void List<T*>::reorder_left(size_t pos)
{
    for (size_t i = pos; i < size_ - 1; i++)
        data_[i] = data_[i + 1];

    data_[size_ - 1] = nullptr;
}

template <class T>
void List<T*>::reorder_right(size_t pos)
{
    for (size_t i = size_ - 1; i >= pos; i--)
        data_[i + 1] = data_[i];
}

template <class T>
void List<T*>::copyFrom(const List& other)
{
    for (size_t i = 0; i < other.size_; i++)
        data_[i] = other.data_[i];

    size_ = other.size_;
    capacity_ = other.capacity_;
}

template <class T>
void List<T*>::moveFrom(List&& other)
{
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <class T>
void List<T*>::free()
{
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <class T>
void List<T*>::resize(size_t newSize)
{
    T** newData = new T*[newSize]{nullptr};
    for (size_t i = 0; i < size_; i++)
        newData[i] = data_[i];

    free();
    
    data_ = newData;
    capacity_ = newSize;
}

template <class T>
void List<T*>::resize()
{
    resize(2 * capacity_);
}

template <class T>
List<T*>::List(): size_(0)
{
    resize(DEFAULT_LIST_CAPACITY);
}

template <class T>
List<T*>::List(const List& other)
{
    copyFrom(other);
}

template <class T>
List<T*>& List<T*>::operator=(const List& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

template <class T>
List<T*>::~List()
{
    free();
}

template <class T>
List<T*>::List(List&& other) noexcept
{
    moveFrom(std::move(other));
}

template <class T>
List<T*>& List<T*>::operator=(List&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template <class T>
const T* List<T*>::operator[](size_t index) const
{
    return data_[index];
}

template <class T>
T*& List<T*>::operator[](size_t index)
{
    return data_[index];
}

template <class T>
size_t List<T*>::getSize() const
{
    return size_;
}

template <class T>
size_t List<T*>::getCapacity() const
{
    return capacity_;
}

template <class T>
void List<T*>::add(T* item)
{
    if (size_ + 1 > capacity_)
        resize();

    data_[size_] = item;
    size_++;
}

template <class T>
void List<T*>::insertAt(size_t index, T* item)
{
    if (index >= size_)
        throw std::out_of_range("Index is out of range");
    
    if (size_ + 1 > capacity_)
        resize();

    reorder_right(index);
    data_[index] = item;
    size_++;
}

template <class T>
void List<T*>::removeAt(size_t index)
{
    if (index >= size_)
        throw std::out_of_range("Index is out of range");
    
    reorder_left(index);
    size_--;
}

template <class T>
void List<T*>::pop()
{
    if (size_ == 0)
        throw std::out_of_range("List is empty");
    
    reorder_left(0);
    size_--;
}

template <class T>
void List<T*>::clear()
{
    free();
    reserve(DEFAULT_LIST_CAPACITY);
}

template <class T>
void List<T*>::append(const List& list)
{
    for (size_t i = 0; i < list.size_; i++)
        this->add(list[i]);
}

template <class T>
void List<T*>::reverse()
{
    for (size_t i = 0; i < size_ / 2; i++)
    {
        T* temp = data_[i];
        data_[i] = data_[size_ - i - 1];
        data_[size_ - i - 1] = temp;
    }
}

template <class T>
void List<T*>::reserve(size_t newCapacity)
{
    resize(newCapacity);
}

template <class T>
void List<T*>::fitToSize()
{
    resize(size_);
}
