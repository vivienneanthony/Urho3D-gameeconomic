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
#include "GameEconomicServerDatabaseGeneral.h"

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
Vector<String> GameEconomicServer::selectDBGeneral(Vector<String> TableName,Vector<String> TableNameParameter, accessdbtable databasetable )
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

    String PreparedStatement("SELECT * FROM ");

    /// access dbatable type
    switch(databasetable)
    {
    case AccessTable_Faction:
        PreparedStatement.Append(String("Factions"));
        break;
    case AccessTable_AlienRace:
        PreparedStatement.Append(String("AlienRaces"));
        break;
    case AccessTable_Starbase:
        PreparedStatement.Append(String("Starbases"));
        break;
    }

    /// If more then one item
    if(TableName.Size()!=0)
    {
        PreparedStatement.Append(" WHERE ");
    }

    /// Loop through each parameter
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(", ");
        }

        /// Switch uniqueid based on the table being used
        switch(databasetable)
        {
        case AccessTable_Faction:
        {
            /// unique values
            if(TableName.At(i).ToLower()==String("factionuniqueid"))
            {
                PreparedStatement.Append("FactionUniqueID = '"+TableNameParameter.At(i)+"'");
            };
        }
        break;
        case AccessTable_AlienRace:
        {
            /// unique values
            if(TableName.At(i).ToLower()==String("alienracealignedfaction"))
            {
                PreparedStatement.Append("AlienRaceAlignedFaction = '"+TableNameParameter.At(i)+"'");
            };
            /// unique values
            if(TableName.At(i).ToLower()==String("alienraceuniqueid"))
            {
                PreparedStatement.Append("AlienRaceUniqueID = '"+TableNameParameter.At(i)+"'");
            };

        }
        break;

        case AccessTable_Starbase:
        {
            /// unique values
            if(TableName.At(i).ToLower()==String("starbaseowner"))
            {
                PreparedStatement.Append("StarbaseOwner = '"+TableNameParameter.At(i)+"'");
            };
            /// unique values
            if(TableName.At(i).ToLower()==String("starbaseuniqueid"))
            {
                PreparedStatement.Append("StarbaseUniqueID = '"+TableNameParameter.At(i)+"'");
            };
        }
        break;
        }
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

/// Get Faction Information
Vector<FactionInformation> * GameEconomicServer::GetAllFactionsDBFactions(void)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// Return player information
    Vector <FactionInformation> * ReturnFactions = NULL;

    /// create a bew player
    ReturnFactions = new Vector<FactionInformation>();

    /// Select Faction
    Vector<String> Results = selectDBGeneral(TableNames,TableFields, AccessTable_Faction);

    /// If no records found
    if(Results.At(1)==String("0"))
    {
        /// return null if player is not found

        delete ReturnFactions;

        return NULL;
    }

    /// Convert rows to 1
    unsigned int NumberRows = atoi(Results.At(1).CString());
    unsigned int NumberCols = atoi(Results.At(0).CString());

    /// loop through
    for(unsigned int i=0; i<NumberRows; i++)
    {
        /// find index
        unsigned int index=(i*NumberCols)+3;

        FactionInformation TempFaction;

        /// copy information
        TempFaction.Name = Results.At(index);
        TempFaction.Prefix = Results.At(index+1);
        TempFaction.Description = Results.At(index+2);
        TempFaction.UniqueID= Results.At(index+3);

        /// copy data to temp player
        ReturnFactions->Push(TempFaction);
    }

    return ReturnFactions;
}

/// Get AlienRace Information
Vector<AlienRaceInformation> * GameEconomicServer::GetAlienRacesDBAlienRaces(String SelectFaction)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// Return player information
    Vector <AlienRaceInformation> * ReturnAlienRaces = NULL;

    /// create a bew player
    ReturnAlienRaces = new Vector<AlienRaceInformation>();

    /// if something was selectioned
    if(!SelectFaction.Empty())
    {
        TableNames.Push(String("alienracealignedfaction"));
        TableFields.Push(SelectFaction);

        /// cout << "pushed here" << endl;

    }
    /// Select AlienRace
    Vector<String> Results = selectDBGeneral(TableNames,TableFields, AccessTable_AlienRace);

    /// If no records found
    if(Results.At(1)==String("0"))
    {
        /// return null if player is not found

        delete ReturnAlienRaces;

        return NULL;
    }

    /// Convert rows to 1
    unsigned int NumberRows = atoi(Results.At(1).CString());
    unsigned int NumberCols = atoi(Results.At(0).CString());

    /// loop through
    for(unsigned int i=0; i<NumberRows; i++)
    {
        /// find index
        unsigned int index=(i*NumberCols)+3;

        AlienRaceInformation TempAlienRace;

        /// copy information
        TempAlienRace.Name = Results.At(index);
        TempAlienRace.Prefix = Results.At(index+1);
        TempAlienRace.Description = Results.At(index+2);
        TempAlienRace.AlignedFaction= Results.At(index+3);
        TempAlienRace.UniqueID= Results.At(index+4);

        /// copy data to temp player
        ReturnAlienRaces->Push(TempAlienRace);
    }

    return ReturnAlienRaces;
}



