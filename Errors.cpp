#include "stdafx.h"

#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/*
NAME:
	InitErrorReporting() - function initializes any error report. 

SYNOPSIS:
	void Errors::InitErrorReporting()

DESCRIPTION:
	Function clears an previously recoreded error messages.

RETURN:
	void
*/

void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
}

/*
NAME:
	RecordError() - function records error messages.

SYNOPSIS:
	void Errors::RecordError(string a_emsg)

DESCRIPTION:
	The function stores all the error messages in the m_ErrorMsgs vector.

RETURN:
	void
	
*/

void Errors::RecordError(string a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
}                

/*
NAME:
	DisplayError() - This fucntion displays all the collected error messages

SYNOPSIS:
	void Errors::DisplayErrors()

DESCRIPTION:
	The function iterates over the vector of error messages and prints each one

RETURN:
	void

*/
 
void Errors::DisplayErrors() {

	for (const auto& error : m_ErrorMsgs) {
		cout << error << endl;
	}
}