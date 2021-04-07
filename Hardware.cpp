#pragma once

#include <iostream>
#include <map>
#include <limits>
using namespace std;

#include "memory.cpp"
#include "memory.cpp"
#include "register.cpp"

struct Flags
{
    void reset() { N = Z = V = C = false; }
    void setN(bool on) { N = on; }
    void setZ(bool on) { Z = on; }
    void setV(bool on) { V = on; }
    void setC(bool on) { C = on; }

    bool eq() { return Z; }
    bool ne() { return !Z; }
    bool lt() { return N != V; }
    bool le() { return Z || N != V; }
    bool gt() { return !Z && N == V; }
    bool ge() { return N == V; }
    bool hs() { return C == 1; }

    bool checkOverflow(long a, long b)
    {
        long result = a + b;
       // cout << "result: " << (int)result << endl;
        if (a > 0 && b > 0 && result < 0)
            return true;
        if (a < 0 && b < 0 && result > 0)
            return true;
        return false;
    }

    bool checkFlagCarry(long a, long b)
    {
        if ((std::numeric_limits<unsigned long>::max() - (unsigned long)a) < (unsigned long)b)
            return true;
        return false;
    }

private:
    bool N, Z, C, V;
};

class Hardware
{
public:
    Register *_reg;
    Memory *_mem;
    map<string, int> _data;
    Flags flags;
    int PC;

    Hardware(size_t memsize)
    {
        _reg = new Register();
        _mem = new Memory(memsize);
        PC = 0;
    }
    ~Hardware()
    {
        if (_reg != NULL)
            delete _reg;
        if (_mem != NULL)
            delete _mem;
    } // TODO: cleanup

    long GetRegister(string reg_name);

    void SetRegister(string reg_name, long value);

    void pushData(string data, int &addr);

    void set(int index, void *source, size_t size) // TODO: checking before write
    {
        _mem->set(index, source, size);
    }

    void log(int bytes = 40);
};

long Hardware::GetRegister(string reg_name)
{
    if (reg_name == "SP") return _reg->Get(28); 
    stringstream ss(reg_name.substr(1, reg_name.length()));
    int i;
    ss >> i;
    return _reg->Get(i);
}

void Hardware::SetRegister(string reg_name, long value)
{
    stringstream ss(reg_name.substr(1, reg_name.length()));
    int i;
    ss >> i;
    _reg->Set(i, value);
}

void Hardware::pushData(string data, int &addr)
{
    stringstream ss(data);
    string data_name;
    ss >> data_name;
    _data[data_name.substr(0, data_name.length() - 1)] = addr;
    _mem->loadVariable(data.substr(data_name.length() + 1, data.length()), addr);
}

void Hardware::log(int bytes)
{
    cout << "-------------MEMORY-------------\n";
    _mem->log(bytes);
    cout << "-------------REGISTER-------------\n";
    _reg->log();
}
/*
int main() {
    return 0; 
}
*/