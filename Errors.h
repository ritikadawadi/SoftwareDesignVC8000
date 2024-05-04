//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>

class Errors {

public:
    
    // Initializes error reports.
    static void InitErrorReporting( );

    // Records an error message.
    static void RecordError( string a_emsg );

    // Displays the collected error message.
    static void DisplayErrors( );

private:

    static vector<string> m_ErrorMsgs;  // This must be declared in the .cpp file.  Why?
    
};
#endif

/*
Static member functions can be called using class name without creating an object of that class. 
So, it allows the error messages to be recorded and accessed without needing an instance of the 'Errors' class.

*/