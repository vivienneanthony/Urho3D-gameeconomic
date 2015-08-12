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

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;



/// Insert into a Player
bool GameEconomicServer::insertDBPlayer(PlayerObject &TempPlayer)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    PlayerObject TemporaryDBPlayer;

    /// Copy specific Players
    TemporaryDBPlayer.Health=0;
    TemporaryDBPlayer.Firstname = TempPlayer.Firstname.Trimmed();
    TemporaryDBPlayer.Middlename = TempPlayer.Middlename.Trimmed();
    TemporaryDBPlayer.Lastname = TempPlayer.Lastname.Trimmed();
    TemporaryDBPlayer.Level = TempPlayer.Level;
    TemporaryDBPlayer.Experience = TempPlayer.Experience;
    TemporaryDBPlayer.Reputation = TempPlayer.Reputation;
    TemporaryDBPlayer.Reputation1 = TempPlayer.Reputation1;
    TemporaryDBPlayer.Reputation2 = TempPlayer.Reputation2;
    TemporaryDBPlayer.Reputation3 = TempPlayer.Reputation3;
    TemporaryDBPlayer.Reputation4 = TempPlayer.Reputation4;
    TemporaryDBPlayer.Reputation5 = TempPlayer.Reputation5;
    TemporaryDBPlayer.AlienRace = TempPlayer.AlienRace;
    TemporaryDBPlayer.AlienAllianceAligned = TempPlayer.AlienAllianceAligned;
    TemporaryDBPlayer.Gender = TempPlayer.Gender;
    TemporaryDBPlayer.PersonalityTrait = TempPlayer.PersonalityTrait;
    TemporaryDBPlayer.UniqueID = TempPlayer.UniqueID.Trimmed();


    if(TemporaryDBPlayer.Firstname.Empty()
            ||TemporaryDBPlayer.Middlename.Empty()
            ||TemporaryDBPlayer.Lastname.Empty()
            ||TemporaryDBPlayer.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBPlayer(PlayerObject &TempPlayer)";
        return false;
    }
    ///StateStart
    String PreparedStatement = String("INSERT INTO Players (`PlayerFirstname`, `PlayerMiddlename`, `PlayerLastname`, `PlayerLevel`, `PlayerExperience`, `PlayerReputation`,`PlayerReputation1`,`PlayerReputation2`,`PlayerReputation3`,`PlayerReputation4`,`PlayerReputation5`,`PlayerAlienRace`,`PlayerAlienAllianceAligned`,`PlayerGender`,`PlayerPersonalityTrait`, `PlayerUniqueID`) Values (");

    /// Copy information to statement
    PreparedStatement.Append("'"+TemporaryDBPlayer.Firstname+"'");
    PreparedStatement.Append(",'"+TemporaryDBPlayer.Middlename+"'");
    PreparedStatement.Append(",'"+TemporaryDBPlayer.Lastname+"'");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Level)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Experience)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation1)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation2)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation3)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation4)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation5)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.AlienRace)+"");
    PreparedStatement.Append(","+String((unsigned int)TemporaryDBPlayer.AlienAllianceAligned)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Gender)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.PersonalityTrait)+"");
    PreparedStatement.Append(",'"+TemporaryDBPlayer.UniqueID+"'");

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

bool GameEconomicServer::deleteDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM 'Players' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values


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

bool GameEconomicServer::editDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE 'Players' SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(", ");
        }
        /// unique values
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };

        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE PlayerUniqueID='");

    PreparedStatement.Append(UniqueID);

    PreparedStatement.Append("' LIMIT 1;");

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

bool GameEconomicServer::verifyDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT PlayerIdx FROM 'Players' WHERE ");

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
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };

        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
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


Vector<String> GameEconomicServer::selectDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM Players");

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
        /// unique values
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };

        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerowneruniqueid")
        {
            PreparedStatement.Append("PlayerOwnerUniqueID = '"+TableNameParameter.At(i)+"'");
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

String GameEconomicServer::ListAllDBAccounts(Urho3D::Connection * sender)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBAccount(TableNames,TableFields);

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

String GameEconomicServer::ListAllDBPlayers(Urho3D::Connection * sender)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBPlayer(TableNames,TableFields);

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



bool GameEconomicServer::UpdateSingleDBPlayer(PlayerObject &TempPlayer)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy field
    TableNames.Push("playerfirstname");
    TableNames.Push("playermiddlename");
    TableNames.Push("playerlastname");

    /// copy parameters
    TableFields.Push(TempPlayer.Firstname);
    TableFields.Push(TempPlayer.Middlename);
    TableFields.Push(TempPlayer.Lastname);

    bool Results = editDBPlayer(TableNames,TableFields, TempPlayer.UniqueID);

    return Results;
}


bool GameEconomicServer::DeleteSingleDBPlayer(PlayerObject &TempPlayer)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("playeruniqueid");

    /// copy parameters
    TableFields.Push(TempPlayer.UniqueID);

    bool Results = deleteDBPlayer(TableNames,TableFields);

    return Results;
}




