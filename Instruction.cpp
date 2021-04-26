#pragma once
#include "utils.cpp"
#include "Hardware.cpp"
#include <algorithm>

// TODO: thêm lệnh giả
// TODO: add more ARM source code for testing

class Instruction
{
public:
    Instruction(string s) : s(s){};
    Instruction(Hardware *hardware, string s) : hardware(hardware), s(s){};
    virtual void execute() = 0;

    enum IType
    {
        R,
        I,
        D,
        B,
        CB,
        PI //PSEUDO INSTRUCTION SET
    };
    static IType instructionType(string s);

protected:
    Hardware *hardware;
    string s;
};

Instruction::IType Instruction::instructionType(string s) // TODO: add defined instruction to sets
{
    vector<string> insWord = PreProcess::parseTokens(s);
    vector<string> CBSet{"CBZ", "CBNZ", "B.NE", "B.EQ", "B.LT", "B.LE", "B.GT", "B.GE", "B.HS"};
    vector<string> RSet{"ADD", "AND", "SUB", "EOR", "LSL", "LSR", "ORR", "AND", "BR", "FADDS", "FCMPS", "FDIVS", "FMULS", "FSUBS", };
    vector<string> ISet{"ADDI", "ANDI", "SUBI", "ADDIS", "ANDIS", "SUBIS", "EORI", "ORRI"};
    vector<string> DSet{"LDUR", "LDURB", "LDURH", "LDURSW", "LDXR", "STUR", "STURB", "LDURH", "LDURSW", "LDXR"};
    // 10 types
    vector<string> BSet{"B", "BL"};
    vector<string> PISet{"CMP", "CMPI", "LDA", "MOV"};
    if (find(RSet.begin(), RSet.end(), insWord[0]) != RSet.end())
        return IType::R;
    else if (find(ISet.begin(), ISet.end(), insWord[0]) != ISet.end())
        return IType::I;
    else if (find(DSet.begin(), DSet.end(), insWord[0]) != DSet.end())
        return IType::D;
    else if (find(BSet.begin(), BSet.end(), insWord[0]) != BSet.end())
        return IType::B;
    else if (find(CBSet.begin(), CBSet.end(), insWord[0]) != CBSet.end())
        return IType::CB;
    else if (find(PISet.begin(), PISet.end(), insWord[0]) != PISet.end())
        return IType::PI;
    else
        throw "Undefined instruction";
}

class RInstruction : public Instruction
{
public:
    RInstruction(string s) : Instruction(s) {}
    RInstruction(Hardware *hardware, string s) : Instruction(hardware, s) {} // New constructor
    void execute();
    void setFlags(long res, long a, long b)
    {
        if (res < 0)
            hardware->flags.setN(true);
        else if (res == 0)
            hardware->flags.setZ(true);
        if (hardware->flags.checkOverflow(a, b))
            hardware->flags.setC(true);
        if (hardware->flags.checkFlagCarry(a, b))
            hardware->flags.setV(true);
    }
    ~RInstruction() {}
};

class IInstruction : public Instruction
{
public:
    IInstruction(Hardware *hardware, string s) : Instruction(hardware, s) {} // New constructor
    void execute();
    void setFlags(long res, long a, long b)
    {
        if (res < 0)
            hardware->flags.setN(true);
        else if (res == 0)
            hardware->flags.setZ(true);
        if (hardware->flags.checkOverflow(a, b))
            hardware->flags.setC(true);
        if (hardware->flags.checkFlagCarry(a, b))
            hardware->flags.setV(true);
    }
    ~IInstruction() {}
};

class DInstruction : public Instruction
{
public:
    DInstruction(Hardware *hardware, string s) : Instruction(hardware, s) {}
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
    BInstruction(Hardware *hardware, string s) : Instruction(hardware, s) {}
    void execute();
    ~BInstruction() {}
};

class CBInstruction : public Instruction
{
public:
    CBInstruction(Hardware *hardware, string s) : Instruction(hardware, s) {}
    void execute();
    ~CBInstruction() {}
};

