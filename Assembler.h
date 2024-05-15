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
   // ~Assembler( );

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
    void RunProgramInEmulator(); //{ cout << "Must implementL RunProgramInEmulator( )" << endl; }

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    Emulator m_emul;        // Emulator object
    
    void CheckOperandsAndLabels();
    void HandleORGOperation(int& a_loc);
    void HandleDSOperation(int& a_loc);
    void HandleDCOperation(int& a_loc, string& a_content);
    void InsertIntoMemory(int& a_loc, const string& a_content);
    void ProcessInstruction(int& a_loc, string& a_content);

    void FormatOpCode(string& OpCode);
    void CheckForHALTOperation();
    void CheckForLabelErrors();
    void CheckOperandPresenceAndType(string& a_content, int& location, string& locate);
    void HandleNumericOperand1(string& a_content, int& location, string& locate, const string& OpCode);
    void HandleSymbolicOperand1(string& a_content, int& location, string& locate, const string& OpCode);
    void ProcessMachineInstruction(string& a_content, int& a_loc);
    
};

