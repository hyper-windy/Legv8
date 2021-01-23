// Pre Process Instructions and Data

#include<iostream>
#include<fstream>
#include<string.h>
#include<sstream>
#include<map>
#include<vector>

using namespace std;

class PreProcess {
private:
    vector<string>      instructions;
    vector<string>      data;
    string              linkFile;   
    map<string, int>    label;
public:
    PreProcess(string _linkFile) : linkFile(_linkFile) {}
    string  getToken(string _instruction, int pos); // get Word at position(pos) from string _instruction
    string  getString(string _instruction);         // get string in " "
    int     countToken(string _instruction);        // count word in a string _instruction
    void    readFile();                             // read File
    int     getNumberOfInstructions()   {return this->instructions.size();} // get number of Instructions
    string  getInstructions(int index)  {return this->instructions[index];}  // get Instruction at Index      
    int     getNumberOfData()           {return this->data.size();}         // get number of Data
    string  getData(int index)          {return this->data[index];}          // get Data at index
    void    printLabel() {
        for(auto it = label.begin(); it != label.end(); ++it)
        {
            std::cout << it->first << " " << it->second << "\n";
        }
    }
};

string  PreProcess::getToken(string _instruction, int pos) {
    int length = _instruction.length();
    for(int i=0; i < length; i++) {
        if(_instruction[i] == ',' || _instruction[i] == ':' || _instruction[i] == '#' || _instruction[i] == '[' || _instruction[i] == ']') 
        _instruction[i] = ' ';
    }
    stringstream ss(_instruction);
    string getW;
    int index = 1;
    while(ss >> getW) {
        if(index == pos) return getW;
        index++;
    }
    return "";
}

string  PreProcess::getString(string _instruction) {
    string str = "";
    int begin = (int)_instruction.find('\"');
    int end = (int)_instruction.find_last_of('\"');
    str = _instruction.substr(begin+1, end-begin-1);
    return str;
}

int     PreProcess::countToken(string _instruction) {
    int length = _instruction.length();
    for(int i=0; i < length; i++) {
        if(_instruction[i] == ',' || _instruction[i] == ':' || _instruction[i] == '#' || _instruction[i] == '[' || _instruction[i] == ']') 
        _instruction[i] = ' ';
    }
    stringstream ss(_instruction);
    string getW;
    int count = 0;
    while(ss >> getW) count++;
    return count; 
}

void    PreProcess::readFile() {
    string temp = "";
    ifstream inFile;
    inFile.open(linkFile, ios::binary);
    // READ DATA
    while(getToken(temp, 1).compare(".data")) getline(inFile, temp);
    while(!inFile.eof()) {
        getline(inFile, temp);
        if(!getToken(temp, 1).compare(".text")) break;
        if((int)temp.find(":") >= 0) {
            int pos = (int)temp.find(":");
            temp[pos] = ' ';
        }
        if(!getToken(temp, 1).compare("")) continue;
        data.push_back(temp);
    }
    // READ INSTRUCTION
    while(!inFile.eof()) {
        getline(inFile, temp);
        if((int)temp.find(':') >= 0) {
            int pos = (int)temp.find(':');
            string labelName = temp.substr(0, pos);
            labelName = getToken(labelName, 1);
            label.insert(std::pair<string, int>(labelName, instructions.size()));
            temp.erase(temp.begin(), temp.begin() + pos + 1);
        }
        if((int)temp.find("//") >= 0) {
            int pos = (int)temp.find("//");
            temp.erase(temp.begin() + pos, temp.end());
        }
        if(!getToken(temp, 1).compare("")) continue;
        instructions.push_back(temp);
    }
}

int main() {
    PreProcess a("text.txt");
    a.readFile();
    a.printLabel();
}