/// Get Player Information
String GameEconomicServer::ConnectionGetAlienRacesDBAccount(String AlignedFactions)
{
    /// Create empty tables and fields and get results
    String ReturnString;

    /// Return player information
    Vector <AlienRaceInformation>  * ReturnAlienRaces;

    ReturnAlienRaces=GetAlienRacesDBAlienRaces(AlignedFactions);

    /// Copy Cols and Row Counts
    ReturnString.Append("|");

    /// If no results return 0|0
    if(ReturnAlienRaces==NULL)
    {
        ReturnString.Append("0|0");

        return ReturnString;
    }

    if(ReturnAlienRaces->Size()>0)
    {
        ReturnString.Append("5");
    }
    else
    {
        ReturnString.Append("0");
    }

    ReturnString.Append("|");
    ReturnString.Append(String(ReturnAlienRaces->Size()));

    /// If blank return nothing
    if(ReturnAlienRaces->Size()==0)
    {
        return ReturnString;
    }

    /// Transform to string
    for(unsigned int i=0; i<ReturnAlienRaces->Size(); i++)
    {
        /// GetResults and copy it
        ReturnString.Append("|");
        ReturnString.Append(ReturnAlienRaces->At(i).Name);

        ReturnString.Append("|");
        ReturnString.Append(ReturnAlienRaces->At(i).Prefix);

        ReturnString.Append("|");
        ReturnString.Append(ReturnAlienRaces->At(i).Description);

        ReturnString.Append("|");
        ReturnString.Append(ReturnAlienRaces->At(i).AlignedFaction);

        ReturnString.Append("|");
        ReturnString.Append(ReturnAlienRaces->At(i).UniqueID);
    }

    return ReturnString;
}

StarbaseInformation * GameEconomicServer::GetStarbaseDBStarbase(String OwnerID)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    TableNames.Push("starbaseowner");
    TableFields.Push(OwnerID);

    /// Return player information
    StarbaseInformation * ReturnStarbase = NULL;

    /// create a bew player
    ReturnStarbase = new StarbaseInformation();

    /// Select Starbase
    Vector<String> Results = selectDBGeneral(TableNames,TableFields, AccessTable_Starbase);

    /// If no records found
    if(Results.At(1)==String("0"))
    {
        /// return null if player is not found

        delete ReturnStarbase;

        return NULL;
    }

    /// Convert rows to 1
    unsigned int NumberRows = atoi(Results.At(1).CString());
    unsigned int NumberCols = atoi(Results.At(0).CString());

    /// loop through
    for(unsigned int i=0; i<NumberRows; i++)
    {
        /// find index
        unsigned int index=(i*NumberCols)+3;

        StarbaseInformation TempStarbase;

        /// copy information
        ReturnStarbase->Name = Results.At(index);
        ReturnStarbase->MapData = Results.At(index+1);
        ReturnStarbase->Owner = Results.At(index+2);
        ReturnStarbase->UniqueID = Results.At(index+3);
    }

    return ReturnStarbase;
}


String GameEconomicServer::ConnectionGetDBStabase(String OwnerID)
{
    /// Create empty tables and fields and get results
    String ReturnString;

    /// Return player information
    StarbaseInformation  * ReturnStarbase;

    ReturnStarbase=GetStarbaseDBStarbase(OwnerID);

    /// Copy Cols and Row Counts
    ReturnString.Append("|");


    /// If no results return 0|0
    if(ReturnStarbase==NULL)
    {
        ReturnString.Append("0|0");

        return ReturnString;
    }

    ReturnString.Append("|");
    ReturnString.Append(String("1"));

    /// GetResults and copy it
    ReturnString.Append("|");
    ReturnString.Append(ReturnStarbase->Name);

    ReturnString.Append("|");
    ReturnString.Append(ReturnStarbase->MapData);

    ReturnString.Append("|");
    ReturnString.Append(ReturnStarbase->Owner);

    ReturnString.Append("|");
    ReturnString.Append(ReturnStarbase->UniqueID);

    return ReturnString;
}

