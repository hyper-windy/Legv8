// leg1.21.0
// Prototype - can gop y va hoan thien

#include <iostream>
#include <vector>
using namespace std;

#include "utils.cpp"
#include "Hardware.cpp"

const int MEMORY_SIZE = (int)10e6;

#include "Instruction.cpp"

class Program
{
public:
    Program();
    ~Program();
    void pushInstruction(string raw);
    void pushData(string raw, int& byteAddress);
    void run();
    void log();

private:
    Hardware *hardware;
    vector<Instruction *> instructions;
};

Program::Program() { hardware = new Hardware(MEMORY_SIZE); }

Program::~Program() { delete hardware; } // TODO: clean up instructions

void Program::pushInstruction(string raw) // TODO: Init Instruction correctly
{
    Instruction::IType type = Instruction::instructionType(raw);
    if (type == Instruction::IType::R)
        instructions.push_back(new RInstruction(raw)); // RInstruction(hardware, raw, index)
    else if (type == Instruction::IType::I)
        instructions.push_back(new IInstruction(raw));
    else if (type == Instruction::IType::D)
        instructions.push_back(new DInstruction(raw));
    else
        cout << "Invalid instruction type - Line: " << raw;
}

void Program::pushData(string raw, int& top)
{
    hardware->pushData(raw, top);
}

void Program::run()
{
    int &PC = hardware->PC;
    while (0 <= PC && PC < (int)instructions.size())
    {
        PC++;
        instructions[PC - 1]->execute();
    }
    cout << "Program finished running\n";
}

void Program::log()
{
    cout << "Hardware Log\n";
    hardware->log();
}


int main()
{
    string filename = "example/test.v";
    PreProcess source(filename);    
    
    Program leg;

    // Load data
    int top = 0;
    for (string var: source.data)
        leg.pushData(var, top);

    // Load instruction
    for (string ins: source.instructions)
        leg.pushInstruction(ins);

    // cout << "Log data\n";
    // logVector(source.data);
    // cout << "Log source\n";
    // logVector(source.instructions);
    // cout << "Log label\n";
    // for (auto p: source.label)
    //     cout << p.first << ": " << p.second << endl;


    leg.run();
    leg.log();
    return 0;
}
