//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "SymTab.h"

/*
NAME:
    Assembler();

SYNOPSIS:
    Assembler::Assembler( int argc, char *argv[] );
    argc        -> represents the number of command line arguments provided to the program
    *argc[]     -> reprsents an array of character pointers that consists the names of the programs to be opened
 
DESCRIPTION:
    Construction function.  
    The constructor initializes the'm_facc' member variable with the Fileaccess constructor. This allows the
    Assembler class to access and process files. 

*/

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  

/*
NAME:

SYNOPSIS:

DESCRIPTION:

*/
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler( )
{
}

/*
NAME: 
    PassI() -  Pass I establishes the location of the labels.

SYNOPSIS:
    Assembler::PassI();
       
DESCRIPTION:

RETURN:
    void
    
    

*/
//  
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::InstructionType::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::InstructionType::ST_Comment )  
        {
        	continue;
	    }
        // Handle the case where there is an error.
        
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
/*
NAME:

    PassII() - Translates and records errors

SYNOPSIS:

    Assembler::PassII();
        
DESCRIPTION:

    This function translates each line, and also records and displays errors.
    Initially, the function rewinds to the beginning of the file. It creates a location variable and sets it to
    0. Inside a while loop, if there is not a next line, an error is returned, as the last line should be of type 
    'END'. Instruction object is called, which parses the line into its fileds. 

RETURN:
    void

*/
void Assembler::PassII()
{
    m_facc.rewind(); // rewinds to the beginning of the file 

    int loc = 0;

    // Initialize for error reporting
    Errors::InitErrorReporting();
    string content;

    //print title
    cout << endl;
    cout << "VC8000 , Ritika's version! " << endl;
    cout << endl; 
    cout << "Translation of the program into machine language..." << endl;
    cout << "Location\tContents\t\t Original Statement" << endl;


    while (true) {

        // reads the line from the source code
        string line;

        if (!m_facc.GetNextLine(line)) 
        {   
            // if there are no more lines, we are probably missing the end statement
            Errors::RecordError("Error! No End Statement");
            Errors::DisplayErrors();
            return;
        }
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

            switch (st)
            {
            case Instruction::InstructionType::ST_End: {
                cout << "\t\t\t" << m_inst.GetInstruction() << endl;
                bool foundNonEmpty = false;
                while (m_facc.GetNextLine(line))
                {
                    if (!line.empty()) {
                        Errors::RecordError("Error! Last Statement is not the end!");
                        Errors::DisplayErrors();
                        foundNonEmpty = true;
                    }
                    if (!foundNonEmpty) {
                        return;
                    }
                }
                break;
            }
            case Instruction::InstructionType::ST_Comment:
                cout << "\t\t\t\t" << m_inst.GetInstruction() << endl;
                break;

            case Instruction::InstructionType::ST_Error: 
                Errors::RecordError("Error! Invalid Operation" + m_inst.GetInstruction());
                Errors::DisplayErrors();
                break;
               
            default:
                // check the label length
                if (m_inst.GetLabel().length() > 15) {
                    Errors::RecordError("Error! Very large label in " + m_inst.GetInstruction());
                    Errors::DisplayErrors();
                    break;
                }

                if (isdigit(m_inst.GetLabel()[0])) {
                    Errors::RecordError("Errors! Label cannot start with an integer in " + m_inst.GetInstruction());
                    Errors::DisplayErrors();
                    break;
                }

                if (m_inst.LocationNextInstruction(loc) > 999999) {
                    Errors::RecordError("Errors! Memory Overload!");
                    Errors::DisplayErrors();
                    break;
                }

                // calls the assembly function if it is an assembly function
                if (st == Instruction::InstructionType::ST_AssemblerInstr) {
                    AssemblyInstruction(content, loc);
                }

                // calls the machine lan function if it is a machine lan function
                else if (st == Instruction::InstructionType::ST_MachineLanguage) {
                    MachineInstruction(content, loc);
                }
            }
    }

}
void Assembler::CheckOperandsAndLabels() {
    if (!m_inst.GetOperand2().empty()) {
        Errors::RecordError("Error! Operand 2 found in Assembly Instruction!");
        Errors::DisplayErrors();
    }
    if (m_inst.GetOperand1().empty()) {
        Errors::RecordError("Error! Missing Operand 1 in " + m_inst.GetOpCode());
        Errors::DisplayErrors();
    }
    else if (!m_inst.IsNumericOperand1()) {
        Errors::RecordError("Error! Operand must be Numeric in " + m_inst.GetOpCode());
        Errors::DisplayErrors();
    }
    if (m_inst.IsNumericOperand1()) {
        if (stoi(m_inst.GetOperand1()) > 10000) {
            Errors::RecordError("Error! Very large value of Operand 1 in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
    }
    if (m_inst.GetLabel().empty() && m_inst.GetOpCode() != "ORG") {
        Errors::RecordError("Error! Label not found in " + m_inst.GetOpCode());
        Errors::DisplayErrors();
    }
    else {
        int temp;
        m_symtab.LookupSymbol(m_inst.GetLabel(), temp);
        if (temp == m_symtab.multipleDefinedSymbol) {
            Errors::RecordError("Error! Symbol Defined in Multiple Locations");
            Errors::DisplayErrors();
        }
    }
}

void Assembler::HandleORGOperation(int& a_loc) {
    if (!m_inst.GetLabel().empty()) {
        Errors::RecordError("Error! Label found in ORG!");
        Errors::DisplayErrors();
    }
    cout << a_loc << "\t\t\t\t" << m_inst.GetInstruction() << endl;
}

void Assembler::HandleDSOperation(int& a_loc) {
    cout << a_loc << "\t\t\t\t" << m_inst.GetInstruction() << endl;
}

void Assembler::HandleDCOperation(int& a_loc, string& a_content) {
    a_content = m_inst.GetOperand1();
    while (a_content.size() != 9) {
        a_content = "0" + a_content;
    }
    InsertIntoMemory(a_loc, a_content);
    cout << a_loc << "\t\t\t" << a_content << "\t\t" << m_inst.GetInstruction() << endl;
}

void Assembler::InsertIntoMemory(int& a_loc, const string& a_content) {
    m_emul.insertMemory(a_loc, stoll(a_content));
}

void Assembler::ProcessInstruction(int& a_loc, string& a_content) {
    if (m_inst.GetOpCode() == "ORG") {
        HandleORGOperation(a_loc);
    }
    else if (m_inst.GetOpCode() == "DS") {
        HandleDSOperation(a_loc);
    }
    else { // Assuming it's "DC"
        HandleDCOperation(a_loc, a_content);
    }
    a_loc = m_inst.LocationNextInstruction(a_loc);
}

void Assembler::AssemblyInstruction(string& a_content, int& a_loc) {
    CheckOperandsAndLabels();

    ProcessInstruction(a_loc, a_content);
}


void Assembler::FormatOpCode(string& OpCode) {
    if (OpCode.size() != 2) {
        OpCode = "0" + OpCode;
    }
}

void Assembler::CheckForHALTOperation() {
    if (m_inst.GetOpCode() == "HALT") {
        if (!m_inst.GetOperand1().empty()) {
            Errors::RecordError("Error! Operand found in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
        if (!m_inst.GetLabel().empty()) {
            Errors::RecordError("Error! Label found in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
    }
}

void Assembler::CheckForLabelErrors() {
    if (!m_inst.GetLabel().empty()) {
        int temp;
        m_symtab.LookupSymbol(m_inst.GetLabel(), temp);
        if (temp == m_symtab.multipleDefinedSymbol) {
            Errors::RecordError("Error! Symbol Defined in Multiple Locations");
            Errors::DisplayErrors();
        }
    }
}

void Assembler::CheckOperandPresenceAndType(string& a_content, int& location, string& locate) {
    if (!m_inst.IsNumericOperand1()) {
        if (m_inst.GetNumOpCode() != 11 && m_inst.GetNumOpCode() != 12 && m_inst.GetNumOpCode() != 17) {
            Errors::RecordError("Error! No Register found in " + m_inst.GetInstruction());
            Errors::DisplayErrors();
        }
        if (!m_inst.GetOperand2().empty()) {
            Errors::RecordError("Error! Extra Operand found in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
    }
    else {
        if (stoi(m_inst.GetOperand1()) < 0 || stoi(m_inst.GetOperand1()) > 9) {
            Errors::RecordError("Error::Invalid Register value");
            Errors::DisplayErrors();
        }
        if (m_inst.GetOperand2().empty()) {
            Errors::RecordError("Error! Operand 2 missing in " + to_string(m_inst.GetNumOpCode()));
            Errors::DisplayErrors();
        }
    }
}

void Assembler::HandleNumericOperand1(string& a_content, int& location, string& locate, const string& OpCode) {
    if (m_inst.GetNumOpCode() >= 7 && m_inst.GetNumOpCode() <= 10) {
        if (!m_inst.IsNumericOperand2()) {
            Errors::RecordError("Error! Operand 2 must be numeric in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
        else {
            if (stoi(m_inst.GetOperand2()) < 0 || stoi(m_inst.GetOperand2()) > 9) {
                Errors::RecordError("Error::Invalid Register value");
                Errors::DisplayErrors();
            }
        }
        a_content = OpCode + m_inst.GetOperand1() + m_inst.GetOperand2();
        while (a_content.size() != 9) {
            a_content = a_content + "0";
        }
    }
    else {
        a_content = OpCode + m_inst.GetOperand1();
        m_symtab.LookupSymbol(m_inst.GetOperand2(), location);
        if (location == 0) {
            Errors::RecordError("Error! Cannot find the location of the symbol " + m_inst.GetOperand2());
            Errors::DisplayErrors();
        }
        locate = to_string(location);
        while (locate.size() != 6) {
            locate = "0" + locate;
        }
        a_content = a_content + locate;
    }
}

void Assembler::HandleSymbolicOperand1(string& a_content, int& location, string& locate, const string& OpCode) {
    a_content = OpCode + "9";
    if (!m_inst.GetOperand1().empty()) {
        m_symtab.LookupSymbol(m_inst.GetOperand1(), location);
        if (location == 0) {
            Errors::RecordError("Error! Cannot find the location of the symbol " + m_inst.GetOperand1());
            Errors::DisplayErrors();
        }
    }
    locate = to_string(location);
    while (locate.size() != 6) {
        locate = "0" + locate;
    }
    a_content = a_content + locate;
}

void Assembler::ProcessMachineInstruction(string& a_content, int& a_loc) {
    string OpCode = to_string(m_inst.GetNumOpCode());
    FormatOpCode(OpCode);
    CheckForHALTOperation();
    CheckForLabelErrors();

    int location = 0;
    string locate;

    if (!m_inst.IsNumericOperand1()) {
        HandleSymbolicOperand1(a_content, location, locate, OpCode);
    }
    else {
        CheckOperandPresenceAndType(a_content, location, locate);
        HandleNumericOperand1(a_content, location, locate, OpCode);
    }

    // Inserting into memory and calculating location of next instruction
    m_emul.insertMemory(a_loc, stoll(a_content));
    cout << a_loc << "\t\t" << a_content << "\t\t" << m_inst.GetInstruction() << endl;
    a_loc = m_inst.LocationNextInstruction(a_loc);
}

void Assembler::MachineInstruction(string& a_content, int& a_loc) {
    ProcessMachineInstruction(a_content, a_loc);
}


void Assembler::RunProgramInEmulator() {
    cout << "----------------------------------------------" << endl;
    cout << "Press Enter to continue..." << endl;
    cin.ignore();
    cout << "Results from Emulating Program:" << endl;

    //run only when there are no errors
    /**/
    if (Errors::NoError()) {
        m_emul.runProgram();
    }
    else {
        cout << "Emulator cannot run because of Errors!" << endl;
    }
    cout << "End of Emulation" << endl;
}
/*void Assembler::RunProgramInEmulator()*/
