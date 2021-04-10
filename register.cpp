// Register: no idea
#pragma once
#include <iostream>
using namespace std;
class Register
{
private:
    long _Register[32];

public:
    Register()
    {
        for (int i = 0; i < 32; i++)
            _Register[i] = 0;
    }
    long Get(int i)
    {
        return _Register[i];
    }
    void Set(int i, long value)
    {
        if (i != 31)
            _Register[i] = value;
        // cout << "Update: R" << i << " = " << Get(i) << endl; 
    }
    void log(int bytes = 40)
    {
        for (int i = 0; i < 32; i++)
            cout << "Register " << i << " : " << _Register[i] << endl;
    }
};
