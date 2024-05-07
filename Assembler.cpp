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
        }
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

            switch (st)
            {
            case Instruction::InstructionType::ST_End:
                cout << "\t\t\t" << m_inst.GetInstruction() << endl;

                while (m_facc.GetNextLine(line))
                {
                    if (!line.empty()) {
                        Errors::RecordError("Error! Last Statement is not the end!");
                        Errors::DisplayErrors();
                    }
                    else {
                        continue;
                    }
                }
                break;

            case Instruction::InstructionType::ST_Comment:
                cout << "\t\t\t\t" << m_inst.GetInstruction() << endl;
                break;

            case Instruction::InstructionType::ST_Error:
                Errors::RecordError("Error! Invalid Operation " + m_inst.GetInstruction());
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

/*
NAME:
    AssemblyInstruction() - Process the assembly language instruction and records errors

SYNOPSIS:
    Assembler::AssemblyInstruction(string& content, int& loc);
    &content    --> content of the program 
    &loc        --> the location of the code

DESCRIPTION:

RETURN:
    void
*/

void Assembler::AssemblyInstruction(string& a_content, int& a_loc) {
     
    // Check operands
    if (!CheckOperands()) {
        return;
    }

    // handle ORG operation
    if (m_inst.GetOpCode() == "ORG") {
        HandleOrgOperation(a_loc);
    }

    // handle DC or DS operations
    else {
        //check label
        if (CheckLabel()) {
            //return;
        }

        // check if label is defined in multiple locations
        if (!CheckLabelDefinition()) {
            //return;
        }

        // handle DS operation, if it is DS, the instruction will be printed 
        if (m_inst.GetOpCode() == "DS") {
           // HandleDsOperation(a_loc);
            cout << a_loc << "\t\t\t" << m_inst.GetInstruction() << endl;
        }

        // handle DC operator
        else {
            HandleDcOperation(a_content, a_loc);
        }
    }

    a_loc = m_inst.LocationNextInstruction(a_loc);

}

bool Assembler::CheckOperands() {
    //checks for operand 2

    bool errorFound = false;

    if (!m_inst.GetOperand2().empty()) {
        Errors::RecordError("Error! Operand 2 found in Assembly Instruction!");
        Errors::DisplayErrors();
        errorFound = true;
    }
    
    // Checks for operand 1
    if (!m_inst.GetOperand1().empty()) {
        Errors::RecordError("Error! Missing operand 1 in " + m_inst.GetOpCode());
        Errors::DisplayErrors();
        errorFound = true;
    }

    //Checks if operand 1 is numeric 
    else if (!m_inst.IsNumericOperand1()) {
        Errors::RecordError("Error! Operand 1 must be numeric in " + m_inst.GetOpCode());
        Errors::DisplayErrors();
        errorFound = true;
    }

    // Checks if value in operand 1 is larger than 10000
    if (m_inst.IsNumericOperand1()) {
        if (stoi(m_inst.GetOperand1()) > 10000) {
            Errors::RecordError("Error! Very large value of Operand 1 in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
            errorFound = true;
        }
    }
    return errorFound;
}



void Assembler::HandleOrgOperation(int& a_loc) {
    // checks if label exists or not
    if (!m_inst.GetLabel().empty()) {
        Errors::RecordError("Error! Label found in the ORG.");
        Errors::DisplayErrors();
    }
    // if no label, prints out the instruction 
    cout << a_loc << "\t\t\t" << m_inst.GetInstruction() << endl;
}

bool Assembler::CheckLabel() {
    // checks if the label exists or not
    if (m_inst.GetLabel().empty()) {
        Errors::RecordError("Error! Label not found in " + m_inst.GetOpCode());
        Errors::DisplayErrors();
        return false;
    }
    return true;
}

bool Assembler::CheckLabelDefinition() {
    // looks up the symbol table to see if there are multiple locations of the label
    int temp;
    m_symtab.LookupSymbol(m_inst.GetLabel(), temp);
    if (temp == m_symtab.multipleDefinedSymbol) {
        Errors::RecordError("Error! symbol(label) Defined in multiple locations");
        Errors::DisplayErrors();
        return false;
    }
    return true;
}

void Assembler::HandleDcOperation(string& a_content, int& a_loc) { 

    a_content = m_inst.GetOperand1();

    while (a_content.size() != 9) {
        a_content = "0" + a_content;
    }
    // inserting into the memory
    m_emul.insertMemory(a_loc, stoll(a_content));
    cout << a_loc << "\t  " << a_content << "\t\t" << m_inst.GetInstruction() << endl; 
}


/*
NAME:
    MachineInstruction() -

SYNOPSIS:
    Assembler::MachineInstruction(string& content, int& loc);
    &content    --> content of the program 
    &loc        --> the location of the code


DESCRIPTION:

RETURN:
    void
*/

void Assembler::MachineInstruction(string& a_content, int& a_loc) {
        
    // to check basic operands and labels 
    if (!ValidateInstruction() && CheckLabelDefinition()) {
        return;
    }

    // to handle HALT operation
    if (m_inst.GetOpCode() == "HALT") {
        HandleHaltInstruction();
    }
    
    

}





