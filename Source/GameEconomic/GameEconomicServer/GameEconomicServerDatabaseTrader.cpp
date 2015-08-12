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

#include "signalHandler.hpp"
#include <pthread.h>

#include "../GameEconomicComponents/Trader.h"
#include "../GameEconomicComponents/Market.h"

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;



using namespace std;
using namespace Urho3D;


/// Insert into a Trader
bool GameEconomicServer::insertDBTrader(TraderInformation &TempTrader)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    TraderInformation TemporaryDBTrader;


    /// Copy specific Traders
    TemporaryDBTrader.Name = TempTrader.Name.Trimmed();
    TemporaryDBTrader.CargoBay= TempTrader.CargoBay.Trimmed();
    TemporaryDBTrader.UniqueID = TempTrader.UniqueID.Trimmed();
    TemporaryDBTrader.Credits=TempTrader.Credits;
    TemporaryDBTrader.TraderType=TempTrader.TraderType;
    TemporaryDBTrader.AlienRace = TempTrader.AlienRace;
    TemporaryDBTrader.AlienAllianceAligned = TempTrader.AlienAllianceAligned;

    if(TemporaryDBTrader.Name.Empty()
            ||TemporaryDBTrader.CargoBay.Empty()
            ||TemporaryDBTrader.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBTrader(TraderInformation &TempTrader)";
        return false;
    }


    ///StateStart
    String PreparedStatement = String("INSERT INTO Traders (`TraderType`, Name`,`TraderAlienRace`,`TraderAlienAllianceAligned`,`TraderCargoBay`, `TraderCredits`,`TraderUniqueID`)VALUES (");

    /// Copy information to statement
    PreparedStatement.Append(""+String(TemporaryDBTrader.TraderType)+"");
    PreparedStatement.Append(",'"+TemporaryDBTrader.Name+"'");
    PreparedStatement.Append(","+String(TemporaryDBTrader.AlienRace)+"");
    PreparedStatement.Append(","+String((unsigned int)TemporaryDBTrader.AlienAllianceAligned)+"");
    PreparedStatement.Append(",'"+TemporaryDBTrader.CargoBay+"'");
    PreparedStatement.Append(","+String(TemporaryDBTrader.Credits)+"");
    PreparedStatement.Append(",'"+TemporaryDBTrader.UniqueID+"");

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

bool GameEconomicServer::deleteDBTrader(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM Traders WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="traderusername")
        {
            PreparedStatement.Append("TraderUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="traderuniqueid")
        {
            PreparedStatement.Append("TraderUniqueID = '"+TableNameParameter.At(i)+"'");
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

bool GameEconomicServer::editDBTrader(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE Traders SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i>0)
        {
            PreparedStatement.Append(",");
        }
        /// unique values
        /// unique values
      if(TableName.At(i).ToLower()=="traderusername")
        {
            PreparedStatement.Append("TraderUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="traderuniqueid")
        {
            PreparedStatement.Append("TraderUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE TraderUniqueID='");

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

bool GameEconomicServer::verifyDBTrader(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT TraderIdx FROM 'Traders' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="traderusername")
        {
            PreparedStatement.Append("TraderUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="traderuniqueid")
        {
            PreparedStatement.Append("TraderUniqueID = '"+TableNameParameter.At(i)+"'");
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


/// select trader
Vector<String> GameEconomicServer::selectDBTrader(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM Traders");

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
        if(TableName.At(i).ToLower()=="tradername")
        {
            PreparedStatement.Append("Name = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="traderuniqueid")
        {
            PreparedStatement.Append("TraderUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    Results = connectionDB -> executePreparedStatementResult(mysqlpreparedstatement);

    return Results;
}


/// List all traders
Vector<String> GameEconomicServer::ListAllDBTraders(void)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBTrader(TableNames,TableFields);

    return Results;
}
