
//		Implementation of the Instruction  class. 
#include "stdafx.h"
#include "Instruction.h"

/*
NAME:
    ParseInstruction - Parses instruction to determine its type- assembly language, machine language,
    comments, end instruction, or an error.

SYNOPSIS:
    InstructionType Instruction::ParseInstruction(string a_line);
    a_line      --> reprsents a single line of code from the source file. The function takes in a_line
                    determine the type of instruction it represents, based on the InstructionType enum.
    The function returns the instruction type

DESCRIPTION:

    Parsing invloves breaking down the input into smaller components, and determining their significance.
    a_line is not passed by reference, as in this case it is not required to modify the contents of the 
    lines from the inputfile. The major task is to read each line and determine its type and field. So,
    a copy of the line is made when iterpreting its content instead of altering the file itself. 


*/
// parse an instruction and return its type. 
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    // Record the original statement.  This will be needed in the sceond pass.
    m_instruction = a_line;

    // Delete any comment from the line.
    DeleteComment( a_line );

    // Record label, opcode, and operands.  Up to you to deal with formatting errors.
    bool isFormatError = RecordFields( a_line );
    
    // Check if this is a comment.
    if (m_Label.empty() && m_OpCode.empty())
    {
        return InstructionType::ST_Comment;
    }
    // Return the instruction type.  This has to be handled in the code.
    return m_type;
}

/*
  NAME:
      DeleteComment() - deletes comments, void function.

  SYNOPSIS:
      void DeleteComment(string &a_line);
      &a_line        --> a line from the input file is passed by reference,
                          and the fucntion checks if it is a comment

  DESCRIPTION:
      If the function finds ';' synbol, it deletes the line because,
      all comments begin with a ';' symbols. In this function size_t is a
      data type that returns the position of the first occurance of character
      in find(). npos represents the largest possible size of any object.
      So, if the semicolon is found, it will be in a certain position,
      so it will not be equal to 'npos' in which case, the case inside
      the if block will be executed.

  */
  // Delete any comments from the statement.
void Instruction::DeleteComment(string& a_line)
{
    size_t isemi1 = a_line.find(';');
    if (isemi1 != string::npos)
    {
        a_line.erase(isemi1);
    }
}

/*
NAME: 

    RecordFields();

SYNOPSIS:

    bool RecordFields( const string &a_line );
    &a_line     --> a_line is pass by const reference here, as it modifies the 
                    line according to its content type.
    returns true if the function successfully records the fields, and false if there is a format error

MACHINE LAN OPCODES:

    ADD: Adds two operands together.
    SUB: Subtracts the second operand from the first.
    MULT: Multiplies two operands.
    DIV: Divides the first operand by the second.
    LOAD: Loads a value from memory into a register.
    STORE: Stores a value from a register into memory.
    ADDR: Adds the contents of two registers and stores the result in another register.
    SUBR: Subtracts the contents of two registers and stores the result in another register.
    MULTR: Multiplies the contents of two registers and stores the result in another register.
    DIVR: Divides the contents of two registers and stores the result in another register.
    READ: Reads data from an input device into memory.
    WRITE: Writes data from memory to an output device.
    B: Branches to a specified address unconditionally.
    BM: Branches to a specified address if the last result was negative.
    BZ: Branches to a specified address if the last result was zero.
    BP: Branches to a specified address if the last result was positive.
    HALT: Halts the program execution.

ASSEMBLER OPCODES:

    DC: Define constant
    DS: Define storage 
    ORG: Define origin
    END: End

DESCRIPTION:

        This function calls several other functions of the Instruction class. Initially, it calls the 
        ParseLineIntoFields function. The function assigns the a_line into it's respective elements. 
        Subsequently, it checks for any comments and operands in the a_line string. 
        The function calls isStrNumber on operand 1 and operand 2, and if it
        returns true it uses stoi() function from standard library. stoi() converts a
        string represting an integer to an integer type. All the character of the function are converted
        to uppercase. Two arrays of machine language opcode and assembler instruction is initialized.
        The fucntion checks if m_OpCode is any of the above by comparing it with the elements of the arrays.
        it assigns m_type into the respective type from the enum class. 


*/
// Record the fields that make up the instructions.
bool Instruction::RecordFields( const string &a_line )
{
    // Get the fields that make up the instruction.
    bool isFormatError = ParseLineIntoFields( a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);

    // if code was a comment, there is nothing to do.
    if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

    // Record whether the operands are numeric and their value if they are.
    m_IsNumericOperand1 = isStrNumber(m_Operand1);
    if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

    m_IsNumericOperand2 = isStrNumber(m_Operand2);
    if (m_IsNumericOperand2) m_Operand2NumericValue = stoi(m_Operand2);

    // For the sake of comparing, convert the op code to upper case.
    for (char& c : m_OpCode)
    {
        c = toupper(c);
    }
  
    // - Determine and record the instruction type from the op code.
    // - Recording the numberic Op code for machine lanuage equivalents.

    string Machine_OpCodes[] = { "ADD", "SUB", "MULT", "LOAD", "STORE", "ADDR", "SUBR", "MULTR", "DIVR"
                                "READ", "WRITE", "B", "BM", "BZ", "BP", "HALT" };
    string Assembler_OpCode[] = { "DC", "DS", "ORG", "END" };

    int size_M = sizeof(Machine_OpCodes) / sizeof(Machine_OpCodes[0]);
    int size_A = sizeof(Assembler_OpCode) / sizeof(Assembler_OpCode[0]);

    // to determine if machine code
    for (int i = 0; i < size_M; i++) {
        if (m_OpCode == Machine_OpCodes[i]) {
            m_type = InstructionType::ST_MachineLanguage;
            m_NumOpCode = i + 1;
            return true;
        }
    }

    // to determine if assembly instruction
    for (int i = 0; i < size_A; i++) {
        if (i < size_A - 1) {
            if (m_OpCode == Assembler_OpCode[i]) {
                m_type = InstructionType::ST_AssemblerInstr;
                return true;
            }
        }
        if (m_OpCode == Assembler_OpCode[size_A - 1]) {
            m_type = InstructionType::ST_End;
            return true;
        }
    }

    // to determine if comment
    if (m_Label.empty() && m_OpCode.empty()) {
        m_type = InstructionType::ST_Comment;
        return true;
    }

    // is a error if it is none of above
    m_type = InstructionType::ST_Error;
    return true;
}

