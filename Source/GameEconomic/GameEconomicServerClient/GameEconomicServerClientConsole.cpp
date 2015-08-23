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
#include <Urho3D/Core/Context.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Core/Thread.h>

#include <iostream>
#include <signal.h>
#include <sstream>
#include <fstream>
#include <vector>

#include "GameEconomicServerClientConsole.h"
#include "../Networking.h"

#include <Urho3D/DebugNew.h>

using namespace std;
using namespace Urho3D;

GameEconomicServerClientConsole::GameEconomicServerClientConsole(Context *context)
    :LogicComponent(context)
{

    return;
}

/// Register Object
void GameEconomicServerClientConsole::RegisterObject(Context * context)
{
    context -> RegisterFactory<GameEconomicServerClientConsole>();


    return;
}


/// Entity Register Object


/// Register Object
GameEconomicServerClientConsole::~GameEconomicServerClientConsole()
{

    return;
}


/// Server Console Start
void GameEconomicServerClientConsole::Start()
{
    Thread::Run();
}


/// Actual thread function
void GameEconomicServerClientConsole::ThreadFunction()
{
    /// Get network subsystem
    Network* network = GetSubsystem<Network>();

    bool success=false;
    bool lineEntered=false;

    /// turn on inteface
    bool isActiveFlag=true;

    /// Urho related variables
    String PromptInput;
    String ConsoleInputLine;

    Connection* serverConnection;

    /// Show prompt
    ShowPrompt();

    /// Clear prompt input
    PromptInput.Clear();
    ConsoleInputLine.Clear();

    /// loop
    do
    {
        ConsoleInputLine.Clear();

        /// Get input from the user
        ConsoleInputLine = GetConsoleInput();

        /// append if its not empty
        if(ConsoleInputLine.Length()>0)
        {
            /// prompt
            PromptInput.Append(ConsoleInputLine);
            lineEntered=true;
        }

        /// test if there was input
        if(lineEntered==true&&PromptInput.Empty())
        {
            cout << "Invalid command.." << endl;

            /// Clear prompt input
            PromptInput.Clear();

            ///Display new prompt
            ShowPrompt();

            lineEntered=false;
        }
        else if (lineEntered==true)
        {
            /// Trim input to clean up
            String CleanedPromptInput = PromptInput.Trimmed();

            /// Test Commands
            if(CleanedPromptInput==String("connect"))
            {
                Connect(NetworkConfiguration);
            }
            else if(CleanedPromptInput==String("quit"))
            {
                /// Terminate program forced
                exit(0);
            }
            else
            {
                /// if server connection and serverConnection
                if(serverconnection&&serverconnection)
                {
                    /// Clean
                    SendMessage(CleanedPromptInput);
                }
            }

            /// Clear prompt input
            PromptInput.Clear();

            ///Display new prompt
            ShowPrompt();

            lineEntered=false;
        }

    }

    while(isActiveFlag);

    return;
}

void GameEconomicServerClientConsole::ShowPrompt(void)
{

    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    /// Get serverconnection
    serverConnection =  network->GetServerConnection();

    /// Display prompt to the user
    if(serverConnection&&serverconnection)
    {
        cout << endl << "(serveconnect) Enter Command >> ";
    }
    else
    {
        cout << endl << "(offline) Enter Command >> ";
    }
}

void GameEconomicServerClientConsole::ConfigureNetwork(networkconfiguration &TempNetworkConfig)
{
    /// Copy configuration
    NetworkConfiguration = TempNetworkConfig;

    /// Configure network is none is specified
    if(TempNetworkConfig.hostserver.Empty())
    {
        NetworkConfiguration.hostserver = String("127.0.0.1");
    }

    if(TempNetworkConfig.hostport==0)
    {
         NetworkConfiguration.hostport = 3632;
    }

    if(TempNetworkConfig.hostidentity.Empty())
    {
         NetworkConfiguration.hostidentity = String("localhost");
    }

    return;
}


/// handle connection
void GameEconomicServerClientConsole::Connect(networkconfiguration &tempnetwork)
{
    /// Get Urho subsystem
    Network* network = GetSubsystem<Network>();

    /// Define Variables
    bool success=true;

    cout << "Connecting Server Console Interface to Host ("<< tempnetwork.hostserver.CString() << " on port " << tempnetwork.hostport << ")." << endl;

    /// Get currenttime
    Urho3D::Time systemtime(context_);
    unsigned int currenttime = systemtime.GetSystemTime();

    /// Get eventmap and time
    Urho3D::VariantMap NetworkClientIdentity;

    NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTYPE] =Unauthenticated;
    NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTARRIVAL]= currenttime;

    /// Set Identity
    ///newConnection->SetIdentity(NetworkClientIdentity);
    serverconnection = network->Connect(tempnetwork.hostserver, tempnetwork.hostport, 0, NetworkClientIdentity);


    return;
}

void GameEconomicServerClientConsole::SendMessage(String Message)
{
    /// If sending message is empty
    if (Message.Empty())
    {
        return; // Do not send an empty message
    }

    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    /// Send message only if serverconnection and connection is true
    if (serverConnection&&serverconnection)
    {
               // A VectorBuffer object is convenient for constructing a message to send
        VectorBuffer msg;
        msg.WriteString(Message);
        // Send the chat message as in-order and reliable
        serverConnection->SendMessage(NetMessageAdminClientSend, true, true, msg);
    }

    return;
}
