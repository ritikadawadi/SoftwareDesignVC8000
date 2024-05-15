//
//		Implementation of the Emulator table class.  
//
#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"
#include <string>

 

/*

NAME:

	insertMemory - writes data into specfic location of the Emulator's memory

SYNOPSIS:

	Emulator::insertMemory(int a_location, long long a_contents);
	a_loc		    --> specifies the index in the memory where data should be stored
	a_content		--> stores a large int value, hold data that needs to be stored at a specfic memory


DESCRIPTION:

	The function checks if the a_location is within the valid range of memory. MEMSZ
    defines the size of the memory. If the location is valid, it stores the a_contents in a_location.

RETURN:

	bool - returns true if the memory insertion was successful

AUTHOR:

    Ritika Dawadi

DATE:

    4:00pm 5/14/24
	
*/

bool Emulator::insertMemory(int a_location, long long a_contents) {
		
	//a_location = a_location + 1;
	if ( a_location >= 0 && a_location < MEMSZ) {
		m_memory[a_location] = a_contents;
        return true;
	}
	else {
		// If invalid, returns false
		return false;
	}
}

/*
NAME:

	runProgram() - reads and executes machine language instruction encoded in a specfic format
					within an Emulator's memory. 

SYNOPSIS:

	Emulator::runProgram();

DESCRIPTION:

    The runProgram() function executes machine language instructions
    stored in memory. It iterates through each instruction, performing arithmetic,
    load/store, branch, input/output, and control flow operations based on OpCode values.
    It manages program execution, updates the Emulator's state, and handles errors, 
    returning true if the program completes successfully.


OPERATIONS:

	Arithmetic Operations, Load/Store Operations, Brach Operations, Input/Output Operations,
	Control Flow

RETURN:

	bool = returns true if successful completion 

AUTHOR:

    Ritika Dawadi

DATE:

    4:00pm 5/14/24

*/

bool Emulator::runProgram() {
    // initializing variables that stores opcode, reg1, reg2, and address
    int OpCode = 0;
    int reg1 = 0;
    int reg2 = 0;
    int address = 0;

    cout << endl;
    cout << "Running the Emulator, Ritika's version" << endl;

    try {
        // run until the memory ends
        for (int i = 0; i < MEMSZ; i++) {
            long long contents = (m_memory[i]);
            if (contents == 0) {
                continue;
            }

            // calculating different values from the content
            OpCode = static_cast<int>(contents / 10'000'000);
            reg1 = (contents / 1'000'000) % 10;
            reg2 = (contents / 100'000) % 10;
            address = contents % 1'000'000;

            switch (OpCode) {
            case 1:
                // Reg <-- c(Reg) + c(ADDR) 
                m_reg[reg1] += m_memory[address];
                break;
            case 2:
                // Reg <-- c(Reg) - c(ADDR)
                m_reg[reg1] -= m_memory[address];
                break;
            case 3:
                // Reg <-- c(Reg) * c(ADDR)
                m_reg[reg1] *= m_memory[address];
                break;
            case 4:
                // Reg <-- c(Reg) / c(ADDR)
                m_reg[reg1] /= m_memory[address];
                break;
            case 5:
                // Reg <-- c(ADDR)
                m_reg[reg1] = m_memory[address];
                break;
            case 6:
                // ADDR <-- c(Reg)
                m_memory[address] = m_reg[reg1];
                break;
            case 7:
                // REG1 <--c(REG1) + c(REG2)
                m_reg[reg1] += m_reg[reg2];
                break;
            case 8:
                // REG1 <--c(REG1) - c(REG2)  
                m_reg[reg1] -= m_reg[reg2];
                break;
            case 9:
                // REG1 <--c(REG1) * c(REG2) 
                m_reg[reg1] *= m_reg[reg2];
                break;
            case 10:
                // REG1 <--c(REG1) / c(REG2)  
                m_reg[reg1] /= m_reg[reg2];
                break;
            case 11:
                // A line is read in and the number found there is recorded
                // in the specified memory address.
                cout << "Enter: " << endl;
                int userInput;
                cout << "? ";
                cin >> userInput;
                if (userInput < MEMSZ) {
                    m_memory[address] = userInput;
                }
                else {
                    cout << "Too large value" << endl;
                }
                break;
            case 12:
                // c(ADDR) is displayed  The register value is ignored.
                cout << m_memory[address] << endl;
                break;
            case 13:
                // go to ADDR for the next instruction.  The register value is ignored.
                i = address;
                break;
            case 14:
                // go to ADDR if c(Reg) < 0
                if (m_reg[reg1] < 0) {
                    i = address - 1;
                }
                break;
            case 15:
                // go to ADDR if c(Reg) = 0
                if (m_reg[reg1] == 0) {
                    i = address - 1;
                }
                break;
            case 16:
                // go to ADDR if c(Reg) > 0
                if (m_reg[reg1] > 0) {
                    i = address - 1;
                }
                break;
            case 17:
                // terminate execution.  The register value and address are ignored.
                i = MEMSZ;
                break;
            default:
                Errors::RecordError("Error! Error in OpCode!!");
                Errors::DisplayErrors();
                return false;
            }

        }
    }
    catch (...) {
        cout << "Error! Invalid OpCode!";
        return false;
    }

    return false;
}
