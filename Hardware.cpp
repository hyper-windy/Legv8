#pragma once

#include <iostream>
#include <map>
using namespace std;

#include "memory.cpp"
#include "memory.cpp"
#include "register.cpp"

class Hardware
{
private:
    Register *_reg;
    Memory *_mem;
    map<string, int> _data;

public:
    int PC;

    Hardware(size_t memsize)
    {
        _reg = new Register();
        _mem = new Memory(memsize);
        PC = 0;
    }
    ~Hardware() {} // TODO: cleanup

    long GetRegister(string reg_name);

    void SetRegister(string reg_name, long value);

    void pushData(string data, int &addr);

    void set(int index, void *source, size_t size) // TODO: checking before write
    {
        _mem->set(index, source, size);
    }

    void log(int bytes = 40);
};


long Hardware::GetRegister(string reg_name) {
    stringstream ss(reg_name.substr(1, reg_name.length())); 
    int i; 
    ss>>i; 
    return _reg->Get(i); 
}

void Hardware::SetRegister(string reg_name, long value) {
    stringstream ss(reg_name.substr(1, reg_name.length())); 
    int i; 
    ss>>i; 
    _reg->Set(i, value);
}

void Hardware::pushData(string data, int &addr)
{
    stringstream ss(data); 
    string data_name;
    ss>>data_name; 
    _data[data_name.substr(0, data_name.length()-1)] = addr;
    _mem->loadVariable(data.substr(data_name.length()+1, data.length()), addr);
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