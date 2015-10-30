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
#include <Urho3D/Network/Connection.h>
#include <Urho3D/Container/Vector.h>

#include <iostream>
#include <signal.h>
#include <sstream>
#include <fstream>
#include <ctime>

#include "GameEconomicServer.h"

#include "../GameEconomicComponents/ServerConsoleInterface.h"
#include "../GameEconomicComponents/connectorDB.h"
#include "../Networking.h"

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;

class GameEconomicServer;


/// handle server side nmessage
void GameEconomicServer::HandleNetworkMessage(StringHash eventType, Urho3D::VariantMap& eventData)
{
    /// Network subsystem
    Network* network = GetSubsystem<Network>();

    using namespace NetworkMessage;

    int msgID = eventData[P_MESSAGEID].GetInt();

    /// Urho related variables
    String PromptInput;
    Vector<String> SplitPromptInput;

    /// If the administartion client sends a command
    if (msgID == NetMessageAdminClientSend)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);

        String text = msg.ReadString();

        Urho3D::Connection* sender = static_cast<Urho3D::Connection*>(eventData[P_CONNECTION].GetPtr());

        /// parse command
        SplitPromptInput = ParseCommand(text.Trimmed());

        /// First command
        String FirstCommand = SplitPromptInput[0];

        /// Check sisze for arguments
        if(SplitPromptInput.Size()>0)
        {
            /// Remove first element since its not needed
            SplitPromptInput.Erase(0);
        }

        /// ExecuteCommand
        ExecuteCommandAdminClient(FirstCommand, SplitPromptInput, sender);
    }

    /// if it was a heartbeat request
    if (msgID == NetMessageRequest)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);

        String text = msg.ReadString();

        Urho3D::Connection* sender = static_cast<Urho3D::Connection*>(eventData[P_CONNECTION].GetPtr());

        /// parse command
        SplitPromptInput = ParseCommand(text.Trimmed());

        /// First command
        String FirstCommand = SplitPromptInput[0];

        /// Check sisze for arguments
        if(SplitPromptInput.Size()>0)
        {
            /// Remove first element since its not needed
            SplitPromptInput.Erase(0);
        }

        /// ExecuteCommand
        ExecuteCommandGameClient(FirstCommand, SplitPromptInput, sender);
    }

    /// If message was authenicated
    if (msgID == NetMessageAuthenticateSend)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);

        /// Read the string from the memory buffer
        String text = msg.ReadString();

        /// Parse received message
        SplitPromptInput = ParseCommand(text.Trimmed());

        cout << "input" << SplitPromptInput.At(0).CString() <<endl;
        cout << "input" <<SplitPromptInput.At(1).CString() <<endl;

        /// Verify password - case sensitive
        bool authorized = VerifyIdentityDB(DBAccount, SplitPromptInput.At(0),SplitPromptInput.At(1));

        /// Get Sender
        Urho3D::Connection* sender = static_cast<Urho3D::Connection*>(eventData[P_CONNECTION].GetPtr());

        /// Send a response for authorized user or not
        if(authorized)
        {
            String connectionAccount=ConnectionGetDBAccount(SplitPromptInput.At(0),SplitPromptInput.At(1));

            /// Get current system time
            /// Get current system time
            Urho3D::Time systemtime(context_);
            unsigned int currenttime = systemtime.GetSystemTime();

            cout << connectionAccount.CString();

            /// Send a message saying authorized
            SendNetworkMessage(NetMessageAuthenticatedApproved,true,true,connectionAccount,sender);

            /// Set Client to Authenticated Player
            Urho3D::VariantMap NetworkClientIdentity;

            NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTYPE] = Authenticated;
            NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTARRIVAL]= currenttime;

            /// Set the sender identity
            sender->SetIdentity(NetworkClientIdentity);
        }else
        {
            /// Send a message saying denied
            SendNetworkMessage(NetMessageAuthenticatedDenied,true,true,"|0|0",sender);
        }
    }
}

/// Check identity
void GameEconomicServer::NewConnectionIdentity(StringHash eventType, Urho3D::VariantMap& eventData)
{
    /// Get connection information
    Urho3D::Connection * newConnection = (Urho3D::Connection *) eventData[ClientConnected::P_CONNECTION].GetPtr();

    /// Get the client type
    unsigned int clienttype=eventData[NetworkClientIdentity::NETWORK_CLIENTYPE].GetInt();

    /// Send out authenticate to acknwoledge a connection
    if(clienttype==Unauthenticated)
    {
        /// send a message
        SendNetworkMessage(NetMessageAuthenticateAcknowledge,true,true,"",newConnection);
    }

    return;
}

/// Handle new connections
void GameEconomicServer::NewConnection(StringHash eventType, Urho3D::VariantMap& eventData)
{\
    /// Get Connection
    Urho3D::Connection * newConnection = (Urho3D::Connection *) eventData[ClientConnected::P_CONNECTION].GetPtr();

    /// Output to screen
    cout << "Network: New connection established Client(" << newConnection->ToString().CString() <<")" <<  endl;

    return;
}

/// Netoworking update
void GameEconomicServer::NetworkingOnUpdate(float timeStep)
{
    /// Create a time check
    static float testnetworkupdate=0;

    /// Add time steps
    testnetworkupdate+=timeStep;

    /// Poll notice
    if(testnetworkupdate>10)
    {
        /// Get current network
        Network* network = GetSubsystem<Network>();
        Urho3D::Connection* serverConnection = network->GetServerConnection();

        /// Get current system time
        Urho3D::Time SystemTime(context_);
        unsigned int currentTime = SystemTime.GetSystemTime();

        /// Get eventmap and time
        Urho3D::VariantMap NetworkClientIdentity;

        /// get all connections
        allConnections = network->GetClientConnections();

        /// Loop through each iterator
        for (unsigned int idx=0; idx<allConnections.Size(); idx++)
        {
            /// Get the identity
            NetworkClientIdentity=allConnections.At(idx)->GetIdentity();

            /// try to get the information
            unsigned int clienttype=NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTYPE].GetInt();
            unsigned long int clientarrival=NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTARRIVAL].GetInt();

            cout << "Network: Poll (" << allConnections.At(idx)->ToString().CString() << " Type " << clienttype << " Arrival " << clientarrival << ")" << endl;
        }
        testnetworkupdate=0;
    }
    return;
}

/// Send a network message
void GameEconomicServer::SendNetworkMessage(NetworkMessageTypes NetworkMessageType, bool flag1, bool flag2, String MessageText, Urho3D::Connection * SenderTo)
{

    /// A VectorBuffer object is convenient for constructing a message to send
    VectorBuffer msg;
    msg.WriteString(MessageText);

 	/// Get current system time
    Urho3D::Time SystemTime(context_);
    unsigned int currentTime = SystemTime.GetSystemTime();

    SenderTo->SendMessage(NetworkMessageType,flag1,flag2, msg, currentTime);

    cout << "\r\nNetwork: SentNetworkMessage (\"" << MessageText.CString() <<"\") to (" << SenderTo->ToString().CString() <<")" << endl;

    return;
}
