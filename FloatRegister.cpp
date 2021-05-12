// Register: no idea
#pragma once
#include <iostream>
using namespace std;
class FloatRegister
{
private:
    float _floatRegister[32];

public:
    FloatRegister()
    {
        for (int i = 0; i < 32; i++)
            _floatRegister[i] = 0.0;
    }
    float Get(int i)
    {
        return _floatRegister[i];
    }
    void Set(int i, float value)
    {
        if (i != 31)
            _floatRegister[i] = value;
        // cout << "Update: R" << i << " = " << Get(i) << endl; 
    }
    void log(int bytes = 40)
    {
        for (int i = 0; i < 32; i++)
            cout << "Register " << i << " : " << _floatRegister[i] << endl;
    }
};
