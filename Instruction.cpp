#pragma once
#include "utils.cpp"
#include "Hardware.cpp"
#include <algorithm>

class Instruction
{
public:
    Instruction(string s): s(s) {};
    Instruction(Hardware *hardware, string s, int index): hardware(hardware), address(index), s(s) {};
    Instruction(Hardware *hardware, string s): hardware(hardware), s(s) {}; // New
    virtual void execute() = 0;
    enum IType
    {
        R,
        I,
        D,
        B,
        CB
    };
    static IType instructionType(string s);

protected: //update
    Hardware *hardware;
    int address;
    string s; // for testing
};

//void Instruction::execute() { cout << "Running " << s << endl; }

Instruction::IType Instruction::instructionType(string s) { 
    vector<string> insWord = PreProcess::parseTokens(s);
    vector<string> CBSet {"CBZ", "CBNZ", "B.NE", "B.EQ", "B.LT", "B.LE", "B.GT", "B.GE", "B.HS"};
	
    // int length = insWord.size();
    // for(int i=0; i < length; i++) 
    //     transform(insWord[i].begin(), insWord[i].end(), insWord[i].begin(), ::toupper); // transform to uppercase
    if(!insWord[0].compare("ADD") || !insWord[0].compare("ADDS") || !insWord[0].compare("AND") || !insWord[0].compare("ANDS")
    || !insWord[0].compare("BR") || !insWord[0].compare("EOR") || !insWord[0].compare("LSL") || !insWord[0].compare("LSR")
    || !insWord[0].compare("ORR") || !insWord[0].compare("SUB") || !insWord[0].compare("SUBS") || !insWord[0].compare("FADDS")
    || !insWord[0].compare("FADDD") || !insWord[0].compare("FCMPS") || !insWord[0].compare("FCMPD") || !insWord[0].compare("FDIVS")
    || !insWord[0].compare("FDIVD") || !insWord[0].compare("FMULS") || !insWord[0].compare("FMULD") || !insWord[0].compare("FSUBS")
    || !insWord[0].compare("FSUBD") || !insWord[0].compare("LDURS") || !insWord[0].compare("LDURD")) 
        return IType::R;
    //TODO:: finish IInstruction,....
    else if(!insWord[0].compare("ADDI") || !insWord[0].compare("ADDIS") || !insWord[0].compare("ANDI") || !insWord[0].compare("ANDIS")
    || !insWord[0].compare("EORI") || !insWord[0].compare("ORRI") || !insWord[0].compare("SUBI") || !insWord[0].compare("SUBIS"))
        return IType::I;
    else if(!insWord[0].compare("LDUR") || !insWord[0].compare("LDURB") || !insWord[0].compare("LDURH") || !insWord[0].compare("LDURSW")
    || !insWord[0].compare("LDXR") || !insWord[0].compare("STUR") || !insWord[0].compare("STURB") || !insWord[0].compare("LDURH")
    || !insWord[0].compare("LDURSW") || !insWord[0].compare("LDXR"))
        return IType::D;
    else if(!insWord[0].compare("B") || !insWord[0].compare("BL")) 
        return IType::B;
    else if (find(CBSet.begin(), CBSet.end(), insWord[0]) != CBSet.end())
        return IType::CB;
    return IType::D;
}

// TODO:
class RInstruction : public Instruction
{
public:
    RInstruction(string s): Instruction(s) {}
    RInstruction(Hardware* hardware, string s): Instruction(hardware, s) {} // New constructor
    void execute();
    ~RInstruction() {}
};
class IInstruction : public Instruction
{
public:
    IInstruction(string s): Instruction(s) {}
    IInstruction(Hardware* hardware, string s): Instruction(hardware, s) {} // New constructor
    void execute();
    ~IInstruction() {}
};
class DInstruction : public Instruction
{
public:
    DInstruction(string s) : Instruction(s) {}
    DInstruction(Hardware *hardware, string s) : Instruction(hardware, s) {} // New constructor
    void execute();
    void toggle(char *start, int n);
    void Load(char *des, char *source, int n, bool wide_sign, int size);
    void Store(char *des, char *source, int n, int size)
    {
        for (int i = 0; i < n; i++)
            *(des + i) = *(source + size - n + i);
    }
    ~DInstruction() {}
};
class BInstruction : public Instruction
{
public:
    BInstruction(string s): Instruction(s) {}
    BInstruction(Hardware* hardware, string s): Instruction(hardware, s) {} // New constructor
    void execute();
    ~BInstruction() {}
};

class CBInstruction : public Instruction
{
public:
    CBInstruction(string s): Instruction(s) {}
    CBInstruction(Hardware* hardware, string s): Instruction(hardware, s) {} // New constructor
    void execute();
    ~CBInstruction() {}
};

void RInstruction::execute() {
    vector<string> insWord = PreProcess::parseTokens(s);
    if(!insWord[0].compare("ADD")) 
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) + hardware->GetRegister(insWord[3]));
    else if(!insWord[0].compare("AND"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) & hardware->GetRegister(insWord[3]));
    else if(!insWord[0].compare("EOR"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) ^ hardware->GetRegister(insWord[3]));
    else if(!insWord[0].compare("LSL"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) << stoi(insWord[3]));
    else if(!insWord[0].compare("LSR"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) >> stoi(insWord[3]));
    else if(!insWord[0].compare("ORR"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) | hardware->GetRegister(insWord[3]));
    else if(!insWord[0].compare("AND"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) & hardware->GetRegister(insWord[3]));
    else if(!insWord[0].compare("SUB"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) - hardware->GetRegister(insWord[3]));
    else if (!insWord[0].compare("BR"))
        hardware->PC = hardware->GetRegister("X30");
}

