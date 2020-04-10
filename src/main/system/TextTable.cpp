#include <sstream>
#include <iomanip>
#include "TextTable.h"
#include "../error/MustardException.h"

TextTable::TextTable ( int columns ) : columns ( columns )
{

}

void TextTable::addRow ( const vector<string> &row )
{
    if ( row.size() != columns ) {
        throw MustardException ( "Tried to add table entry with wrong number of columns" );
    }
    rows.push_back ( row );
}

string TextTable::getTable()
{
    vector<unsigned int >maximumColumnLength;
    for ( int c = 0; c < columns; ++c ) {
        unsigned int max = 0;
        for ( const auto &row : rows ) {
            unsigned int curlength = lengthWithoutAnsiCommands ( row[c] );
            if ( curlength > max ) {
                max = curlength;
            }
        }
        maximumColumnLength.push_back ( max );
    }

    stringstream ss;
    ss << std::left << std::setfill ( ' ' );
    for ( auto row : rows ) {
        for ( int c = 0; c < columns; ++c ) {
            unsigned int targetWidth = maximumColumnLength[c] + ( row[c].size() - lengthWithoutAnsiCommands ( row[c] ) );
            ss << setw ( targetWidth ) << row[c] << "   ";
        }
        ss << endl;
    }

    return ss.str();
}

unsigned int TextTable::lengthWithoutAnsiCommands ( const string &s )
{
    bool inCommand = false;
    unsigned int length = 0;
    for ( auto c : s ) {
        if ( !inCommand && c == '\033' ) {
            inCommand = true;
            continue;
        }
        if ( inCommand && c == 'm' ) {
            inCommand = false;
            continue;
        }
        if ( !inCommand && ( c  & 0xc0 ) != 0x80 ) {
            ++length;
        }
    }
    return length;
}