void RInstruction::execute()
{
    vector<string> insWord = PreProcess::parseTokens(s);
    if (!insWord[0].compare("ADD") || !insWord[0].compare("ADDS"))
    {
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) + hardware->GetRegister(insWord[3]));
        if (!insWord[0].compare("ADDS"))
            setFlags(hardware->GetRegister(insWord[1]), hardware->GetRegister(insWord[2]), hardware->GetRegister(insWord[3]));
    }
    else if (!insWord[0].compare("AND") || !insWord[0].compare("ANDS"))
    {
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) & hardware->GetRegister(insWord[3]));
        if (!insWord[0].compare("ANDS"))
            setFlags(hardware->GetRegister(insWord[1]), hardware->GetRegister(insWord[2]), hardware->GetRegister(insWord[3]));
    }
    else if (!insWord[0].compare("EOR"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) ^ hardware->GetRegister(insWord[3]));
    else if (!insWord[0].compare("LSL"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) << stoi(insWord[3]));
    else if (!insWord[0].compare("LSR"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) >> stoi(insWord[3]));
    else if (!insWord[0].compare("ORR"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) | hardware->GetRegister(insWord[3]));
    else if (!insWord[0].compare("AND"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) & hardware->GetRegister(insWord[3]));
    else if (!insWord[0].compare("SUB") || !insWord[0].compare("SUBS"))
    {
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) - hardware->GetRegister(insWord[3]));
        if (!insWord[0].compare("SUBS"))
            setFlags(hardware->GetRegister(insWord[1]), hardware->GetRegister(insWord[2]), hardware->GetRegister(insWord[3]));
    }
    else if (!insWord[0].compare("BR"))
        hardware->PC = hardware->GetRegister(insWord[1]);

    //update float instructions    
    else if (!insWord[0].compare("FADDS"))
        hardware->setFloatRegister(insWord[1], hardware->getFloatRegister(insWord[2]) + hardware->getFloatRegister(insWord[3]));
    else if (!insWord[0].compare("FSUBS"))
        hardware->setFloatRegister(insWord[1], hardware->getFloatRegister(insWord[2]) - hardware->getFloatRegister(insWord[3]));
    else if (!insWord[0].compare("FMULS"))
        hardware->setFloatRegister(insWord[1], hardware->getFloatRegister(insWord[2]) * hardware->getFloatRegister(insWord[3]));
    else if (!insWord[0].compare("FDIVS"))
        hardware->setFloatRegister(insWord[1], hardware->getFloatRegister(insWord[2]) / hardware->getFloatRegister(insWord[3]));
}

void IInstruction::execute()
{
    vector<string> insWord = PreProcess::parseTokens(s);
    if (!insWord[0].compare("ADDI") || !insWord[0].compare("ADDIS"))
    {
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) + stoi(insWord[3]));
        if (!insWord[0].compare("ADDIS"))
            setFlags(hardware->GetRegister(insWord[1]), hardware->GetRegister(insWord[2]), stoi(insWord[3]));
    }
    else if (!insWord[0].compare("ANDI") || !insWord[0].compare("ANDIS"))
    {
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) & stoi(insWord[3]));
        if (!insWord[0].compare("ANDIS"))
            setFlags(hardware->GetRegister(insWord[1]), hardware->GetRegister(insWord[2]), stoi(insWord[3]));
    }
    else if (!insWord[0].compare("EORI"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) ^ stoi(insWord[3]));
    else if (!insWord[0].compare("ORRI"))
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) | stoi(insWord[3]));
    else if (!insWord[0].compare("SUBI") || !insWord[0].compare("SUBIS"))
    {
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) - stoi(insWord[3]));
        if (!insWord[0].compare("SUBIS"))
            setFlags(hardware->GetRegister(insWord[1]), hardware->GetRegister(insWord[2]), stoi(insWord[3]));
    }
}

void BInstruction::execute()
{
    vector<string> insWord = PreProcess::parseTokens(s);
    if (!insWord[0].compare("B"))
    {
        hardware->PC = PreProcess::label[insWord[1]];
        cout << "PC is: " << hardware->PC << '\n';
    }
    else if (!insWord[0].compare("BL"))
    {
        hardware->SetRegister("X30", hardware->PC);
        hardware->PC = PreProcess::label[insWord[1]];
    }
}

