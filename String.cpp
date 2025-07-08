#include "String.h"
#include <algorithm>
#include <fstream>
#include "List.hpp"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

namespace
{
    size_t count_separators(const String& str, const String& separator)
    {
        size_t count = 0;
        for (size_t i = 0; i < str.length(); i++)
        {
            String temp = str.substr(i, i + separator.length());
            if (temp == separator)
                count++;
        }

        return count;
    }
}

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
        data_ = new char[capacity_]{'\0'};
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

String::String(char ch)
{
    capacity_ = 2;
    data_ = new char[capacity_]{ch, '\0'};
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

void String::clear()
{
    free();
    capacity_ = 1;
    data_ = new char[capacity_]{'\0'};
}

String String::toLower() const
{
    String temp(*this);
    for (size_t i = 0; i < temp.length(); i++)
        temp[i] = std::tolower(data_[i]);
    return temp;
}

String String::toUpper() const
{
    String temp(*this);
    for (size_t i = 0; i < temp.length(); i++)
        temp[i] = std::toupper(data_[i]);
    return temp;
}

size_t String::length() const
{
    return strlen(data_);
}

bool String::isEmpty() const
{
    return length() == 0;
}

String String::substr(size_t start, size_t end) const
{
    String substr;
    if (start > end || end > length())
        throw std::out_of_range("Arguments are out of range");

    for (size_t i = start; i < end; i++)
        substr += data_[i];

    return substr;
}

List<String> String::split(const String& separator) const
{
    const size_t check_lenght = length() - separator.length();
    const size_t separators_count = count_separators(*this, separator);

    List<String> result;
    size_t begin = 0;
    for (size_t i = 0; i < separators_count + 1; i++)
    {
        size_t end = length();
        for (size_t j = begin; j < check_lenght; j++)
        {
            String temp = substr(j, j + separator.length());
            if (temp == separator)
            {
                end = j;
                break;
            }
        }

        result.add(substr(begin, end));
        begin = end + separator.length();
    }
    return result;
}

void String::reverse() const
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

void String::serialize(std::ofstream& ofs) const
{
    size_t length = this->length();
    ofs.write((const char*)&length, sizeof(size_t));
    ofs.write(data_, length + 1);
}

void String::deserialize(std::ifstream& ifs)
{
    size_t length;
    ifs.read((char*)&length, sizeof(size_t));
    char* data = new char[length + 1];
    ifs.read(data, length + 1);
    *this = data;
    delete[] data;
}

String operator+(const String& left, const String& right)
{
    String str(left);
    return str += right;
}

bool operator==(const String& left, const String& right)
{
    if (left.length() != right.length())
        return false;

    size_t length = left.length();
    for (size_t i = 0; i < length; i++)
        if (left[i] != right[i])
            return false;

    return true;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
    return os << str.data_;
}

std::istream& operator>>(std::istream& is, String& str)
{
    String temp;
    if (is && (is.peek() == ' ' || is.peek() == '\n'))
        is.ignore();

    while (is && is.peek() != ' ' && is.peek() != '\n')
    {
        char ch = '\0';
        is.get(ch);
        temp += ch;
    }

    if (is.fail())
        temp.clear();

    str = temp;

    return is;
}

void String::getline(std::istream& is, String& str)
{
    String temp;
    while (is && is.peek() != '\n')
    {
        char ch = '\0';
        is.get(ch);
        temp += ch;
    }

    if (is.fail())
        temp.clear();

    str = temp;
}
