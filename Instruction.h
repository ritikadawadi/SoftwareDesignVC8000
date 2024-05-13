//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction() {};
    ~Instruction() { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum class InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End,                 // end instruction.
        ST_Error                // Statement has an error.
    };

    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);
    
    int LocationNextInstruction(int a_loc);

    //getter functions

    inline string& GetLabel() {

        return m_Label;
    };
    
    inline bool isLabel() {

        return !m_Label.empty();
    };

    inline string& GetInstruction() {
        return m_instruction;
    };

    inline string& GetOperand1() {
        return m_Operand1;
    };

    inline string& GetOperand2() {
        return m_Operand2;
    };

    inline string& GetOpCode() {
        return m_OpCode;
    };

    inline bool IsNumericOperand1() {
        return m_IsNumericOperand1;
    };

    inline bool IsNumericOperand2() {
        return m_IsNumericOperand2;
    };
    inline int GetNumOpCode() {
        return m_NumOpCode;
    };

private:

    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand1;     // The first operand. 
    string m_Operand2;     // The second operand.


    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode = 0;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type = InstructionType::ST_Error; // The type of instruction.

    bool m_IsNumericOperand1 = false;// == true if the operand 1 is numeric.
    int m_Operand1NumericValue = 0;   // The value of the operand 1 if it is numeric.

    bool m_IsNumericOperand2 = false;// == true if the operand 2 is numeric.
    int m_Operand2NumericValue = 0;   // The value of the operand 2 if it is numeric.
    
    // Deletes Comments
    void DeleteComment(string& a_line);

    // Record the fields of the instructions.
    bool RecordFields(const string& a_line);

    // Get the fields that make up the statement.  This function returns false if there
    // are extra fields.
    bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
        string& a_Operand1, string& a_Operand2);

    // Check if a string contains a number. 
    bool isStrNumber(const string& a_str);

};