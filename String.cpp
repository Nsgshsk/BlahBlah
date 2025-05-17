#include "String.h"
#include <iostream>

void String::copyFrom(const char* str)
{
    if (str == nullptr)
    {
        capacity_ = 1;
        data_ = new char[capacity_]{'\0'};
    }
    else
    {
        capacity_ = strlen(str) + 1;
        data_ = new char[capacity_];
        strcpy_s(data_, capacity_, str);
    }
}

void String::copyFrom(const String& other)
{
    capacity_ = other.capacity_;
    data_ = new char[capacity_];
    strcpy_s(data_, capacity_, other.data_);
}

void String::moveFrom(String&& other)
{
    capacity_ = other.capacity_;
    data_ = other.data_;
    other.data_ = nullptr;
    other.capacity_ = 0;
}

void String::free()
{
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
}

void String::resize(size_t minSize)
{
    minSize = std::max(minSize, capacity_ * 2);
    char* newData = new char[minSize];
    strcpy_s(newData, minSize, data_);
    free();

    capacity_ = minSize;
    data_ = newData;
}

String::String()
{
    capacity_ = 1;
    data_ = new char[capacity_]{'\0'};
}

String::String(const char* str)
{
    copyFrom(str);
}

String::String(const String& other)
{
    copyFrom(other);
}

String& String::operator=(const String& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

String& String::operator=(const char* str)
{
    free();
    copyFrom(str);
    return *this;
}

String::~String()
{
    free();
}

String::String(String&& other) noexcept
{
    moveFrom(std::move(other));
}

String& String::operator=(String&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

const char& String::operator[](size_t index) const
{
    if (index >= capacity_)
        throw std::out_of_range("Index is out of range");

    return data_[index];
}

char& String::operator[](size_t index)
{
    if (index >= capacity_)
        throw std::out_of_range("Index is out of range");

    return data_[index];
}

size_t String::length() const
{
    return strlen(data_);
}

bool String::isEmpty() const
{
    return length() == 0;
}

void String::reverse()
{
    size_t length = this->length();
    for (size_t i = 0; i < length / 2; i++)
    {
        char temp = data_[i];
        data_[i] = data_[length - i - 1];
        data_[length - i - 1] = temp;
    }
}

const char* String::c_str() const
{
    return data_;
}

String& String::operator+=(const String& other)
{
    if (this->length() + other.length() >= capacity_)
        resize(this->length() + other.length() + 1);

    strcat_s(this->data_, capacity_, other.data_);
    return *this;
}

String operator+(const String& left, const String& right)
{
    String str(left);
    return str += right;
}

std::ostream& operator<<(std::ostream& os, const String& other)
{
    return os << other.data_;
}

std::istream& operator>>(std::istream& is, String& other)
{
    size_t length;
    is >> length;
    other.resize(length + 1);
    return is >> other.data_;
}
