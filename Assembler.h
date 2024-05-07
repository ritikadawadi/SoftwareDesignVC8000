//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII();

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }

    // Assembler part
    void AssemblyInstruction(string& content, int& loc);

    //Machine language part
    void MachineInstruction(string& content, int& loc);

    // Run emulator on the translation.
    void RunProgramInEmulator() { cout << "Must implementL RunProgramInEmulator( )" << endl; }

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object

    bool CheckOperands();
    void HandleOrgOperation(int& loc);
    bool CheckLabel();
    bool CheckLabelDefinition();
   // void HandleDsOperation(int& loc);
    void HandleDcOperation(string& content, int& loc);

    bool ValidateInstruction();
    void HandleHaltInstruction();
    void HandleNonHaltInstruction(string& content, int& loc, const string &OpCode);
    void UpdateLocation(int& loc);

    
};

