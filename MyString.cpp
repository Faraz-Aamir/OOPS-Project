// MyString.cpp — Custom string wrapper implementation
// Replaces std::string — all text operations via char arrays
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "MyString.h"
#include <cctype>

// ============================================================
// PRIVATE HELPERS
// ============================================================

void MyString::ensureCapacity(int newCap) {
    // Only reallocate if we need more space
    if (newCap + 1 <= capacity) return;

    // Grow by doubling or to newCap+1, whichever is larger
    int newAllocSize = (capacity * 2 > newCap + 1) ? capacity * 2 : newCap + 1;
    char* newData = new char[newAllocSize];

    // Copy existing content
    if (data) {
        for (int i = 0; i <= len; i++) { // include null terminator
            newData[i] = data[i];
        }
        delete[] data;
    } else {
        newData[0] = '\0';
    }

    data = newData;
    capacity = newAllocSize;
}

// ============================================================
// CONSTRUCTORS & DESTRUCTOR
// ============================================================

MyString::MyString() : data(nullptr), len(0), capacity(0) {
    data = new char[1];
    data[0] = '\0';
    capacity = 1;
}

MyString::MyString(const char* str) : data(nullptr), len(0), capacity(0) {
    if (str) {
        len = 0;
        while (str[len] != '\0') len++;
        capacity = len + 1;
        data = new char[capacity];
        for (int i = 0; i <= len; i++) {
            data[i] = str[i];
        }
    } else {
        data = new char[1];
        data[0] = '\0';
        capacity = 1;
    }
}

MyString::MyString(const MyString& other) : data(nullptr), len(other.len), capacity(other.len + 1) {
    data = new char[capacity];
    for (int i = 0; i <= len; i++) {
        data[i] = other.data[i];
    }
}

MyString::MyString(char c) : data(nullptr), len(1), capacity(2) {
    data = new char[2];
    data[0] = c;
    data[1] = '\0';
}

MyString::~MyString() {
    delete[] data;
    data = nullptr;
}

// ============================================================
// ASSIGNMENT
// ============================================================

MyString& MyString::operator=(const MyString& other) {
    if (this == &other) return *this;

    delete[] data;
    len = other.len;
    capacity = len + 1;
    data = new char[capacity];
    for (int i = 0; i <= len; i++) {
        data[i] = other.data[i];
    }
    return *this;
}

MyString& MyString::operator=(const char* str) {
    delete[] data;
    if (str) {
        len = 0;
        while (str[len] != '\0') len++;
        capacity = len + 1;
        data = new char[capacity];
        for (int i = 0; i <= len; i++) {
            data[i] = str[i];
        }
    } else {
        len = 0;
        capacity = 1;
        data = new char[1];
        data[0] = '\0';
    }
    return *this;
}

// ============================================================
// ELEMENT ACCESS
// ============================================================

char& MyString::operator[](int index) {
    return data[index];
}

const char& MyString::operator[](int index) const {
    return data[index];
}

char MyString::at(int index) const {
    if (index < 0 || index >= len) {
        std::cerr << "MyString::at() — index " << index << " out of range [0, " << len << ")" << std::endl;
        return data[0]; // fallback
    }
    return data[index];
}

// ============================================================
// CAPACITY
// ============================================================

int MyString::length() const { return len; }
int MyString::size() const { return len; }
bool MyString::empty() const { return len == 0; }

void MyString::clear() {
    delete[] data;
    data = new char[1];
    data[0] = '\0';
    len = 0;
    capacity = 1;
}

void MyString::reserve(int newCap) {
    ensureCapacity(newCap);
}

// ============================================================
// MODIFIERS
// ============================================================

void MyString::append(const MyString& other) {
    if (other.len == 0) return;
    ensureCapacity(len + other.len);
    for (int i = 0; i <= other.len; i++) {
        data[len + i] = other.data[i];
    }
    len += other.len;
}

void MyString::append(const char* str) {
    if (!str) return;
    int slen = 0;
    while (str[slen] != '\0') slen++;
    if (slen == 0) return;
    ensureCapacity(len + slen);
    for (int i = 0; i <= slen; i++) {
        data[len + i] = str[i];
    }
    len += slen;
}

