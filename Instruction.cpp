#pragma once
#include "utils.cpp"

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
        J
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
};
class IInstruction : public Instruction
{
public:
    IInstruction(string s): Instruction(s) {}
};
class JInstruction : public Instruction
{
public:
    JInstruction(string s): Instruction(s) {}
};
