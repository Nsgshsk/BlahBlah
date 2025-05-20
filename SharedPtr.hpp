#pragma once
#include <stdexcept>

template <typename T>
class SharedPtr
{
    T* ptr;
    size_t* refCount;

    void copyFrom(const SharedPtr& other);
    void moveFrom(SharedPtr&& other) noexcept;
    void free();

public:
    SharedPtr();
    explicit SharedPtr(T* ptr);
    SharedPtr(const SharedPtr& other);
    SharedPtr& operator=(const SharedPtr& other);
    ~SharedPtr();

    SharedPtr(SharedPtr&& other) noexcept;
    SharedPtr& operator=(SharedPtr&& other) noexcept;

    const T* operator->() const;
    T* operator->();

    const T& operator*() const;
    T& operator*();

    bool operator==(const SharedPtr& other) const;
    bool operator==(const T* other) const;

    bool operator!=(const SharedPtr& other) const;
    bool operator!=(const T* other) const;
};

template <typename T>
void SharedPtr<T>::copyFrom(const SharedPtr& other)
{
    this->ptr = other.ptr;
    this->refCount = other.refCount;

    if (this->refCount)
        (*refCount)++;
}

template <typename T>
void SharedPtr<T>::moveFrom(SharedPtr&& other) noexcept
{
    this->ptr = other.ptr;
    this->refCount = other.refCount;

    other.ptr = nullptr;
    other.refCount = nullptr;
}

template <typename T>
void SharedPtr<T>::free()
{
    if (this->ptr = nullptr && this->refCount == nullptr)
        return;

    if (*refCount == 1)
    {
        delete this->ptr;
        delete this->refCount;
    }
    else
    {
        (*refCount)--;
    }
}

template <typename T>
SharedPtr<T>::SharedPtr() : ptr(nullptr), refCount(nullptr)
{
}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : ptr(ptr)
{
    if (this->ptr)
        this->refCount = new size_t(1);
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other)
{
    copyFrom(other);
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template <typename T>
const T* SharedPtr<T>::operator->() const
{
    return this->ptr;
}

template <typename T>
T* SharedPtr<T>::operator->()
{
    return this->ptr;
}

template <typename T>
const T& SharedPtr<T>::operator*() const
{
    if (!this->ptr)
        throw std::runtime_error("nullptr!");

    return *this->ptr;
}

template <typename T>
T& SharedPtr<T>::operator*()
{
    if (!this->ptr)
        throw std::runtime_error("nullptr!");

    return *this->ptr;
}

template <typename T>
bool SharedPtr<T>::operator==(const SharedPtr& other) const
{
    return this->ptr == other.ptr;
}

template <typename T>
bool SharedPtr<T>::operator==(const T* other) const
{
    return this->ptr == other;
}

template <typename T>
bool SharedPtr<T>::operator!=(const SharedPtr& other) const
{
    return !(*this == other);
}

template <typename T>
bool SharedPtr<T>::operator!=(const T* other) const
{
    return !(*this == other);
}

template <typename T>
SharedPtr<T>::~SharedPtr()
{
    free();
}
