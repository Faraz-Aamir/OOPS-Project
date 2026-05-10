// MyString.h — Custom string wrapper using char arrays (replaces std::string)
// No STL containers or std::string used anywhere
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>
#include <cstring>
#include <cstdlib>

class MyString {
private:
    char* data;       // Dynamically allocated character array
    int len;          // Current length (excluding null terminator)
    int capacity;     // Allocated capacity (including null terminator)

    // Helper: ensure capacity for at least 'newCap' characters (+ null)
    void ensureCapacity(int newCap);

public:
    // --- Constructors & Destructor ---
    MyString();                              // Default: empty string
    MyString(const char* str);               // From C-string
    MyString(const MyString& other);         // Copy constructor
    MyString(char c);                        // From single char
    ~MyString();                             // Destructor — frees memory

    // --- Assignment ---
    MyString& operator=(const MyString& other);
    MyString& operator=(const char* str);

    // --- Element Access ---
    char& operator[](int index);
    const char& operator[](int index) const;
    char at(int index) const;

    // --- Capacity ---
    int length() const;
    int size() const;
    bool empty() const;
    void clear();
    void reserve(int newCap);

    // --- Modifiers ---
    void append(const MyString& other);
    void append(const char* str);
    void append(char c);
    MyString& operator+=(const MyString& other);
    MyString& operator+=(const char* str);
    MyString& operator+=(char c);

    // --- String Operations ---
    int find(const MyString& substr, int startPos = 0) const;
    int find(const char* substr, int startPos = 0) const;
    int find(char c, int startPos = 0) const;
    MyString substr(int pos, int count = -1) const;
    bool contains(const MyString& substr) const;
    bool contains(const char* substr) const;

    // --- Comparison ---
    bool operator==(const MyString& other) const;
    bool operator==(const char* str) const;
    bool operator!=(const MyString& other) const;
    bool operator!=(const char* str) const;
    bool operator<(const MyString& other) const;
    bool operator>(const MyString& other) const;
    bool operator<=(const MyString& other) const;
    bool operator>=(const MyString& other) const;

    // --- Concatenation ---
    MyString operator+(const MyString& other) const;
    MyString operator+(const char* str) const;

    // --- Conversion ---
    const char* c_str() const;
    int toInt() const;
    static MyString fromInt(int value);
    static MyString fromLong(long value);
    static MyString fromSizeT(size_t value);

    // --- Utility ---
    MyString toLower() const;
    MyString toUpper() const;
    MyString trim() const;
    bool startsWith(const MyString& prefix) const;
    bool endsWith(const MyString& suffix) const;
    void replace(const MyString& oldStr, const MyString& newStr);

    // Split string by delimiter — returns count, fills 'parts' array (caller provides)
    int split(char delimiter, MyString* parts, int maxParts) const;

    // --- I/O ---
    friend std::ostream& operator<<(std::ostream& os, const MyString& str);
    friend std::istream& operator>>(std::istream& is, MyString& str);

    // Read an entire line from input stream
    static MyString getLine(std::istream& is);

    // --- Hashing (for HashMap) ---
    unsigned int hash() const;
};

// Free-standing concatenation: const char* + MyString
MyString operator+(const char* lhs, const MyString& rhs);

#endif
