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
#include <Urho3D/Network/NetworkEvents.h>
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

#include "GameEconomicServerClient.h"

#include <pthread.h>

#include <Urho3D/DebugNew.h>

// Expands to this example's entry-point
DEFINE_APPLICATION_MAIN(GameEconomicServerClient)

using namespace std;
using namespace Urho3D;

GameEconomicServerClient::GameEconomicServerClient(Context* context) :
    GameEconomicApp(context)
{

    return;
}



void GameEconomicServerClient::Start()
{
    /// get system
    FileSystem * fileSystem = GetSubsystem<FileSystem>();

    Log * logs=GetSubsystem<Log>();

    bool success=false;

    /// Execute base class startup
    GameEconomicApp::Start();


    /// Open the operating system console window (for stdin / stdout) if not open yet
    OpenConsoleWindow();

    /// odbc stuff here
    networkconfiguration MyNetwork;

    /// Display text
    cout << "Info: Loading network configuration ." << endl;

    /// Load database configuration file
    success = LoadNetworkConfig(MyNetwork);

    /// Ff configuration loaded configuration database
    if(success)
    {
        cout << "Info: Loading network configuration successful." << endl;
    }
    else
    {
        cout << "Info: Loading network configuration failed. Using 127.0.0.1 and port 3306 as default." << endl;
    }


    /// show splash
    Splash();



    /// Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    /// like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    /// could subscribe in the constructor instead.
    SubscribeToEvents();

    ConsoleInterface(MyNetwork);

    /// change log level
    logs->SetLevel(LOG_ERROR );

    return;
}

/// Display simple splash screen
void GameEconomicServerClient::Splash(void)
{
    /// Output program information
    cout << "Headless Server Model " << endl;
    cout << "Programmer Vivienne Anthony Client\r\n " << endl;

    return;
}

/// Subscribe to all events
void GameEconomicServerClient::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(GameEconomicServerClient, HandleUpdate));

    /// Network related
    SubscribeToEvent(E_NETWORKMESSAGE, HANDLER(GameEconomicServerClient, HandleNetworkMessage));
    SubscribeToEvent(E_SERVERCONNECTED, HANDLER(GameEconomicServerClient, OnConnection));
    SubscribeToEvent(E_SERVERDISCONNECTED, HANDLER(GameEconomicServerClient, OnDisconnection));
    SubscribeToEvent(E_CONNECTFAILED, HANDLER(GameEconomicServerClient, OnConnectionFailed));

    return;
}

/// Handle Updates
void GameEconomicServerClient::HandleUpdate(StringHash eventType, Urho3D::VariantMap& eventData)
{
    // Do nothing for now, could be extended to eg. animate the display
    return;
}


void GameEconomicServerClient::Stop(void)
{
    ///Dump resources
    engine_ -> DumpResources();

    /// Exit
    engine_->Exit();

    /// Terminate
    exit(0);

    return;
}




/// Actual thread function
void GameEconomicServerClient::ConsoleInterface(networkconfiguration &networksetup)
{
    Network* network = GetSubsystem<Network>();
    bool success=false;

    /// turn on inteface
    bool isActiveFlag=true;

    /// Urho related variables
    String PromptInput;
    Vector<String> SplitPromptInput;


    Connection* serverConnection;

    /// Configure network is none is specified

    if(networksetup.hostserver.Empty())
    {
        networksetup.hostserver = String("127.0.0.1");
    }

    if(networksetup.hostport==0)
    {
        networksetup.hostport = 3306;
    }

    if(networksetup.hostidentity.Empty())
    {
        networksetup.hostidentity = String("localhost");
    }

    /// loop
    do
    {
        serverConnection =  network->GetServerConnection();;

        // Clear prompt input
        PromptInput.Clear();
        SplitPromptInput.Clear();

        /// Display prompt to the user
        if(serverConnection&&serverconnection)
        {
            cout << endl << "(serveconnect) Enter Command >> ";
        }
        else
        {
            cout << endl << "(offline) Enter Command >> ";
        }

        /// Get input from the user
        GetLine(PromptInput);

        /// Promput empty
        if(PromptInput.Empty())
        {
            cout << "Error. Invalid  command." << endl;
            continue;
        }

        /// Trim input
        String CleanedPromptInput = PromptInput.Trimmed();

        if(CleanedPromptInput==String("connect"))
        {
            Connect(networksetup);
        }
        else if(CleanedPromptInput==String("quit"))
        {
            Stop();
        }
        else
        {
            /// if server connection and serverConnection
            if(serverconnection&&serverconnection)
            {
                    cout << "send message" << endl;

                /// Clean
                SendMessage(CleanedPromptInput);
            }
        }
    }

    while(isActiveFlag);

    return;
}


void GameEconomicServerClient::GetLine(String & returnstring)
{
    /// Define a String
    string getpromptcommand;

    /// write line
    bool success = getline (std::cin,getpromptcommand);

    /// Clear String
    returnstring.Clear();

    /// Append input to a string
    returnstring.Append(getpromptcommand.c_str());

    return;
}