// TODO: test LDUR - offset in byte
void CBInstruction::execute()
{
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
    for (int i = 0; i < n / 2; i++)
    {
        swap(start[i], start[n - i - 1]);
    }
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
        int size = sizeof(tempregister);
        int index;
        if (hardware->_data.find(insWord[2]) != hardware->_data.end())
            index = hardware->_data[insWord[2]];
        else
            index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), hardware->_mem->mem + index, size, false, size);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDURB"))
    {
        long tempregister;
        int size = sizeof(tempregister);
        int index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), hardware->_mem->mem + index, 1, false, size);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDURH"))
    {
        long tempregister;
        int size = sizeof(tempregister);
        int index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), hardware->_mem->mem + index, 2, false, size);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDURSW"))
    {
        long tempregister;
        int size = sizeof(tempregister);
        int index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), hardware->_mem->mem + index, 4, true, size);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("LDXR"))
        cout << 0;

    else if (!insWord[0].compare("STUR"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        int size = sizeof(tempregister);
        this->toggle((char *)(&tempregister), size);
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        //cout << "index: " << index << endl
        //   << "ofsset: " << offset << endl;
        this->Store(hardware->_mem->mem + index + offset, (char *)(&tempregister), size, size);
    }

    else if (!insWord[0].compare("STURB"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        int size = sizeof(tempregister);
        this->toggle((char *)(&tempregister), size);
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        this->Store(hardware->_mem->mem + index + offset, (char *)(&tempregister), 1, size);
    }

    else if (!insWord[0].compare("STURH"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        int size = sizeof(tempregister);
        this->toggle((char *)(&tempregister), size);
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        this->Store(hardware->_mem->mem + index + offset, (char *)(&tempregister), 2, size);
    }

    else if (!insWord[0].compare("STURW"))
    {
        long tempregister = hardware->GetRegister(insWord[1]);
        int size = sizeof(tempregister);
        this->toggle((char *)(&tempregister), size);
        int index = hardware->GetRegister(insWord[2]);
        int offset = stoi(insWord[3]);
        this->Store(hardware->_mem->mem + index + offset, (char *)(&tempregister), 4, size);
    }

    else if (!insWord[0].compare("STUXR")) // TODO: STUXR
        cout
            << 0;
    else
        cout << 0;
}

class PIInstruction : public DInstruction
{
public:
    PIInstruction(Hardware *hardware, string s) : DInstruction(hardware, s) {} // New constructor
    void execute();
    void setFlags(long res, long a, long b)
    {
        if (res < 0)
            hardware->flags.setN(true);
        else if (res == 0)
            hardware->flags.setZ(true);
        if (hardware->flags.checkOverflow(a, b))
            hardware->flags.setC(true);
        if (hardware->flags.checkFlagCarry(a, b))
            hardware->flags.setV(true);
    }
    // void Load(char *des, char *source, int n, bool wide_sign, int size)
    // {
    //     int sign = *(source)&0x80;
    //     for (int i = 0; i < n; i++)
    //         *(des + size - n + i) = *(source + i);
    //     for (int i = 0; i < size - n; i++)
    //         *(des + i) = *(des + i) & 0x00;
    //     if (wide_sign)
    //     {
    //         if (sign != 0)
    //             for (int i = 0; i < size - n; i++)
    //                 *(des + i) = *(des + i) | 0xff;
    //     }
    //     this->toggle(des, size);
    // }
    ~PIInstruction() {}
};

void PIInstruction::execute()
{
    vector<string> insWord = PreProcess::parseTokens(s);
    if (!insWord[0].compare("CMP"))
    {
        long tempregister;
        tempregister = hardware->GetRegister(insWord[1]) - hardware->GetRegister(insWord[2]);
        if (!insWord[0].compare("SUBS"))
            setFlags(tempregister, hardware->GetRegister(insWord[1]), hardware->GetRegister(insWord[2]));
    }

    else if (!insWord[0].compare("CMPI"))
    {
        long tempregister;
        tempregister = hardware->GetRegister(insWord[1]) - stoi(insWord[2]);
        if (!insWord[0].compare("SUBS"))
            setFlags(tempregister, hardware->GetRegister(insWord[1]), stoi(insWord[2]));
    }

    else if (!insWord[0].compare("LDA"))
    {
        long tempregister;
        int size = sizeof(tempregister);
        int index;
        if (hardware->_data.find(insWord[2]) != hardware->_data.end())
            index = hardware->_data[insWord[2]];
        else
            index = hardware->GetRegister(insWord[2]) + stoi(insWord[3]);
        this->Load((char *)(&tempregister), hardware->_mem->mem + index, size, false, size);
        hardware->SetRegister(insWord[1], tempregister);
    }

    else if (!insWord[0].compare("MOV"))
    {
        hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]));
    }
}