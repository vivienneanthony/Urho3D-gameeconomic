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
#include <Urho3D/Container/Str.h>
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
#include <Urho3D/Network/Connection.h>

#include <iostream>
#include <signal.h>
#include <sstream>
#include <fstream>
#include <vector>

#include "GameEconomicServer.h"

#include "../GameEconomicComponents/ServerConsoleInterface.h"
#include "../GameEconomicComponents/connectorDB.h"

#include "signalHandler.hpp"
#include <pthread.h>

#include <Urho3D/DebugNew.h>
#include "../GameEconomicComponents/Accounts.h"
#include "../Networking.h"


using namespace std;
using namespace Urho3D;


/// Parse command
Vector<String> GameEconomicServer::ParseCommand(String EnteredString)
{
    /// Vector String
    Vector<String> SplitEntered;

    /// Split
    SplitEntered = EnteredString.Split(' ');

    return SplitEntered;
}

void GameEconomicServer::ExecuteCommandAdminClient(String FirstCommand, Vector<String> Arguments, Urho3D::Connection * sender)
{
    /// removed old code;
    String Command=FirstCommand.ToLower().Trimmed();

    /// test command
    if(Command==String("quit"))
    {
        cout << "Shutting down." << endl;

        Stop();
    }
    else if(Command==String("name"))
    {

        String NameResponse("My name is Caspian identification 1701-A prototype core Existence.");
        // Send the chat message as in-order and reliable

        SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,NameResponse,sender);

        ///cout << "My name is Caspian identification 1701-A prototype core Existence." << endl;

    }
    else if(Command==String("network"))
    {
        HandleNetworkCommands(Arguments, sender);
    }
    else if(Command==String("accounts"))
    {
        HandleAccountCommands(Arguments, sender);
    }
    else if(Command==String("administrators"))
    {
        HandleAdministratorCommands(Arguments, sender);
    }
    else if(Command==String("players"))
    {
        HandlePlayerCommands(Arguments, sender);
    }
    return;
}

/// Query from the client
void GameEconomicServer::ExecuteCommandGameClient(String FirstCommand, Vector<String> Arguments, Urho3D::Connection * sender)
{
    /// removed old code;
    String Command=FirstCommand.ToLower().Trimmed();

    /// Get eventmap and time
    Urho3D::VariantMap NetworkClientIdentity;

    /// Get the identity
    NetworkClientIdentity=sender->GetIdentity();

    /// Try to get the information
    unsigned int clienttype=NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTYPE].GetInt();

    /// Return unauthorized
    if(clienttype!=Authenticated)
    {
        return;
    }

    /// Setup first command
    if(Command==String("requestplayers"))
    {
        /// Get string and then form a result
        String connectionDBPlayers=ConnectionGetPlayersDBAccount(Arguments.At(0));

        /// Send a message saying authorized
        SendNetworkMessage(NetMessageRequestApprovedGetAccountPlayers,true,true,connectionDBPlayers,sender);
    }

    return;
}

/// Handle All Acounts
void GameEconomicServer::HandleNetworkCommands(Vector <String> &Arguments, Urho3D::Connection * sender)
{
    /// Get console connection
    if(Arguments.At(0) == String("getserverconnection"))
    {
        /// Get network info
        Network* network = GetSubsystem<Network>();
        Urho3D::Connection * serverConnection =  network->GetServerConnection();

        /// Output
        cout << "Server connection : " << serverConnection->ToString().CString();
    }

    return;
}

/// Handle All Acounts
void GameEconomicServer::HandleAccountCommands(Vector <String> &Arguments, Urho3D::Connection * sender)
{

    if(Arguments.At(0) == String("addaccount"))
    {

        /// if accounts less then needed write message
        if(Arguments.Size()<7)
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Error: Arguments incomplete",sender);

            return;
        }

        if(EmailValidCheck(Arguments.At(5))==false)
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Email valid",sender);

            return;
        }

        /// create a tempaccount
        AccountInformation TempAccount;

        /// copy database
        TempAccount.Username = Arguments.At(1);
        TempAccount.Firstname = Arguments.At(2);
        TempAccount.Middlename = Arguments.At(3);
        TempAccount.Lastname = Arguments.At(4);
        TempAccount.Email = Arguments.At(5);
        TempAccount.Password = Arguments.At(6);

        /// Return message if success
        if(insertDBAccount(TempAccount))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Account created.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Account creation error.",sender);
        }

    }

    /// List accounts command
    if(Arguments.At(0) == String("listaccounts"))
    {
        String Results=ListAllDBAccounts(sender);

        /// Send results
        SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,Results,sender);
    }

    /// Update administrator
    if(Arguments.At(0)== String("updateaccount"))
    {

        /// if accounts less then needed write message
        if(Arguments.Size()<7)
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Error: Arguments incomplete",sender);

            return;
        }

        /// create a tempaccount
        AccountInformation TempAccount;

        TempAccount.Username = Arguments.At(1);
        TempAccount.Firstname = Arguments.At(2);
        TempAccount.Middlename = Arguments.At(3);
        TempAccount.Lastname = Arguments.At(4);
        TempAccount.Email = Arguments.At(5);
        TempAccount.Password = Arguments.At(6);
        TempAccount.UniqueID = Arguments.At(7);

        /// Return message if success
        if(UpdateSingleDBAccount(TempAccount))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Account updated.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Account update failed.",sender);
        }

    }

    if(Arguments.At(0)== String("getplayers"))
    {
        /// create a tempPlayer

        GetPlayersDBAccount(Arguments.At(1));
    }



    return;

}

