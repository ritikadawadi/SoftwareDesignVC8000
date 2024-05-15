#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/*
NAME:

	InitErrorReporting() - function initializes any error report. 

SYNOPSIS:

	void Errors::InitErrorReporting()

DESCRIPTION:

	Function clears an previously recoreded error messages, by using clear() method.

RETURN:

	void, so returns nothing

AUTHOR:

	Ritika Dawadi

DATE:

	4:00pm 5/14/24
*/

void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
	
}

/*
NAME:

	RecordError() - function records error messages.

SYNOPSIS:

	void Errors::RecordError(string a_emsg);
	a_esmg		--> the error message that gets recorded

DESCRIPTION:

	The function stores all the error messages in the m_errormsgs vector.

RETURN:

	void, so returns nothing 

AUTHOR:

	Ritika Dawadi

DATE:

	4:00pm 5/14/24
	
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

	void, so returns nothing

AUTHOR:

	Ritika Dawadi

DATE:

	4:00pm 5/14/24

*/
 
void Errors::DisplayErrors() {

	for (const auto& error : m_ErrorMsgs) {
		cout << error << endl;
	}
}