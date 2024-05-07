//
//		Implementation of the emulator table class.  
//
#include "stdafx.h"
#include "Emulator.h"

/*
NAME: 
	emulator() - constructor for the emulator class

SYNOPSIS:
	emulator::emulator()

DESCRIPTION:
	When called, the function prompts the user with input, and reads the input. 
	This ensure that the emulator can dynamically interact with user during the execution

RETURN:
	none

*/


emulator::emulator() {
	// Initialize memory and registers to default values
	inputReader = []() -> int {
		cout << "User Input? ";
		int input;
		cin >> input;
		return input;
		};
}


/*

NAME:
	insertMemory - writes data into specfic location of the emulator's memory

SYNOPSIS:
	emulator::insertMemory(int a_location, long long a_contents);
	a_loc		--> specifies the index in the memory where data should be stored
	a_content		--> stores a large int value, hold data that needs to be stored at a specfic memory


DESCRIPTION:
	The function checks if the a_location is within the valid range of memory. MEMSZ defines the size 
	of the memory. If the location is valid, it stores the a_contents in a_location.

RETURN:
	bool - returns true if the memory insertion was successful
	
*/

bool emulator::insertMemory(int a_location, long long a_contents) {
		
	if (a_location < 0 || a_location >= MEMSZ) {
		cerr << "Error: Memory location out of bounds " << endl;
		return false;
	}

	m_memory[a_location] = a_contents;
	return true;
}

/*
NAME:
	runProgram() - reads and executes machine language instruction encoded in a specfic format
					within an emulator's memory. 

SYNOPSIS:
	emulator::runProgram();

DESCRIPTION:


OPERATIONS:
	Arithmetic Operations, Load/Store Operations, Brach Operations, Input/Output Operations,
	Control Flow

RETURN:
	bool = returns true if successful completion 

*/

bool emulator::runProgram() {

	// initializes variable that stores opcode, reg1, reg2 and address
	int counter = 0;
	int OpCode = 0;
	int reg1 = 0;
	int reg2 = 0;
	int address = 0;

	while (counter<MEMSZ) {
		long long contents = (m_memory[counter]);
		if (contents == 0) {
			counter++;
			continue;
		}


		// calculates the different calues from the content
		OpCode = contents / 10'000'000;
		reg1 = (contents / 1'000'000) % 10;
		reg2 = (contents / 100'000) % 10;
		address = contents % 1'000'000;

		try {
			switch (OpCode) {
			case1:
				// c(Reg) + c(ADDR) --> Reg
				m_reg[reg1] += m_memory[address];
				break;
			case 2:
				// c(ADDR) - c(Reg) --> Reg
				m_reg[reg1] -= m_memory[address];
				break;
			case 3:
				// c(Reg) * c(ADDR) ---> Reg
				m_reg[reg1] *= m_memory[address];
				break;
			case 4:
				// c(Reg) / c(ADDR) --> Reg
				if (m_memory[address] == 0) throw runtime_error("Error! Division by zero!");
				m_reg[reg1] /= m_memory[address];
				break;
			case 5:
				// c(ADDR) --> Reg
				m_reg[reg1] = m_memory[address];
				break;
			case 6:
				// c(Reg) --> ADDR
				m_memory[address] = m_reg[reg1];
				break;
			case 7:
				// c(reg2) + c(reg1) --> reg1
				m_reg[reg1] += m_reg[reg2];
				break;
			case 8:
				// c(reg1) - c(reg2) --> reg1
				m_reg[reg1] -= m_reg[reg2];
				break;
			case 9:
				//  c(reg1) * c(reg2) --> reg1
				m_reg[reg1] *= m_reg[reg2];
				break;
			case 10:
				//  c(reg1) / c(reg2) --> reg1
				if (m_reg[reg2] == 0) throw runtime_error("Error! Division by 0");
				m_reg[reg1] /= m_reg[reg2];
				break;
			case 11:
				// to read a value 
				m_memory[address] = inputReader();
				break;
			case 12:
				// c(ADDR) is displayed 
				cout << m_memory[address] << endl;
				break;
			case 13:
				// branch! goes to ADDR for next instruction
				counter = address;
				break;
			case 14:
				// branch if negative, goes to ADDR if c(reg) <0
				if (m_reg[reg1] < 0) {
					counter = address - 1;
				}
				break;
			case 15:
				// brach if zero, goes to ADDR if c(reg) =0
				if (m_reg[reg1] == 0) {
					counter = address - 1;
				}
				break;
			case 16:
				// branch if postive, goes to ADDR if a(reg)>0
				if (m_reg[reg1] > 0) {
					counter = address - 1;
				}
				break;
			case 17:
				// halt, terminates the execution
				counter = MEMSZ;
				break;
			default:
				throw runtime_error("Error! Invalid OpCode encountered!")
			}
		}
		catch (const exception& e) {
			cerr << "Execution error " << e.what() << endl;
			return false;
		}
		counter++;
	}
	return true;
}