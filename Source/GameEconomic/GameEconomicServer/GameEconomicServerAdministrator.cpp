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
#include "../Accounts.h"
#include "../Administrator.h"
#include "../GameEconomicServer/Networking.h"

#include "signalHandler.hpp"
#include <pthread.h>

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;



/// Insert into a Administrator
bool GameEconomicServer::insertDBAdministrator(AdministratorInformation &TempAdministrator)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    AdministratorInformation TemporaryDBAdministrator;

    /// Copy specific Administrators
    TemporaryDBAdministrator.Idx=0;
    TemporaryDBAdministrator.Username = TempAdministrator.Username.Trimmed();
    TemporaryDBAdministrator.Password= TempAdministrator.Password.Trimmed();
    TemporaryDBAdministrator.UniqueID = TempAdministrator.UniqueID.Trimmed();
    TemporaryDBAdministrator.AdminAccounts=TempAdministrator.AdminAccounts;

    if(TemporaryDBAdministrator.Username.Empty()
            ||TemporaryDBAdministrator.Password.Empty()
            ||TemporaryDBAdministrator.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBAdministrator(AdministratorInformation &TempAdministrator)";
        return false;
    }

    cout << "it got here" << endl;

    ///StateStart
    ///String PreparedStatement = String("INSERT INTO Administrators ( `AdministratorUsername`, `AdministratorPassword`, `AdministratorUniqueID`,`AdministratorAdminAccounts`) VALUES (");
    String PreparedStatement = String("INSERT INTO Administrators (AdministratorUsername, AdministratorPassword, AdministratorUniqueID,AdministratorAdminAccounts) VALUES (");

    /// Copy information to statement
    PreparedStatement.Append("'"+TemporaryDBAdministrator.Username+"'");
    PreparedStatement.Append(",'"+TemporaryDBAdministrator.Password+"'");
    PreparedStatement.Append(",'"+TemporaryDBAdministrator.UniqueID+"'");
    PreparedStatement.Append(","+String(TemporaryDBAdministrator.AdminAccounts)+"");

    /// Create the rest of the statement
    PreparedStatement.Append(");");

    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB ->executeQuery(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::deleteDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM Administrators WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="administratorusername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratoruniqueid")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" LIMIT 1;");

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

bool GameEconomicServer::editDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE Administrators SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i>0)
        {
            PreparedStatement.Append(",");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="administratorusername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratorpassword")
        {
            PreparedStatement.Append("AdministratorPassword = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratoruniqueid")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE AdministratorUniqueID='");

    PreparedStatement.Append(UniqueID);

    PreparedStatement.Append("' LIMIT 1;");

    /// Change statement to string
    string mysqlpreparedstatement = string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::verifyDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT AdministratorIdx FROM 'Administrators' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="administratorUsername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratorUniqueID")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
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


Vector<String> GameEconomicServer::selectDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM Administrators");

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
        /// unique values
        if(TableName.At(i).ToLower()=="administratorusername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratoruniqueid")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    cout << PreparedStatement.CString() << endl;

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    Results = connectionDB -> executePreparedStatementResult(mysqlpreparedstatement);


    return Results;
}



String GameEconomicServer::ListAllDBAdministrators(void)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBAdministrator(TableNames,TableFields);

    TableNames.Clear();
    TableFields.Clear();

    String ResultsString;

    for(unsigned int i=0; i<Results.Size(); i++)
    {
        /// Append
        ResultsString.Append(" "+Results.At(i));

    }

    return ResultsString;
}

bool GameEconomicServer::UpdateSingleDBAdministrator(AdministratorInformation &TempAdministrator)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("administratorusername");
    TableNames.Push("administratorpassword");

    /// copy parameters
    TableFields.Push(TempAdministrator.Username);
    TableFields.Push(TempAdministrator.Password);

    bool Results = editDBAdministrator(TableNames,TableFields, TempAdministrator.UniqueID);

    return Results;
}

bool GameEconomicServer::DeleteSingleDBAdministrator(AdministratorInformation &TempAdministrator)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("administratoruniqueid");

    /// copy parameters
    TableFields.Push(TempAdministrator.UniqueID);

    bool Results = deleteDBAdministrator(TableNames,TableFields);

    return Results;
}
