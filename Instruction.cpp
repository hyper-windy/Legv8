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
        D
    };
    static IType instructionType(string s);

protected: //update
    Hardware *hardware;
    int address;
    string s; // for testing
};

//void Instruction::execute() { cout << "Running " << s << endl; }

Instruction::IType Instruction::instructionType(string s) { 
    //TODO: implement
    vector<string> insWord = PreProcess::parseTokens(s);
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
    DInstruction(string s): Instruction(s) {}
    DInstruction(Hardware* hardware, string s): Instruction(hardware, s) {} // New constructor
    void execute() {cout << "d";}
    ~DInstruction() {}
};

void RInstruction::execute() {
    vector<string> insWord = PreProcess::parseTokens(s);
     if(!insWord[0].compare("ADD")) 
         hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) + hardware->GetRegister(insWord[3]));
    cout << insWord[0]<<endl<<insWord[1]<<endl<<insWord[2]<<endl<<insWord[3];
}

void IInstruction::execute() {
    vector<string> insWord = PreProcess::parseTokens(s);
     if(!insWord[0].compare("ADDI")) 
         hardware->SetRegister(insWord[1], hardware->GetRegister(insWord[2]) + stoi(insWord[3]));
}