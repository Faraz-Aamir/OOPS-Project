// ProblemBank.cpp — 50 beginner C++ problems across 5 difficulty levels
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "ProblemBank.h"
#include <cstdlib>
#include <ctime>

ProblemBank::ProblemBank(Volume* vfs) : vfs(vfs) {
    srand((unsigned)time(nullptr));
}

ProblemBank::~ProblemBank() {
    for (int i = 0; i < allProblems.size(); i++) delete allProblems[i];
}

static void addP(DynamicArray<Problem*>& all, int id, const char* title, const char* desc,
                 const char* constraints, int diff, const char* sIn, const char* sOut,
                 const char* tmpl, const char* ti1, const char* to1, const char* ti2, const char* to2) {
    Problem* p = new Problem(id, MyString(title), MyString(desc), MyString(constraints), diff,
                             MyString(sIn), MyString(sOut), MyString(tmpl));
    p->addTestCase(TestCase(MyString(ti1), MyString(to1)));
    p->addTestCase(TestCase(MyString(ti2), MyString(to2)));
    all.push_back(p);
}

void ProblemBank::seedProblems() {
    const char* tmpl = "#include<iostream>\nusing namespace std;\n{{USER_CODE}}\n";

    // ===== DIFFICULTY 1 (Easiest) — 10 problems =====
    addP(allProblems, 1, "Hello World", "Print 'Hello World' to the screen.", "None", 1,
         "", "Hello World", tmpl, "", "Hello World", "", "Hello World");
    addP(allProblems, 2, "Sum of Two", "Read two integers and print their sum.", "1<=a,b<=1000", 1,
         "3 5", "8", tmpl, "3 5\n", "8", "10 20\n", "30");
    addP(allProblems, 3, "Even or Odd", "Read an integer and print 'Even' or 'Odd'.", "1<=n<=1000", 1,
         "4", "Even", tmpl, "4\n", "Even", "7\n", "Odd");
    addP(allProblems, 4, "Multiply Two", "Read two integers and print their product.", "1<=a,b<=100", 1,
         "3 4", "12", tmpl, "3 4\n", "12", "5 6\n", "30");
    addP(allProblems, 5, "Larger of Two", "Read two integers and print the larger one.", "1<=a,b<=1000", 1,
         "5 3", "5", tmpl, "5 3\n", "5", "2 8\n", "8");
    addP(allProblems, 6, "Absolute Value", "Read an integer and print its absolute value.", "-1000<=n<=1000", 1,
         "-5", "5", tmpl, "-5\n", "5", "3\n", "3");
    addP(allProblems, 7, "Is Positive", "Print 'Yes' if input is positive, else 'No'.", "-1000<=n<=1000", 1,
         "5", "Yes", tmpl, "5\n", "Yes", "-3\n", "No");
    addP(allProblems, 8, "Difference", "Print the absolute difference of two integers.", "1<=a,b<=1000", 1,
         "10 3", "7", tmpl, "10 3\n", "7", "3 10\n", "7");
    addP(allProblems, 9, "Square", "Read an integer and print its square.", "1<=n<=100", 1,
         "5", "25", tmpl, "5\n", "25", "3\n", "9");
    addP(allProblems, 10, "Remainder", "Print the remainder when a is divided by b.", "1<=a,b<=1000", 1,
         "10 3", "1", tmpl, "10 3\n", "1", "15 4\n", "3");

    // ===== DIFFICULTY 2 — 10 problems =====
    addP(allProblems, 11, "Sum of N", "Read N then N integers, print their sum.", "1<=N<=100", 2,
         "3\n1 2 3", "6", tmpl, "3\n1 2 3\n", "6", "4\n10 20 30 40\n", "100");
    addP(allProblems, 12, "Factorial", "Print factorial of N.", "0<=N<=12", 2,
         "5", "120", tmpl, "5\n", "120", "0\n", "1");
    addP(allProblems, 13, "Power", "Print a^b (a raised to power b).", "1<=a<=10, 0<=b<=10", 2,
         "2 3", "8", tmpl, "2 3\n", "8", "3 4\n", "81");
    addP(allProblems, 14, "Count Digits", "Count the number of digits in N.", "1<=N<=1000000", 2,
         "12345", "5", tmpl, "12345\n", "5", "100\n", "3");
    addP(allProblems, 15, "Sum of Digits", "Print the sum of digits of N.", "1<=N<=1000000", 2,
         "123", "6", tmpl, "123\n", "6", "999\n", "27");
    addP(allProblems, 16, "Reverse Number", "Print the reverse of a number.", "1<=N<=100000", 2,
         "1234", "4321", tmpl, "1234\n", "4321", "500\n", "5");
    addP(allProblems, 17, "Is Palindrome Number", "Print 'Yes' if N reads same backward.", "1<=N<=100000", 2,
         "121", "Yes", tmpl, "121\n", "Yes", "123\n", "No");
    addP(allProblems, 18, "Min of Array", "Read N then N integers, print the minimum.", "1<=N<=100", 2,
         "4\n3 1 4 1", "1", tmpl, "4\n3 1 4 1\n", "1", "3\n10 5 8\n", "5");
    addP(allProblems, 19, "Max of Array", "Read N then N integers, print the maximum.", "1<=N<=100", 2,
         "4\n3 1 4 1", "4", tmpl, "4\n3 1 4 1\n", "4", "3\n10 5 8\n", "10");
    addP(allProblems, 20, "Count Even", "Read N integers, count how many are even.", "1<=N<=100", 2,
         "5\n1 2 3 4 5", "2", tmpl, "5\n1 2 3 4 5\n", "2", "3\n2 4 6\n", "3");

    // ===== DIFFICULTY 3 — 10 problems =====
    addP(allProblems, 21, "Fibonacci", "Print the Nth Fibonacci number (0-indexed).", "0<=N<=20", 3,
         "6", "8", tmpl, "6\n", "8", "10\n", "55");
    addP(allProblems, 22, "Prime Check", "Print 'Yes' if N is prime, else 'No'.", "2<=N<=10000", 3,
         "7", "Yes", tmpl, "7\n", "Yes", "4\n", "No");
    addP(allProblems, 23, "GCD", "Print the GCD of two numbers.", "1<=a,b<=10000", 3,
         "12 8", "4", tmpl, "12 8\n", "4", "15 25\n", "5");
    addP(allProblems, 24, "LCM", "Print the LCM of two numbers.", "1<=a,b<=1000", 3,
         "4 6", "12", tmpl, "4 6\n", "12", "3 5\n", "15");
    addP(allProblems, 25, "Count Primes", "Count primes from 2 to N.", "2<=N<=1000", 3,
         "10", "4", tmpl, "10\n", "4", "20\n", "8");
    addP(allProblems, 26, "Sort Array", "Read N integers, print them sorted ascending.", "1<=N<=100", 3,
         "5\n5 3 1 4 2", "1 2 3 4 5", tmpl, "5\n5 3 1 4 2\n", "1 2 3 4 5", "3\n3 1 2\n", "1 2 3");
    addP(allProblems, 27, "Binary Search", "Search for X in sorted array. Print index or -1.", "1<=N<=100", 3,
         "5\n1 2 3 4 5\n3", "2", tmpl, "5\n1 2 3 4 5\n3\n", "2", "5\n1 2 3 4 5\n6\n", "-1");
    addP(allProblems, 28, "Reverse Array", "Read N integers, print them reversed.", "1<=N<=100", 3,
         "5\n1 2 3 4 5", "5 4 3 2 1", tmpl, "5\n1 2 3 4 5\n", "5 4 3 2 1", "3\n10 20 30\n", "30 20 10");
    addP(allProblems, 29, "Second Largest", "Find second largest in array.", "2<=N<=100", 3,
         "5\n3 1 4 1 5", "4", tmpl, "5\n3 1 4 1 5\n", "4", "3\n10 20 30\n", "20");
    addP(allProblems, 30, "Remove Duplicates", "Print sorted unique elements.", "1<=N<=100", 3,
         "7\n1 2 2 3 3 3 4", "1 2 3 4", tmpl, "7\n1 2 2 3 3 3 4\n", "1 2 3 4", "5\n5 5 5 5 5\n", "5");

    // ===== DIFFICULTY 4 — 10 problems =====
    addP(allProblems, 31, "Matrix Sum", "Read 2x2 matrices A and B, print A+B.", "Elements<=100", 4,
         "1 2\n3 4\n5 6\n7 8", "6 8\n10 12", tmpl,
         "1 2\n3 4\n5 6\n7 8\n", "6 8\n10 12", "0 0\n0 0\n1 1\n1 1\n", "1 1\n1 1");
    addP(allProblems, 32, "Pattern Triangle", "Print a right triangle of * with N rows.", "1<=N<=10", 4,
         "3", "*\n**\n***", tmpl, "3\n", "*\n**\n***", "2\n", "*\n**");
    addP(allProblems, 33, "String Length", "Read a word, print its length.", "len<=100", 4,
         "hello", "5", tmpl, "hello\n", "5", "world\n", "5");
    addP(allProblems, 34, "Count Vowels", "Count vowels in a word (lowercase).", "len<=100", 4,
         "hello", "2", tmpl, "hello\n", "2", "aeiou\n", "5");
    addP(allProblems, 35, "Palindrome String", "Check if a word is a palindrome.", "len<=100", 4,
         "racecar", "Yes", tmpl, "racecar\n", "Yes", "hello\n", "No");
    addP(allProblems, 36, "Power of Two", "Check if N is a power of 2.", "1<=N<=1000000", 4,
         "16", "Yes", tmpl, "16\n", "Yes", "12\n", "No");
    addP(allProblems, 37, "Perfect Number", "Check if N equals sum of its proper divisors.", "1<=N<=10000", 4,
         "6", "Yes", tmpl, "6\n", "Yes", "10\n", "No");
    addP(allProblems, 38, "Merge Sorted", "Merge two sorted arrays into one sorted array.", "1<=N,M<=50", 4,
         "3\n1 3 5\n3\n2 4 6", "1 2 3 4 5 6", tmpl,
         "3\n1 3 5\n3\n2 4 6\n", "1 2 3 4 5 6", "2\n1 4\n2\n2 3\n", "1 2 3 4");
    addP(allProblems, 39, "Rotate Array", "Rotate array left by K positions.", "1<=N<=100", 4,
         "5\n1 2 3 4 5\n2", "3 4 5 1 2", tmpl,
         "5\n1 2 3 4 5\n2\n", "3 4 5 1 2", "4\n10 20 30 40\n1\n", "20 30 40 10");
    addP(allProblems, 40, "Spiral Order", "Print elements of 2x2 matrix in spiral order.", "N=2", 4,
         "1 2\n3 4", "1 2 4 3", tmpl, "1 2\n3 4\n", "1 2 4 3", "5 6\n7 8\n", "5 6 8 7");

    // ===== DIFFICULTY 5 (Hardest) — 10 problems =====
    addP(allProblems, 41, "Tower of Hanoi", "Print number of moves for N disks.", "1<=N<=20", 5,
         "3", "7", tmpl, "3\n", "7", "4\n", "15");
    addP(allProblems, 42, "Longest Increasing", "Length of longest increasing subsequence.", "1<=N<=100", 5,
         "6\n10 9 2 5 3 7", "3", tmpl, "6\n10 9 2 5 3 7\n", "3", "5\n1 2 3 4 5\n", "5");
    addP(allProblems, 43, "Matrix Multiply", "Multiply two 2x2 matrices.", "Elements<=100", 5,
         "1 2\n3 4\n5 6\n7 8", "19 22\n43 50", tmpl,
         "1 2\n3 4\n5 6\n7 8\n", "19 22\n43 50", "1 0\n0 1\n1 0\n0 1\n", "1 0\n0 1");
    addP(allProblems, 44, "Roman to Int", "Convert Roman numeral to integer. (I,V,X,L,C,D,M)", "len<=15", 5,
         "XIV", "14", tmpl, "XIV\n", "14", "IX\n", "9");
    addP(allProblems, 45, "Valid Parentheses", "Check if brackets are balanced. Print Yes/No.", "len<=100", 5,
         "(())", "Yes", tmpl, "(())\n", "Yes", "(()\n", "No");
    addP(allProblems, 46, "Next Permutation", "Print next lexicographic permutation.", "1<=N<=8", 5,
         "3\n1 2 3", "1 3 2", tmpl, "3\n1 2 3\n", "1 3 2", "3\n3 2 1\n", "1 2 3");
    addP(allProblems, 47, "Kadane Max Subarray", "Find maximum subarray sum.", "1<=N<=100", 5,
         "8\n-2 1 -3 4 -1 2 1 -5", "6", tmpl,
         "8\n-2 1 -3 4 -1 2 1 -5\n", "6", "5\n1 2 3 4 5\n", "15");
    addP(allProblems, 48, "Coin Change", "Min coins to make amount. Coins: 1,5,10,25.", "1<=amt<=100", 5,
         "30", "2", tmpl, "30\n", "2", "11\n", "2");
    addP(allProblems, 49, "Josephus Problem", "Find survivor position (0-indexed) with k=2.", "1<=N<=100", 5,
         "5", "2", tmpl, "5\n", "2", "7\n", "6");
    addP(allProblems, 50, "N-Queens Count", "Count solutions to N-Queens for small N.", "1<=N<=8", 5,
         "4", "2", tmpl, "4\n", "2", "8\n", "92");

    // Seed into VFS
    User* sysUser = vfs->findUser("system");
    if (!sysUser) {
        sysUser = vfs->addUser("system", 0, "/system", UserRole::SUPERUSER);
    }
    if (sysUser) {
        for (int i = 0; i < allProblems.size(); i++) {
            MyString path = MyString("system/problem_bank/difficulty_")
                          + MyString::fromInt(allProblems[i]->getDifficulty())
                          + "/problem_" + MyString::fromInt(allProblems[i]->getProblemId()) + ".dat";
            vfs->writeToVFS(path, allProblems[i]->serialize(), sysUser);
        }
    }
    std::cout << "  Seeded " << allProblems.size() << " problems into VFS." << std::endl;
}

