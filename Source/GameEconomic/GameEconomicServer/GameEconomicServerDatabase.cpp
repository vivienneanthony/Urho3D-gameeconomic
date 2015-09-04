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

/// verify
bool GameEconomicServer::VerifyIdentityDB(DBTable mode, String Username, String Password)
{
    Vector<String> TableNames;
    Vector<String> TableFields;

    if(mode==DBAccount)
    {
        TableNames.Push("accountusername");

        TableFields.Push(Username);

        /// if a valid email was given
        if(EmailValidCheck(Username)==true)
        {
            TableNames.At(0) = String("accountemail");
        }
    }
    else if(mode==DBAdministrator)
    {
        TableNames.Push("administratorusername");

        TableFields.Push(Username);
    }
    else
    {
        return false;
    }

    Vector<String> Results;

    if(mode==DBAccount)
    {
        Results = selectDBAccount(TableNames,TableFields);
    }
    else if(mode==DBAdministrator)
    {
        Results = selectDBAdministrator(TableNames,TableFields);
    }

    /// If no results return
    if(Results.At(1)==String("0"))
    {
        return false;
    }

    String ResultsPassword;

    if(mode==DBAccount)
    {
        ResultsPassword=Results.At(8);
    }

    if(mode==DBAdministrator)
    {
        ResultsPassword=Results.At(4);
    }

    if(ResultsPassword.ToLower()!=Password.ToLower())
    {
        return false;
    }

    return true;
}


/// Get Player Information
PlayerObject *  GameEconomicServer::GetSingleDBPlayer(String PlayerUniqueID)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// Return player information
    PlayerObject * ReturnPlayer = NULL;

    /// create a bew player
    ReturnPlayer = new PlayerObject();

    /// Copy fields
    TableNames.Push("playeruniqueid");

    /// Copy parameters
    TableFields.Push(PlayerUniqueID);

    /// Select Player
    Vector<String> Results = selectDBPlayer(TableNames,TableFields);

    /// If no records found
    if(Results.At(1)==String("0"))
    {
        /// return null if player is not found

        delete ReturnPlayer;

        return NULL;
    }

    /// Copy Information
    ReturnPlayer->Firstname=Results.At(3);
    ReturnPlayer->Middlename=Results.At(4);
    ReturnPlayer->Lastname=Results.At(5);
    ReturnPlayer->Level=atoi(Results.At(6).CString());
    ReturnPlayer->Experience=atoi(Results.At(7).CString());
    ReturnPlayer->Reputation=atoi(Results.At(8).CString());
    ReturnPlayer->Reputation1=atoi(Results.At(9).CString());
    ReturnPlayer->Reputation2=atoi(Results.At(10).CString());
    ReturnPlayer->Reputation3=atoi(Results.At(11).CString());
    ReturnPlayer->Reputation4=atoi(Results.At(12).CString());
    ReturnPlayer->Reputation5=atoi(Results.At(13).CString());
    ReturnPlayer->AlienRace=atoi(Results.At(14).CString());
    ReturnPlayer->AlienAllianceAligned=atoi(Results.At(15).CString());
    ReturnPlayer->Gender=atoi(Results.At(16).CString());
    ReturnPlayer->PersonalityTrait=atoi(Results.At(17).CString());
    ReturnPlayer->UniqueID=Results.At(18).CString();
    ReturnPlayer->GalaxySeed=Results.At(19).CString();
    ReturnPlayer->Credits=atoi(Results.At(20).CString());

    /// Clear Traits
    ReturnPlayer->TotalGroundPassiveTraits=0;
    ReturnPlayer->GroundPassiveTrait.Clear();
    ReturnPlayer->TotalSpacePassiveTraits=0;
    ReturnPlayer->SpacePassiveTrait.Clear();

    return ReturnPlayer;
}

/// Get Player Information
Vector<PlayerList> * GameEconomicServer::GetPlayersDBAccount(String AccountUniqueID)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// Return player information
    Vector <PlayerList> * ReturnPlayers = NULL;

    /// create a bew player
    ReturnPlayers = new Vector<PlayerList>();

    /// Copy fields
    TableNames.Push("playerowneruniqueid");

    /// Copy parameters
    TableFields.Push(AccountUniqueID);

    /// Select Player
    Vector<String> Results = selectDBPlayer(TableNames,TableFields);

    cout << "it got here" << endl;

    /// If no records found
    if(Results.At(1)==String("0"))
    {
        /// return null if player is not found

        delete ReturnPlayers;

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

        PlayerList TempPlayer;

        /// copy information
        TempPlayer.Firstname = Results.At(index);
        TempPlayer.Middlename = Results.At(index+1);
        TempPlayer.Lastname= Results.At(index+2);
        TempPlayer.Level = atoi(Results.At(index+3).CString());
        TempPlayer.AlienRace = atoi(Results.At(index+11).CString());
        TempPlayer.AlienRaceAllianceAligned = atoi(Results.At(index+12).CString());
        TempPlayer.Gender = atoi(Results.At(index+13).CString());
        TempPlayer.PersonalityTrait = atoi(Results.At(index+14).CString());
        TempPlayer.UniqueID= Results.At(index+15);

        /// copy data to temp player
        ReturnPlayers->Push(TempPlayer);
    }

    return ReturnPlayers;
}

