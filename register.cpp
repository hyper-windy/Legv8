// Register: no idea
#pragma once
#include<iostream>
using namespace std; 
class Register
    {
    private:
        long _Register[32];

    public:
        long Get(int i) { return _Register[i]; }
        void Set(int i, long value)
        {
            _Register[i] = value;
        }
        void log(int bytes = 40) {} // TODO: add register log
    };
