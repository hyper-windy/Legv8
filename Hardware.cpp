#pragma once

#include <iostream>
#include <map>
#include <limits>
using namespace std;

#include "memory.cpp"
#include "memory.cpp"
#include "register.cpp"
#include "FloatRegister.cpp"

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
    FloatRegister *_floatReg;
    Memory *_mem;
    map<string, int> _data;
    Flags flags;
    int PC;

    Hardware(size_t memsize)
    {
        _reg = new Register();
        _floatReg = new FloatRegister();
        _mem = new Memory(memsize);
        PC = 0;
    }
    ~Hardware()
    {
        delete _reg;
        delete _mem;
    }

    long GetRegister(string reg_name) { return _reg->Get(indexOf(reg_name)); }
    void SetRegister(string reg_name, long value) { _reg->Set(indexOf(reg_name), value); }

    //update for float Register
    long getFloatRegister(string reg_name) {return _floatReg->Get(indexOf(reg_name)) ;}
    void setFloatRegister(string reg_name, float value) {_floatReg->Set(indexOf(reg_name), value);}

    void pushData(string data, int &addr);
    void set(int index, void *source, size_t size) { _mem->set(index, source, size); }

    void log(int bytes = 40);

private:
    int indexOf(string reg_name);
};

// TODO: init value for SP

void Hardware::pushData(string data, int &addr)
{
    stringstream ss(data);
    string data_name;
    ss >> data_name;
    _data[data_name] = addr;
    _mem->loadVariable(data.substr(data_name.length() + 1, data.length()), addr);
}

void Hardware::log(int bytes)
{
    cout << "-------------MEMORY-------------\n";
    _mem->log(bytes);
    cout << "-------------REGISTER-------------\n";
    _reg->log();
    //update float Register
    cout << "-------------FLOAT REGISTER-------------\n";
    _floatReg->log(); 
}

int Hardware::indexOf(string reg_name)
{
    if (reg_name == "SP") // stack pointer
        return 28;
    else if (reg_name == "FP") // frame pointer
        return 29;
    else if (reg_name == "LR") // link register
        return 30;
    else if (reg_name == "XZR") // zero register
        return 31;
    else
    {
        stringstream ss(reg_name.substr(1, reg_name.length()));
        int i = -1;
        ss >> i;
        return i;
    }
}