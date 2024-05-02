//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*
NAME
    
    DisplaySymbolTable - displays the symbols of the symbol table

SYNOPSIS

    DisplaySymbolTable();

DESCRIPTION
    
    This function will print all the symbols that are stored in the map: m_symbolTable. 
    It is a void function so it does not return anything. An iterator is used to loop through the 
    map. 

*/

void 
SymbolTable::DisplaySymbolTable()
{
    int count = 0;
    cout << "The Symbol Table: " << endl;
    cout << "-----------------------------" << endl;
    cout << "Symbol No. \t\t Symbol \t\t Location" << endl;
    for (auto it = m_symbolTable.begin(); it != m_symbolTable.end(); ++it)
    {
        cout << count++ << it->first << it->second << endl;
    }
    cout << "-----------------------------" << endl;
}

/*
NAME
    LoopupSymbol - looks up the requires symbol in the symbol table map.

SYNOPSIS
    LookupSymbol(const string& a_symbol, int& a_loc)
    where 
    a_symbol	-> The name of the symbol to search in the symbol table.
    a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will look up the symbols that are stored in the map: m_symbolTable. 
    It is a boolean functon so it will return true if the symbol is found, else it will return false.
    The find function in the std::map is used to search for a specfic key in the map.
    it will return an iterator pointing to that element if found, else it will returns false.
*/

bool
SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
    auto it = m_symbolTable.find(a_symbol);

    if (it != m_symbolTable.end())
    {
        a_loc = it->second;
        return true;
    }
    else {
        return false;
    }
}