void MyString::append(char c) {
    ensureCapacity(len + 1);
    data[len] = c;
    data[len + 1] = '\0';
    len++;
}

MyString& MyString::operator+=(const MyString& other) {
    append(other);
    return *this;
}

MyString& MyString::operator+=(const char* str) {
    append(str);
    return *this;
}

MyString& MyString::operator+=(char c) {
    append(c);
    return *this;
}

// ============================================================
// STRING OPERATIONS
// ============================================================

int MyString::find(const MyString& substr, int startPos) const {
    return find(substr.data, startPos);
}

int MyString::find(const char* substr, int startPos) const {
    if (!substr) return -1;
    int slen = 0;
    while (substr[slen] != '\0') slen++;
    if (slen == 0) return startPos;
    if (startPos + slen > len) return -1;

    for (int i = startPos; i <= len - slen; i++) {
        bool match = true;
        for (int j = 0; j < slen; j++) {
            if (data[i + j] != substr[j]) {
                match = false;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

int MyString::find(char c, int startPos) const {
    for (int i = startPos; i < len; i++) {
        if (data[i] == c) return i;
    }
    return -1;
}

MyString MyString::substr(int pos, int count) const {
    if (pos < 0 || pos >= len) return MyString();
    if (count < 0 || pos + count > len) count = len - pos;

    char* buf = new char[count + 1];
    for (int i = 0; i < count; i++) {
        buf[i] = data[pos + i];
    }
    buf[count] = '\0';
    MyString result(buf);
    delete[] buf;
    return result;
}

bool MyString::contains(const MyString& substr) const {
    return find(substr) != -1;
}

bool MyString::contains(const char* substr) const {
    return find(substr) != -1;
}

// ============================================================
// COMPARISON
// ============================================================

bool MyString::operator==(const MyString& other) const {
    if (len != other.len) return false;
    for (int i = 0; i < len; i++) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}

bool MyString::operator==(const char* str) const {
    if (!str) return len == 0;
    int slen = 0;
    while (str[slen] != '\0') slen++;
    if (len != slen) return false;
    for (int i = 0; i < len; i++) {
        if (data[i] != str[i]) return false;
    }
    return true;
}

bool MyString::operator!=(const MyString& other) const { return !(*this == other); }
bool MyString::operator!=(const char* str) const { return !(*this == str); }

bool MyString::operator<(const MyString& other) const {
    int minLen = (len < other.len) ? len : other.len;
    for (int i = 0; i < minLen; i++) {
        if (data[i] < other.data[i]) return true;
        if (data[i] > other.data[i]) return false;
    }
    return len < other.len;
}

bool MyString::operator>(const MyString& other) const { return other < *this; }
bool MyString::operator<=(const MyString& other) const { return !(other < *this); }
bool MyString::operator>=(const MyString& other) const { return !(*this < other); }

// ============================================================
// CONCATENATION
// ============================================================

MyString MyString::operator+(const MyString& other) const {
    MyString result(*this);
    result.append(other);
    return result;
}

MyString MyString::operator+(const char* str) const {
    MyString result(*this);
    result.append(str);
    return result;
}

MyString operator+(const char* lhs, const MyString& rhs) {
    MyString result(lhs);
    result.append(rhs);
    return result;
}

// ============================================================
// CONVERSION
// ============================================================

const char* MyString::c_str() const {
    return data;
}

int MyString::toInt() const {
    return atoi(data);
}

MyString MyString::fromInt(int value) {
    char buf[32];
    int i = 0;
    bool negative = false;

    if (value == 0) return MyString("0");
    if (value < 0) {
        negative = true;
        value = -value;
    }

    while (value > 0 && i < 30) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }
    if (negative) buf[i++] = '-';
    buf[i] = '\0';

    // Reverse the buffer
    for (int j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }

    return MyString(buf);
}

MyString MyString::fromLong(long value) {
    char buf[32];
    int i = 0;
    bool negative = false;

    if (value == 0) return MyString("0");
    if (value < 0) {
        negative = true;
        value = -value;
    }

    while (value > 0 && i < 30) {
        buf[i++] = '0' + (int)(value % 10);
        value /= 10;
    }
    if (negative) buf[i++] = '-';
    buf[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }

    return MyString(buf);
}

MyString MyString::fromSizeT(size_t value) {
    char buf[32];
    int i = 0;

    if (value == 0) return MyString("0");

    while (value > 0 && i < 30) {
        buf[i++] = '0' + (int)(value % 10);
        value /= 10;
    }
    buf[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }

    return MyString(buf);
}

// ============================================================
// UTILITY
// ============================================================

MyString MyString::toLower() const {
    MyString result(*this);
    for (int i = 0; i < result.len; i++) {
        if (result.data[i] >= 'A' && result.data[i] <= 'Z') {
            result.data[i] = result.data[i] + ('a' - 'A');
        }
    }
    return result;
}

MyString MyString::toUpper() const {
    MyString result(*this);
    for (int i = 0; i < result.len; i++) {
        if (result.data[i] >= 'a' && result.data[i] <= 'z') {
            result.data[i] = result.data[i] - ('a' - 'A');
        }
    }
    return result;
}

MyString MyString::trim() const {
    int start = 0, end = len - 1;
    while (start < len && (data[start] == ' ' || data[start] == '\t' ||
                           data[start] == '\n' || data[start] == '\r')) {
        start++;
    }
    while (end > start && (data[end] == ' ' || data[end] == '\t' ||
                           data[end] == '\n' || data[end] == '\r')) {
        end--;
    }
    return substr(start, end - start + 1);
}

bool MyString::startsWith(const MyString& prefix) const {
    if (prefix.len > len) return false;
    for (int i = 0; i < prefix.len; i++) {
        if (data[i] != prefix.data[i]) return false;
    }
    return true;
}

bool MyString::endsWith(const MyString& suffix) const {
    if (suffix.len > len) return false;
    int offset = len - suffix.len;
    for (int i = 0; i < suffix.len; i++) {
        if (data[offset + i] != suffix.data[i]) return false;
    }
    return true;
}

void MyString::replace(const MyString& oldStr, const MyString& newStr) {
    if (oldStr.empty()) return;

    // Build a new string with replacements
    MyString result;
    int i = 0;
    while (i < len) {
        int pos = find(oldStr, i);
        if (pos == -1) {
            // No more occurrences — append the rest
            result.append(substr(i, len - i));
            break;
        }
        // Append everything before the match
        if (pos > i) {
            result.append(substr(i, pos - i));
        }
        // Append replacement
        result.append(newStr);
        i = pos + oldStr.len;
    }

    *this = result;
}

int MyString::split(char delimiter, MyString* parts, int maxParts) const {
    int count = 0;
    int start = 0;

    for (int i = 0; i <= len && count < maxParts; i++) {
        if (i == len || data[i] == delimiter) {
            parts[count] = substr(start, i - start);
            count++;
            start = i + 1;
        }
    }
    return count;
}

// ============================================================
// I/O
// ============================================================

std::ostream& operator<<(std::ostream& os, const MyString& str) {
    if (str.data) os << str.data;
    return os;
}

std::istream& operator>>(std::istream& is, MyString& str) {
    str.clear();
    char c;
    // Skip leading whitespace
    while (is.get(c) && (c == ' ' || c == '\t' || c == '\n' || c == '\r'));

    if (is) {
        str.append(c);
        while (is.get(c) && c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            str.append(c);
        }
    }
    return is;
}

MyString MyString::getLine(std::istream& is) {
    MyString result;
    char c;
    while (is.get(c) && c != '\n') {
        result.append(c);
    }
    return result;
}

// ============================================================
// HASHING
// ============================================================

unsigned int MyString::hash() const {
    // djb2 hash algorithm
    unsigned int h = 5381;
    for (int i = 0; i < len; i++) {
        h = ((h << 5) + h) + (unsigned char)data[i]; // h * 33 + c
    }
    return h;
}
