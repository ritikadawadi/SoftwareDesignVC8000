
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
// parse an instruction and return its type.  Yes, I want a call by value.  Why??
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

    RecordFields();

SYNOPSIS:

    bool RecordFields( const string &a_line );
    &a_line     --> a_line is pass by reference here, as it modifies the 
                    line according to its content type.

DESCRIPTION:
        
        This function calls several other functions of the Instruction class.
        
        The function calls isStrNumber on operand 1 and operand 2, and if it 
        returns true it uses stoi() function from standard library. stoi() converts a
        string represting an integer to an integer type.


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
    // I leave to the class the tasks:
    // - Determine and record the instruction type from the op code.
    // - Recording the numberic Op code for machine lanuage equivalents.
    return isFormatError;
}

/*
NAME: 
    ParseLineIntoFields();

SYNOPSIS:
    bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
    a_line          -->
    &a_label        -->
    &a_OpCode       -->
    &a_Operand1     -->
    &a_Operand2     -->

DESCRIPTION:
    

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
