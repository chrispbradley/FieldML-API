/* \file
 * $Id$
 * \author Caton Little
 * \brief 
 *
 * \section LICENSE
 *
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is FieldML
 *
 * The Initial Developer of the Original Code is Auckland Uniservices Ltd,
 * Auckland, New Zealand. Portions created by the Initial Developer are
 * Copyright (C) 2010 the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 */

#include "SimpleTest.h"

using namespace std;


SimpleTestException::SimpleTestException( string _message, const char *_file, const int _line )
{
    stringstream messageBuffer;
    messageBuffer << "At " <<  _file << ":" << _line << ": " << _message;
    
    message = messageBuffer.str();
}


const std::string SimpleTestException::getMessage()
{
    return message;
}


SimpleTestRecorder::SimpleTestRecorder()
{
}


void SimpleTestRecorder::reset()
{
    passed = 0;
    failed = 0;
    excepted = 0;
}


void SimpleTestRecorder::checkAndThrow( int expr, const string &message, const char *_file, const int _line )
{
    if( !expr )
    {
        excepted++;
        throw SimpleTestException( message, _file, _line );
    }
    else
    {
        passed++;
    }
}

template<> void SimpleTestRecorder::assertEquals<char *>( char *const&expected, char *const&actual, const std::string &actualName, const char *_file, const int _line )
{
    assertEquals( std::string( expected ), std::string( actual ), actualName, _file, _line );
}


template<> void SimpleTestRecorder::assertEquals<const char *>( const char *const&expected, const char *const&actual, const std::string &actualName, const char *_file, const int _line )
{
    assertEquals( std::string( expected ), std::string( actual ), actualName, _file, _line );
}


void SimpleTestRecorder::assert( int expr, const string &exprString, const char *_file, const int _line )
{
    stringstream message;

    message << "Assert failed on " << exprString << ".";
    checkAndThrow( expr, message.str(), _file, _line );
}


void SimpleTestRecorder::assertEquals( const char * const &expected, char * const &actual, const string &actualName, const char *_file, const int _line )
{
    assertEquals( string( expected ), string( actual ), actualName, _file, _line );
}


void SimpleTestRecorder::assertEquals( const int &expected, const unsigned int &actual, const string &actualName, const char *_file, const int _line )
{
    if( expected < 0 )
    {
        stringstream message;
        
        message << "Assert failed on " << actualName << ". Expected " << expected << ", got " << actual << ".";
        checkAndThrow( false, message.str(), _file, _line );
    }
    
    assertEquals( (const unsigned int)expected, actual, actualName, _file, _line );
}


void SimpleTestRecorder::assertEquals( const unsigned int &expected, const int &actual, const string &actualName, const char *_file, const int _line )
{
    if( actual < 0 )
    {
        stringstream message;
        
        message << "Assert failed on " << actualName << ". Expected " << expected << ", got " << actual << ".";
        checkAndThrow( false, message.str(), _file, _line );
    }

    assertEquals( expected, (const unsigned int)actual, actualName, _file, _line );
}


void SimpleTestRecorder::report()
{
    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
    cout << "Excepted: " << excepted << endl;
}


SimpleTest::SimpleTest( void(*_testFunction)( SimpleTestRecorder &__recorder ), const string &_name ) :
    name( _name )
{
    testFunction = _testFunction;
    tests.push_back( this );
}

vector<SimpleTest*> SimpleTest::tests;


int main( int argc, char **argv )
{
    SimpleTestRecorder recorder;

    recorder.reset();

    cout << "Running tests..." << endl;

    for( vector<SimpleTest*>::const_iterator i = SimpleTest::tests.begin(); i != SimpleTest::tests.end(); i++ )
    {
        cout << ( *i )->name << endl;
        try
        {
            ( *i )->testFunction( recorder );
        }
        catch( SimpleTestException &e )
        {
            cout << e.getMessage() << endl;
        }
    }

    cout << "Done." << endl;

    recorder.report();
}