/// Handle All Acounts
void GameEconomicServer::HandleAdministratorCommands(Vector <String> &Arguments, Urho3D::Connection * sender)
{
    /// Add administrator command
    if(Arguments.At(0) == String("addadministrator"))
    {

        /// if accounts less then needed write message
        if(Arguments.Size()<4)
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Error: Arguments incomplete",sender);

            return;
        }

        /// create a tempaccount
        AdministratorInformation TempAdministrator;

        /// copy database
        TempAdministrator.Username = Arguments.At(1);
        TempAdministrator.Password = Arguments.At(2);
        TempAdministrator.UniqueID = Arguments.At(3);
        TempAdministrator.AdminAccounts=1;

        /// Return message if success
        if(insertDBAdministrator(TempAdministrator))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Administrator created.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Administrator creation error.",sender);
        }
    }

    /// List administrators command
    if(Arguments.At(0) == String("listadministrators"))
    {
        String Results=ListAllDBAdministrators();

        /// Send results
        SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,Results,sender);

    }

    /// Update administrator command
    if(Arguments.At(0)== String("updateadministrator"))
    {
        /// if accounts less then needed write message
        if(Arguments.Size()<4)
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Error: Arguments incomplete",sender);

            return;
        }

        /// create a tempaccount
        AdministratorInformation TempAdministrator;

        TempAdministrator.Username = Arguments.At(1);
        TempAdministrator.Password = Arguments.At(2);
        TempAdministrator.UniqueID = Arguments.At(3);
        TempAdministrator.AdminAccounts=1;

        /// Return message if success
        if(UpdateSingleDBAdministrator(TempAdministrator))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Administrator updated.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Administrator update failed.",sender);
        }

    }

    /// Delete administrator command
    if(Arguments.At(0)== String("deleteadministrator"))
    {

        /// create a tempaccount
        AdministratorInformation TempAdministrator;

        TempAdministrator.UniqueID = Arguments.At(1);

        /// Return message if success
        if(DeleteSingleDBAdministrator(TempAdministrator))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Administrator deleted.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Administrator delete failed.",sender);
        }
    }

    return;
}


/// Handle All Acounts
void GameEconomicServer::HandlePlayerCommands(Vector <String> &Arguments, Urho3D::Connection * sender)
{
    /// List players command
    if(Arguments.At(0) == String("listplayers"))
    {
        String Results=ListAllDBPlayers(sender);

        /// Send results
        SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,Results,sender);
    }

    /// Add player command
    if(Arguments.At(0) == String("addplayer"))
    {

        /// if Players less then needed write message
        if(Arguments.Size()<5)
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Error: Arguments incomplete",sender);
            return;
        }

        /// create a tempPlayer
        PlayerObject TempPlayer;

        /// copy database
        TempPlayer.Firstname = Arguments.At(1);
        TempPlayer.Middlename = Arguments.At(2);
        TempPlayer.Lastname = Arguments.At(3);
        TempPlayer.UniqueID = Arguments.At(4);

        /// add defaults
        /// Copy specific Players
        TempPlayer.Health=0;
        TempPlayer.Level = 0;
        TempPlayer.Experience = 0;
        TempPlayer.Reputation = 0;
        TempPlayer.Reputation1 = 0;
        TempPlayer.Reputation2 = 0;
        TempPlayer.Reputation3 = 0;
        TempPlayer.Reputation4 = 0;
        TempPlayer.Reputation5 = 0;
        TempPlayer.AlienRace = 0;
        TempPlayer.AlienAllianceAligned = 0;
        TempPlayer.Gender = 0;
        TempPlayer.PersonalityTrait = 0;

        if(insertDBPlayer(TempPlayer))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Player created.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Player creation error.",sender);
        }

    }

    /// Update player command
    if(Arguments.At(0)== String("updateplayer"))
    {
        /// if Players less then needed write message
        if(Arguments.Size()<5)
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Error: Arguments incomplete",sender);

            return;
        }

        /// create a tempPlayer
        PlayerObject TempPlayer;

        TempPlayer.Firstname = Arguments.At(1);
        TempPlayer.Middlename = Arguments.At(2);
        TempPlayer.Lastname = Arguments.At(3);
        TempPlayer.UniqueID = Arguments.At(4);



        /// Return message if success
        if(UpdateSingleDBPlayer(TempPlayer))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Player updated.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Player update failed.",sender);
        }

    }
    /// Delete player command
    if(Arguments.At(0)== String("deleteplayer"))
    {

        /// create a tempPlayer
        PlayerObject TempPlayer;

        TempPlayer.UniqueID = Arguments.At(1);

        /// Return message if success
        if(DeleteSingleDBPlayer(TempPlayer))
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Player deleted.",sender);
        }
        else
        {
            SendNetworkMessage(NetMessageAdminClientSendAcknowledge,true,true,"Player delete failed.",sender);
        }

    }

    /// Delete player command
    if(Arguments.At(0)== String("getplayer"))
    {

        /// create a tempPlayer
        PlayerObject * TempPlayer = NULL;

        TempPlayer = new PlayerObject();

        TempPlayer = GetSingleDBPlayer(Arguments.At(1));
    }



    return;

}
