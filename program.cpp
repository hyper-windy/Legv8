// leg1.21.0
// Prototype - can gop y va hoan thien

#include <iostream>
#include <vector>
using namespace std;

class Hardware
{
public:
    class Memory;
    class Register;

    Hardware();
    ~Hardware();
    void log(); // display data saved on memory and register
    int PC;

private:
    Memory *mem;
    Register *reg;
};

// #include "hardware.cpp"
// TODO: implement hardware
class Hardware::Memory
{
};
class Hardware::Register
{
};
Hardware::Hardware() {}
Hardware::~Hardware() {}
void Hardware::log() {}

class Instruction
{
public:
    Instruction();
    Instruction(Hardware *hardware, string s, int index);
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
};

// #include <Instruction.cpp>
// TODO: implement Instruction
Instruction::Instruction() {}
Instruction::Instruction(Hardware *hardware, string s, int index) {}
void Instruction::execute() {}
Instruction::IType Instruction::instructionType(string s) { return IType::I; }

// TODO:
class RInstruction : public Instruction
{
};
class IInstruction : public Instruction
{
};
class JInstruction : public Instruction
{
};

class Program
{
public:
    Program();
    ~Program();
    void push(string raw);
    void run();
    void log();

private:
    Hardware *hardware;
    vector<Instruction *> instructions;
};

Program::Program() { hardware = new Hardware(); }

Program::~Program() { delete hardware; } // TODO: clean up instructions

void Program::push(string raw) // TODO: Init Instruction correctly
{
    Instruction::IType type = Instruction::instructionType(raw);
    if (type == Instruction::IType::R)
        instructions.push_back(new RInstruction); // RInstruction(hardware, raw, index)
    else if (type == Instruction::IType::I)
        instructions.push_back(new IInstruction);
    else if (type == Instruction::IType::J)
        instructions.push_back(new JInstruction);
    else
        cout << "Invalid instruction type - Line: " << raw;
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
    vector<string> rawIns = {
        "addi 1 0 -1",
        "addi 2 0 -2",
        "add 3 1 2"};

    Program leg;
    for (string ins : rawIns)
        leg.push(ins);
    leg.run();
    leg.log();
    return 0;
}