void ProblemBank::loadFromVFS() {
    // Problems are already in memory after seeding; this would reload from VFS on restart
}

DynamicArray<Problem*> ProblemBank::getByDifficulty(int difficulty) const {
    DynamicArray<Problem*> result;
    for (int i = 0; i < allProblems.size(); i++) {
        if (allProblems[i]->getDifficulty() == difficulty) result.push_back(allProblems[i]);
    }
    return result;
}

DynamicArray<Problem*> ProblemBank::getRandomByDifficulty(int difficulty, int count) const {
    DynamicArray<Problem*> pool = getByDifficulty(difficulty);
    DynamicArray<Problem*> result;
    if (pool.size() == 0 || count <= 0) return result;

    // Simple Fisher-Yates partial shuffle
    DynamicArray<int> indices;
    for (int i = 0; i < pool.size(); i++) indices.push_back(i);

    int toSelect = (count < pool.size()) ? count : pool.size();
    for (int i = 0; i < toSelect; i++) {
        int j = i + (rand() % (indices.size() - i));
        // Swap
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
        result.push_back(pool[indices[i]]);
    }
    return result;
}

const DynamicArray<Problem*>& ProblemBank::getAllProblems() const { return allProblems; }

Problem* ProblemBank::getProblemById(int id) const {
    for (int i = 0; i < allProblems.size(); i++) {
        if (allProblems[i]->getProblemId() == id) return allProblems[i];
    }
    return nullptr;
}

int ProblemBank::getTotalCount() const { return allProblems.size(); }

int ProblemBank::getCountByDifficulty(int difficulty) const {
    return getByDifficulty(difficulty).size();
}
