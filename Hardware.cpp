#include <iostream>
#include <map>
#include "memory.cpp"
#include "register.cpp"
#include <string>
using namespace std;

class Hardware
{
private:
    Register *_reg;
    Memory *_mem;
    map<string, int> _data;

public:
    Hardware()
    {
        _reg = new Register();
        _mem = new Memory(10000);
    }
    ~Hardware() {}
    long GetRegister(string reg_name);

    void SetRegister(string reg_name, long value);

    void WriteData(string data[], int n);

    void set(int index, void *source, size_t size) // TODO: checking before write
    {
        _mem->set(index, source, size);
    }
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

void Hardware::WriteData(string data[], int n)
{
    int top = 0;
    for (int i = 0; i < n; i++)
    {
        stringstream ss(data[i]); 
        string data_name;
        ss>>data_name; 
        _data[data_name.substr(0, data_name.length()-1)] = top;
        _mem->loadVariable(data[i].substr(data_name.length()+1, data[i].length()), top);
    }
}
int main() {
    return 0; 
}