/// verify
String GameEconomicServer::ConnectionGetDBAccount(String Username, String Password)
{
    /// Get tables
    Vector<String> TableNames;
    Vector<String> TableFields;
    String ReturnString;
    Vector<String> Results;

    /// Create name to serch
    TableNames.Push("accountusername");

    TableFields.Push(Username);

    /// if a valid email was given
    if(EmailValidCheck(Username)==true)
    {
        TableNames.At(0) = String("accountemail");
    }


    /// Add password
    TableNames.Push("accountpassword");

    TableFields.Push(Password);

    Results = selectDBAccount(TableNames,TableFields);

    if(Results.Size()==0)
    {
        return String("|0|0");
    }

    /// Append String
    for(unsigned int i=0; i<Results.Size(); i++)
    {
        /// GetResults
        ReturnString.Append("|");
        ReturnString.Append(Results.At(i));
    }

    return ReturnString;
}

/// Get Player Information
String GameEconomicServer::ConnectionGetPlayersDBAccount(String AccountUniqueID)
{
    /// Create empty tables and fields and get results
    String ReturnString;

    /// Return player information
    Vector <PlayerList>  * ReturnPlayers;

    ReturnPlayers=GetPlayersDBAccount(AccountUniqueID);

    /// Return no character make sure
    if(ReturnPlayers==NULL)
    {
        return String("|0|0");
    }

    /// Copy Cols and Row Counts
    ReturnString.Append("|");

    if(ReturnPlayers->Size()>0)
    {
        ReturnString.Append("9");
    }
    else
    {
        ReturnString.Append("0");
    }

    ReturnString.Append("|");
    ReturnString.Append(String(ReturnPlayers->Size()));

    /// If blank return nothing
    if(ReturnPlayers->Size()==0)
    {

        return ReturnString;
    }

    /// Transform to string
    for(unsigned int i=0; i<ReturnPlayers->Size(); i++)
    {
        /// GetResults and copy it
        ReturnString.Append("|");
        ReturnString.Append(ReturnPlayers->At(i).Firstname);

        ReturnString.Append("|");
        ReturnString.Append(ReturnPlayers->At(i).Middlename);

        ReturnString.Append("|");
        ReturnString.Append(ReturnPlayers->At(i).Lastname);

        ReturnString.Append("|");
        ReturnString.Append(ReturnPlayers->At(i).UniqueID);

        ReturnString.Append("|");
        ReturnString.Append(String(ReturnPlayers->At(i).Gender));

        ReturnString.Append("|");
        ReturnString.Append(String(ReturnPlayers->At(i).AlienRace));

        ReturnString.Append("|");
        ReturnString.Append(String(ReturnPlayers->At(i).AlienRaceAllianceAligned));

        ReturnString.Append("|");
        ReturnString.Append(String(ReturnPlayers->At(i).PersonalityTrait));

        ReturnString.Append("|");
        ReturnString.Append(String(ReturnPlayers->At(i).Level));


    }

    return ReturnString;
}

/// get single player
String GameEconomicServer::ConnectionGetSingleDBPlayer(String PlayerUniqueID)
{
    /// create a player object
    PlayerObject * TemporarySinglePlayer;

    /// Create a results string
    String ReturnString;

    if(PlayerUniqueID.Empty())
    {
        return String("Error");
    }

    /// allocate a new object
    TemporarySinglePlayer = new PlayerObject();

    TemporarySinglePlayer = GetSingleDBPlayer(PlayerUniqueID);

    if(TemporarySinglePlayer==NULL)
    {
        /// delete temporary
        delete TemporarySinglePlayer;

        return String("|0|0");

    }

    ReturnString.Append(String("|17|1"));

    ReturnString.Append("|");
    ReturnString.Append(TemporarySinglePlayer->Firstname);

    ReturnString.Append("|");
    ReturnString.Append(TemporarySinglePlayer->Middlename);

    ReturnString.Append("|");
    ReturnString.Append(TemporarySinglePlayer->Lastname);

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Level));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Experience));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Reputation));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Reputation1));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Reputation2));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Reputation3));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Reputation4));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Reputation5));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->AlienRace));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->AlienAllianceAligned));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Gender));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->PersonalityTrait));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->GalaxySeed));

    ReturnString.Append("|");
    ReturnString.Append(String(TemporarySinglePlayer->Credits));

    ReturnString.Append("|");
    ReturnString.Append(TemporarySinglePlayer->UniqueID);


    delete TemporarySinglePlayer;

    return ReturnString;
}



















