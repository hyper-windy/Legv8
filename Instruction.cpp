#pragma once
#include "utils.cpp"
#include "Hardware.cpp"

class Instruction
{
public:
    Instruction(string s): s(s) {};
    Instruction(Hardware *hardware, string s, int index): hardware(hardware), address(index), s(s) {};
    void execute();
    enum IType
    {
        R,
        I,
        D
    };
    static IType instructionType(string s);

private:
    Hardware *hardware;
    int address;
    string s; // for testing
};

void Instruction::execute() { cout << "Running " << s << endl; }
Instruction::IType Instruction::instructionType(string s) { return IType::I; }

// TODO:
class RInstruction : public Instruction
{
public:
    RInstruction(string s): Instruction(s) {}
    virtual void execute();
    ~RInstruction() {}
};
class IInstruction : public Instruction
{
public:
    IInstruction(string s): Instruction(s) {}
    virtual void execute();
    ~IInstruction() {}
};
class DInstruction : public Instruction
{
public:
    DInstruction(string s): Instruction(s) {}
    virtual void execute();
    ~DInstruction() {}
};
