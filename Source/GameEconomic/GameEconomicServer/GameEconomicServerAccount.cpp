//
// Copyright (c) 2008-2015 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/Urho3D.h>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Core/Thread.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>

#include <iostream>
#include <signal.h>
#include <sstream>
#include <fstream>
#include <vector>

#include "GameEconomicServer.h"

#include "../GameEconomicComponents/ServerConsoleInterface.h"
#include "../GameEconomicComponents/connectorDB.h"
#include "../GameEconomicComponents/Player.h"
#include "../GameEconomicComponents/Accounts.h"
#include "../Administrator.h"
#include "../Networking.h"



#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;


/// Insert into a account
bool GameEconomicServer::insertDBAccount(AccountInformation &TempAccount)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// time related variables
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    /// get current time
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    /// convert time
    strftime (buffer,40,"%Y%m%d %r",timeinfo);

    /// temporary user
    AccountInformation TemporaryDBAccount;

    /// Copy specific accounts
    TemporaryDBAccount.Idx=0;
    TemporaryDBAccount.Username = TempAccount.Username.Trimmed();
    TemporaryDBAccount.Firstname = TempAccount.Firstname.Trimmed();
    TemporaryDBAccount.Middlename = TempAccount.Middlename.Trimmed();
    TemporaryDBAccount.Lastname = TempAccount.Lastname.Trimmed();
    TemporaryDBAccount.Email = TempAccount.Email.Trimmed();
    TemporaryDBAccount.Password = TempAccount.Password.Trimmed();
    TemporaryDBAccount.UniqueID = TempAccount.Lastname.Trimmed()+TempAccount.Firstname.Trimmed();

    if(TemporaryDBAccount.Username.Empty()
            ||TemporaryDBAccount.Firstname.Empty()
            ||TemporaryDBAccount.Middlename.Empty()
            ||TemporaryDBAccount.Lastname.Empty()
            ||TemporaryDBAccount.Email.Empty()
            ||TemporaryDBAccount.Password.Empty()
            ||TemporaryDBAccount.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBAccount(AccountInformation &TempAccount)";
        return false;
    }

    /// Convert time to buffer
    String DateTime(buffer);

    /// Now create the SQL Statement

    ///StateStart
    String PreparedStatement = String("INSERT INTO `Accounts` (`AccountUsername`, `AccountFirstname`, `AccountMiddlename`, `AccountLastname`, `AccountEmail`, `AccountPassword`, `AccountLastLogin`, `AccountUniqueIdx`) VALUES(");

    /// Copy information to statement
    PreparedStatement.Append("'"+TemporaryDBAccount.Username+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Firstname+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Middlename+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Lastname+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Email+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Password+"'");
    PreparedStatement.Append(",'"+DateTime+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.UniqueID+"'");

    /// Create the rest of the statement
    PreparedStatement.Append(");");

    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::deleteDBAccount(Vector<String> TableName,Vector<String> TableNameParameter)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("DELETE FROM Accounts WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }

        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    cout << PreparedStatement.CString() << endl;

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::editDBAccount(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();


    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }



    /// Prepare statement
    String PreparedStatement("UPDATE Accounts SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(", ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE AccountUniqueID='");

    PreparedStatement.Append(UniqueID);

    PreparedStatement.Append("' LIMIT 1;");

    cout << PreparedStatement.CString() << endl;

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::verifyDBAccount(Vector<String> TableName,Vector<String> TableNameParameter)
{


    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();


    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("SELECT AccountIdx FROM 'Accounts' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }


    return true;
}

Vector<String> GameEconomicServer::selectDBAccount(Vector<String> TableName,Vector<String> TableNameParameter)
{

    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// Set up a single statement
    bool multipletablenames=false;

    Vector<String> Results;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return Results;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("SELECT * FROM Accounts");

    if(TableName.Size()!=0)
    {
        PreparedStatement.Append(" WHERE ");
    }

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    cout << PreparedStatement.CString() << endl;

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    Results = connectionDB -> executePreparedStatementResult(mysqlpreparedstatement);

    for(unsigned int i=0; i<Results.Size(); i ++)
    {
        cout << "r:" <<Results.At(i).CString() << endl;
    }


    return Results;
}


bool GameEconomicServer::UpdateSingleDBAccount(AccountInformation &TempAccount)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("accountusername");
    TableNames.Push("accountfirstname");
    TableNames.Push("accountmiddlename");
    TableNames.Push("accountlastname");
    TableNames.Push("accountemail");
    TableNames.Push("accountpassword");

    /// copy parameters
    TableFields.Push(TempAccount.Username);
    TableFields.Push(TempAccount.Firstname);
    TableFields.Push(TempAccount.Middlename);
    TableFields.Push(TempAccount.Lastname);
    TableFields.Push(TempAccount.Email);
    TableFields.Push(TempAccount.Password);

    bool Results = editDBAccount(TableNames,TableFields, TempAccount.UniqueID);

    return Results;
}

bool GameEconomicServer::DeleteSingleDBAccount(AccountInformation &TempAccount)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("accountuniqueid");

    /// copy parameters
    TableFields.Push(TempAccount.UniqueID);

    bool Results = deleteDBAccount(TableNames,TableFields);

    return Results;
}
