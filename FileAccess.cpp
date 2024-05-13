//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*

NAME:

    FileAccess - constructor function. The function initializes an instance an instance of 'FileAccess'
    class when an object is created. 

SYNOPSIS:

    FileAccess( int argc, char *argv[])
    argc        -> represents the number of command line arguments provided to the program
    *argc[]     -> reprsents an array of character pointers that consists the names of the programs to be opened


DESCRIPTION:

    "argc" will tell the program how many arguments were provided. It must be at least 2: one for the filename
    and one for the program name. ( for example: helloworld input.txt)
    "*argv[]" is an array of characters where argv[0] usually holds the name of the program itself, and argv[1]
    , and so on hold a additional arguments when running the program. The constructor checks if correct number 
    of arguments are provided, and displays message accordingly. 
    "m_sfile()" is used to read data from a file


*/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}

/*
NAME:

    ~FileAccess() - Destructor function

SYNOPSIS:

    Releases the resources when an object of FileAccess class goes out of scople using the close() function.

*/


FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}


/*
NAME:

    GetNextLine( string &a_line ) - function reads the next line from the file (m_sfile)

SYNOPSIS:
    
    bool GetNextLine( string &a_line );
    &a_line -->     reads the next line from the file an stores it in the string a_line.
  
DESCRIPTION:

   String "&a_line" is a call by reference, so any change made in the function directly affects the original.
   "eof()" represents end of file, is there is no more data, then the function returns false, else it will read
   a file from the file and return true. The function uses getline to read from the file. 
   
*/


// Get the next line from the file.
bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}


/*
NAME:


    rewind() - goes to the very beginning of the file 

SYNOPSIS:

    void rewind();
    void function that does not return anything.

DESCRIPTION:
    The function clears any error flags set on the file stream using .clear(). ios::beg indicates the beginning of the 
    file. When ios::beg is used along with seekg() on file stream, it tells the stream to the beginning 
    of the file. 0 offsets the reference point. 
    
*/

void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
    