/// Get Player Information

String GameEconomicServer::ConnectionGetAllDBFactions(void)
{
    /// Create empty tables and fields and get results
    String ReturnString;

    /// Return player information
    Vector <FactionInformation>  * ReturnFactions;


    ReturnFactions=GetAllFactionsDBFactions();

/// Copy Cols and Row Counts
    ReturnString.Append("|");


    /// If no results return 0|0
    if(ReturnFactions==NULL)
    {
        ReturnString.Append("0|0");

        return ReturnString;
    }


    if(ReturnFactions->Size()>0)
    {
        ReturnString.Append("4");
    }
    else
    {
        ReturnString.Append("0");
    }

    ReturnString.Append("|");
    ReturnString.Append(String(ReturnFactions->Size()));

    /// If blank return nothing
    if(ReturnFactions->Size()==0)
    {
        return ReturnString;
    }

    /// Transform to string
    for(unsigned int i=0; i<ReturnFactions->Size(); i++)
    {
        /// GetResults and copy it
        ReturnString.Append("|");
        ReturnString.Append(ReturnFactions->At(i).Name);

        ReturnString.Append("|");
        ReturnString.Append(ReturnFactions->At(i).Prefix);

        ReturnString.Append("|");
        ReturnString.Append(ReturnFactions->At(i).Description);

        ReturnString.Append("|");
        ReturnString.Append(ReturnFactions->At(i).UniqueID);

    }

    return ReturnString;
}


/// Insert into a account
bool GameEconomicServer::insertDBGeneral(Vector<String> ColumnType,Vector<String> ColumnTypeParameter, accessdbtable databasetable)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// time related variables
    //time_t rawtime;
    //struct tm * timeinfo;
    //char buffer [80];

    /// get current time
    //time (&rawtime);
    //timeinfo = localtime (&rawtime);

    /// convert time
    //strftime (buffer,40,"%Y%m%d %r",timeinfo);

    bool multipleColumnTypes = false;
    String PreparedStatement;


    /// Clear string
    PreparedStatement.Empty();

    /// Table name does not match
    if(ColumnType.Size()==0||ColumnTypeParameter.Size()==0)
    {
        return false;
    }

    /// Table name does not match
    if(ColumnType.Size()!=ColumnTypeParameter.Size())
    {
        return false;
    }

    /// Muiltiple parameters
    if(ColumnType.Size()>1)
    {
        multipleColumnTypes=true;
    }

    /// access dbatable type
    switch(databasetable)
    {
    case AccessTable_Starbase:
        PreparedStatement.Append(String("INSERT INTO Starbases (`StarbaseName`, `StarbaseMapData`, `StarbaseOwner`, `StarbaseUniqueID`) VALUES ("));
        break;
    default:
        return false;
    }

    /// loop through each
    for(unsigned int i=0; i<ColumnType.Size(); i++)
    {
        /// seatch through multiple table names
        if(multipleColumnTypes)
        {
            PreparedStatement.Append(",");
        }

        if(ColumnType.At(i).ToLower()==String("string"))
        {
            PreparedStatement.Append("'"+ColumnTypeParameter.At(i)+"'");
        }
        else if(ColumnType.At(i).ToLower()==String("integer"))
        {
            PreparedStatement.Append(""+ColumnTypeParameter.At(i)+"");
        }
    }

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

/// set null
bool GameEconomicServer::ConnnectInsertBaseDBStarbase(Vector<String>ColumnType,Vector<String> ColumnTypeParameter)
{
    /// set int
    bool results=false;

    /// Table name does not match
    if(ColumnType.Size()==0||ColumnTypeParameter.Size()==0)
    {
        return results;
    }

    /// Table name does not match
    if(ColumnType.Size()!=ColumnTypeParameter.Size())
    {
        return results;
    }

    /// set specific variable uniqueid
    results = insertDBGeneral(ColumnType,ColumnTypeParameter, AccessTable_Starbase);

    return results;
}
