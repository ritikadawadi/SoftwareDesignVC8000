#include "stdafx.h"

#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/*
NAME:
	

SYNOPSIS:
	void Errors::InitErrorReporting()

DESCRIPTION:
	Function clears an previously recoreded error messages.
*/

void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
}
