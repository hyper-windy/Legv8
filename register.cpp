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
        void log(int bytes = 40) {
            for(int i = 0; i<32; i++) 
            cout<<"Register " << i << " : " << _Register[i] << endl; 
        } // TODO: add register log
    };