void IInstruction::execute() {
    vector<string> insWord = PreProcess::parseTokens(s);
    if(!insWord[0].compare("ADDI")) 
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) + stoi(insWord[3]));
    else if(!insWord[0].compare("ANDI")) 
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) & stoi(insWord[3]));
    else if(!insWord[0].compare("EORI")) 
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) ^ stoi(insWord[3]));
    else if(!insWord[0].compare("ORRI")) 
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) | stoi(insWord[3]));
    else if(!insWord[0].compare("SUBI")) 
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) - stoi(insWord[3]));
}

void BInstruction::execute() {
    vector<string> insWord = PreProcess::parseTokens(s);
    if(!insWord[0].compare("B")) {
        hardware->PC = PreProcess::label[insWord[1]];
        cout << "PC is: " << hardware->PC << '\n';
    }
    else if(!insWord[0].compare("BL")) {
        hardware->SetRegister("X30", hardware->PC + 1);
        hardware->PC = PreProcess::label[insWord[1]];
    }
}

    
void CBInstruction::execute()
{
    // vector<string> CBSet{"B.NE", "B.EQ", "B.LT", "B.LE", "B.GT", "B.GE", "B.HS"};
    vector<string> insWord = PreProcess::parseTokens(s);
    string command = insWord[0];
    if (command == "CBZ")
    {
        if (hardware->GetRegister(insWord[1]) == 0)
            hardware->PC = PreProcess::label[insWord[2]];
    }
    else if (command == "CBNZ")
    {
        if (hardware->GetRegister(insWord[1]) != 0)
            hardware->PC = PreProcess::label[insWord[2]];
    }
    else if ((command == "B.NE" && hardware->flags.ne()) ||
             (command == "B.EQ" && hardware->flags.eq()) ||
             (command == "B.LT" && hardware->flags.lt()) ||
             (command == "B.LE" && hardware->flags.le()) ||
             (command == "B.GT" && hardware->flags.gt()) ||
             (command == "B.GE" && hardware->flags.ge()) ||
             (command == "B.HS" && hardware->flags.hs()))
        hardware->PC = PreProcess::label[insWord[1]];
}



void DInstruction::toggle(char *start, int n)
{
    cout << "\n size of long: " << sizeof(long) << endl; 
    for (int i = 0; i < n; i++) 
         printf(" %02hhX", start[i]); 
         cout<<" size: "<<n; 
    printf("\n"); 
    for (int i = 0; i < n / 2; i++)
    {
        swap(start[i], start[n - i - 1]);
    }
    for (int i = 0; i < n; i++) 
         printf(" %02hhX", start[i]); 
         cout<<" size: "<<n; 
    printf("\n"); 
}
void DInstruction::Load(char *des, char *source, int n, bool wide_sign, int size)
{
    int sign = *(source)&0x80;
    for (int i = 0; i < n; i++)
        *(des + size - n + i) = *(source + i);
    for (int i = 0; i < size - n; i++)
        *(des + i) = *(des + i) & 0x00;
    if (wide_sign)
    {
        if (sign != 0)
            for (int i = 0; i < size - n; i++)
                *(des + i) = *(des + i) | 0xff;
    }
    this->toggle(des, size);
}
void DInstruction::execute()
{
    vector<string> insWord = PreProcess::parseTokens(s);
    if (!insWord[0].compare("LDUR"))
    {
        long tempregister;
        int index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), (char *)(&hardware->_mem[index]), 8, false, 8);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDURB"))
    {
        long tempregister;
        int index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), (char *)(&hardware->_mem[index]), 1, false, 8);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDURH"))
    {
        long tempregister;
        int index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), (char *)(&hardware->_mem[index]), 2, false, 8);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDURSW"))
    {
        long tempregister;
        int index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), (char *)(&hardware->_mem[index]), 4, true, 8);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDXR"))
        cout << 0;

    else if (!insWord[0].compare("STUR"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        cout << "value: " << tempregister << endl;
        this->toggle((char*)(&tempregister), sizeof(tempregister));
        cout << "value: " << tempregister << endl;
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        cout << "index: "<< index << endl << "ofsset: "<< offset<<endl;
        this->Store(hardware->_mem->mem+index+ offset, (char *)(&tempregister), sizeof(tempregister), sizeof(tempregister));
    }

    else if (!insWord[0].compare("STURB"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        this->toggle((char*)(&tempregister), 8);
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        this->Store((char *)(&hardware->_mem[index]) + offset, (char *)(&tempregister), 1, sizeof(tempregister));
    }

    else if (!insWord[0].compare("STURH"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        this->toggle((char*)(&tempregister), sizeof(tempregister));
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        this->Store((char *)(&hardware->_mem[index]) + offset, (char *)(&tempregister), 2, 8);
    }

    else if (!insWord[0].compare("STURW"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        this->toggle((char*)(&tempregister), 8);
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        this->Store((char *)(&hardware->_mem[index]) + offset, (char *)(&tempregister), 4, 8);
    }

    else if (!insWord[0].compare("STUXR"))
        cout
            << 0;
    else
        cout << 0;
}