/*
NAME: 
    ParseLineIntoFields(); - parses the instruction to its respective fields

SYNOPSIS:
    bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
    a_line          --> represents a single line of assembly line instruction, is passed by value
    &a_label        --> records the label in the instruction
    &a_OpCode       --> records the operation code in the instruction
    &a_Operand1     --> records the operand1 in the instruction
    &a_Operand2     --> records the operand2 in the instruction

DESCRIPTION:
    This boolean function is reposnsible for splitting an instruction line into label, operation code,
    and operands. All the elements are initialized to empty strings. If the assembly language line ie 
    a_line is empty, it returns false which indicates that there is nothing to parse. Else, it will replace
    all the commas ',' in the line with a blank space. An istringstream object is initialized with a_line, which
    allows to treat a_line as a stream. If the first character of the input line is a space or a tab, it implies 
    the absence of a label. So, it reads operation code, operand1 and operand2 into the respective variables. Else,
    it reads label along with the afforementioned variables. If there is still extra data, it returns true. 

*/

// Parse the intruction into fields.
bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
{
    // Initialize the statrment elements to empty strings.
    a_label = a_OpCode = a_Operand1 = a_Operand2 = "";

    // If this is an empty string, return indicating that it is OK.
    if (a_line.empty()) return false;

    // Get rid of any commas from the line.
    replace(a_line.begin(), a_line.end(), ',', ' ');

    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    string endStr;
    istringstream ins(a_line);
    if (a_line[0] == ' ' || a_line[0] == '\t')
    {
        a_label = "";
        ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    else
    {
        ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    // If there is extra data, return true.
    return ! endStr.empty() ? false : true;
}

/*
NAME: 
    isStrNumber - checks if a string has more than 2 characters
                  and if it contains any numbers. 

SYNOPSIS:
    bool isStrNumber(const string& a_str);
    &a_str      --> A string is passed by reference, and the
                    fucntion checks for any digits in the string

DESCRIPTION:
    boolean function.
    The function checks if the string is empty, using the /empty() method,
    and returns false if it is empty. Then it proceeds to check if
    the characters within '-' and '+' are more than 2 in length. Lastly,
    the function checks for any digits, and returns true if it does not 
    find any digit.

*/
bool Instruction::isStrNumber(const string& a_str)
{
    if (a_str.empty()) return false;

    // If there is a - or a plus, make sure there are more characters.
    int ichar = 0;
    if (a_str[0] == '-' || a_str[0] == '+')
    {
        ichar++;
        if (a_str.length() < 2) return false;
    }
    // Make sure that the remaining characters are all digits
    for (; ichar < a_str.length(); ichar++)
    {
        if (!isdigit(a_str[ichar])) return false;
    }
    return true;
}


/*
NAME:
    LocationNextInstruction() - Determines the next location

SYNOPSIS:
    int LocationNextInstruction(int a_loc);
        a_loc       ---> Represents the current location in the program's memory
    returns the location of the next instruction

DESCRIPTION:
    The function determines the location of the next location based on the current location's
    operation code and its operands. If the current location is either DS or ORG, it will add that to 
    location, else it will increment location by 1. 
*/

int Instruction::LocationNextInstruction(int a_loc) {
        
    if (m_OpCode == "DS" || m_OpCode == "ORG")
    {
        return a_loc + m_Operand1NumericValue;
    }
    return a_loc + 1;
}
