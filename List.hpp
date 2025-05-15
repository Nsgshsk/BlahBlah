#pragma once
#include <iostream>

constexpr uint8_t DEFAULT_LIST_CAPACITY = 16;

template<class T>
class List
{
    T** data_;
    size_t size_;
    size_t capacity_;

    static void reorder_left(T** arr, size_t size, size_t pos);
    static void reorder_right(T** arr, size_t size, size_t pos);

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

    const T& operator[](size_t index) const;
    T& operator[](size_t index);

    size_t getSize() const;
    size_t getCapacity() const;

    void add(const T& item);
    //void remove(const T& item);

    void insertAt(size_t index, const T& item);
    void removeAt(size_t index);

    void pop();
    void clear();

    void append(const List<T*>& list);
    void append(List<T*>&& list) noexcept;
    
    void reverse();

    void reserve(size_t newCapacity);
    void fitToSize();

    friend std::ostream& operator<<(std::ostream& os, const List& other)
    {
        for (size_t i = 0; i < other.size_; i++)
            os << other[i] << '\n';

        return os;
    }
};

template <class T>
void List<T>::reorder_left(T** arr, size_t size, size_t pos)
{
    for (size_t i = pos; i < size - 1; i++)
        arr[i] = arr[i + 1];

    arr[size - 1] = nullptr;
}

template <class T>
void List<T>::reorder_right(T** arr, size_t size, size_t pos)
{
    for (size_t i = size - 1; i >= pos; i--)
        arr[i + 1] = arr[i];
}

template <class T>
void List<T>::copyFrom(const List& other)
{
    for (size_t i = 0; i < other.size_; i++)
        data_[i] = new T(other[i]);

    size_ = other.size_;
    capacity_ = other.capacity_;
}

template <class T>
void List<T>::moveFrom(List&& other)
{
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <class T>
void List<T>::free()
{
    for (size_t i = 0; i < capacity_; i++)
        if (data_[i] != nullptr)
            delete data_[i];
    
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <class T>
void List<T>::resize(size_t newSize)
{
    T** newData = new T*[newSize]{nullptr};
    for (size_t i = 0; i < size_; i++)
        newData[i] = data_[i];

    if (data_ != nullptr)
        delete[] data_;
    
    data_ = newData;
    capacity_ = newSize;
}

template <class T>
void List<T>::resize()
{
    resize(2 * capacity_);
}

template <class T>
List<T>::List() : size_(0), capacity_(DEFAULT_LIST_CAPACITY)
{
    data_ = new T*[DEFAULT_LIST_CAPACITY]{nullptr};
}

template <class T>
List<T>::List(const List& other)
{
    copyFrom(other);
}

template <class T>
List<T>& List<T>::operator=(const List& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

template <class T>
List<T>::~List()
{
    free();
}

template <class T>
List<T>::List(List&& other) noexcept
{
    moveFrom(std::move(other));
}

template <class T>
List<T>& List<T>::operator=(List&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template <class T>
const T& List<T>::operator[](size_t index) const
{
    return *data_[index];
}

template <class T>
T& List<T>::operator[](size_t index)
{
    return *data_[index];
}

template <class T>
size_t List<T>::getSize() const
{
    return size_;
}

template <class T>
size_t List<T>::getCapacity() const
{
    return capacity_;
}

template <class T>
void List<T>::add(const T& item)
{
    if (size_ + 1 > capacity_)
        resize();

    data_[size_] = new T(item);
    size_++;
}

template <class T>
void List<T>::insertAt(size_t index, const T& item)
{
    if (index >= size_)
        throw std::out_of_range("Index is out of range");
    
    if (size_ + 1 > capacity_)
        resize();

    reorder_right(data_, size_, index);
    data_[index] = new T(item);
    size_++;
}

template <class T>
void List<T>::removeAt(size_t index)
{
    if (index >= size_)
        throw std::out_of_range("Index is out of range");

    delete data_[index];
    reorder_left(data_, size_, index);
    size_--;
}

template <class T>
void List<T>::pop()
{
    if (size_ == 0)
        throw std::out_of_range("List is empty");

    delete data_[0];
    reorder_left(data_, size_, 0);
    size_--;
}

template <class T>
void List<T>::clear()
{
    free();
    reserve(DEFAULT_LIST_CAPACITY);
}

template <class T>
void List<T>::append(const List<T*>& list)
{
    for (size_t i = 0; i < list.size_; i++)
        this->add(list[i]);
}

template <class T>
void List<T>::append(List<T*>&& list) noexcept
{
    if (this == &list)
    {
        append(list);
        return;
    }
    
    while (size_ + list.size_ > capacity_)
        resize();
    
    for (size_t i = 0; i < list.size_; i++)
    {
        data_[size_ + i] = list.data_[i];
        list.data_[i] = nullptr;
    }
}

template <class T>
void List<T>::reverse()
{
    for (size_t i = 0; i < size_ / 2; i++)
    {
        T* temp = data_[i];
        data_[i] = data_[size_ - i - 1];
        data_[size_ - i - 1] = temp;
    }
}

template <class T>
void List<T>::reserve(size_t newCapacity)
{
    resize(newCapacity);
}

template <class T>
void List<T>::fitToSize()
{
    resize(size_);
}
