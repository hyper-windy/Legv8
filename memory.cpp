// Memory: not yet

#pragma once
#include <iostream>
#include <sstream>
#include <cstring> // cstring::memmove()
using namespace std;

class Memory
{
public:
    char *mem;
private:
    const size_t size;
    char *getAddress(int index);

public:
    Memory(size_t sizeInByte);
    ~Memory();
    void set(int index, void *source, size_t size);
    void *get(int index);
    void log(int bytes = 40);
    void loadVariable(string line, int &top);
};

Memory::Memory(size_t sizeInByte) : size(sizeInByte) { mem = new char[sizeInByte]; }

Memory::~Memory() { delete mem; }

char *Memory::getAddress(int index)
{
    return mem + index;
}

void Memory::set(int index, void *source, size_t size)
{
    char *dest = getAddress(index);
    memmove(dest, source, size);
}

void Memory::log(int bytes)
{
    int lines = min((int)size, bytes);
    char *readPtr = mem;
    for (int i = 0; i < lines; i++)
    {
        // cout << " | " << *readPtr;
        cout << " | " << (unsigned short)(*readPtr);
        readPtr++;
        cout << endl;
    }
}

void Memory::loadVariable(string raw, int &top)
{
    stringstream ss(raw);
    string header;
    ss >> header;

    bool success = true;

    while (success)
    {
        success = false;
        if (header == ".byte") 
        {
            char a;
            if (ss >> a)
            {
                success = true;
                set(top, &a, 1);
            }
            top += 1;
        }
        else if (header == ".half")
        {
            short a;
            if (ss >> a)
            {
                success = true;
                set(top, &a, 2);
            }
            top += 2;
        }
        else if (header == ".word")
        {
            int a;
            if (ss >> a)
            {
                success = true;
                set(top, &a, 4);
            }
            top += 4;
        }
        else if (header == ".long")
        {
            long a;
            if (ss >> a)
            {
                success = true;
                set(top, &a, 8);
            }
            top += 8;
        }
        else if (header == ".asciz") // TODO: tìm cách bắt skip character \n\t\0, ...
        {
            char a;
            if (ss >> a)
            {
                success = true;
                set(top, &a, 1);
                top += 1;
            }
            else
            {
                a = '\n';
                set(top, &a, 1);
                top += 1;
            }
        }
        else if (header == ".skip")
        {
            int space;
            ss >> space;
            top += space;
        }
        else // TODO: add ascii
            cout << "Invalid type: " << header << endl;
    }
}

/*
int main()
{
    Memory mem(4 * 10);

    string data[] = { ".word 10 21 22 23", ".asciz abc\n" };
    stringstream ss(data[1]);

    int N = sizeof(data) / sizeof(data[0]);
    int top = 0;
    for (int i = 0; i < N; i++)
        mem.loadVariable(data[i], top);

    mem.log();
    return 0;
}
// */