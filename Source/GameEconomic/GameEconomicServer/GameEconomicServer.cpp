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

#include "signalHandler.hpp"
#include <pthread.h>

#include <Urho3D/DebugNew.h>

// Expands to this example's entry-point
DEFINE_APPLICATION_MAIN(GameEconomicServer)

using namespace std;
using namespace Urho3D;

GameEconomicServer::GameEconomicServer(Context* context) :
    GameEconomicApp(context)
{

    connectorDB::RegisterNewSubsystem(context);
    return;
}

void GameEconomicServer::Start()
{
    /// get system
    FileSystem * fileSystem = GetSubsystem<FileSystem>();
    connectorDB * connectionDB= GetSubsystem<connectorDB>();
    Log * logs=GetSubsystem<Log>();


    /// Execute base class startup
    GameEconomicApp::Start();

    /// Create a signalHandler
    SignalHandler signalHandler;

    /// Register signal handler to handle kill signal
    signalHandler.setupSignalHandlers();

    /// Open the operating system console window (for stdin / stdout) if not open yet
    OpenConsoleWindow();

    /// Create a interface point
    ServerConsoleInterface * PrimaryConsoleInterface;

    /// odbc stuff here
    databaseconnection MyDB;
    networkconfiguration MyNetwork;

    /// Display text
    cout << "Info: Loading database configuration ." << endl;

    /// Load database configuration file
    bool  success = LoadDatabaseConfig(MyDB);

    /// Ff configuration loaded configuration database
    if(success)
    {
        cout << "Info: Loading database configuration successful." << endl;

        /// configure database
        connectionDB->setHost(MyDB.hostname,MyDB.username,MyDB.password);
        connectionDB->setDB(MyDB.schema);

        /// Attempt to connect to database to validate
        if(connectionDB->connectHost())
        {
            cout << "Info: Database connection succesful. "  << endl;

            /// Actual database initialization
            DatabaseInitialization();
        }
        else
        {
            cout << "Info: Database connection failed. Check configuration." << endl;
        }
    }
    else
    {
        cout << "Info: Loading database configuration failed. Check configuration." << endl;
    }


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

    /// Create Interface Context
    PrimaryConsoleInterface = new ServerConsoleInterface(context_);

    /// Copy network
    PrimaryConsoleInterface->SetHostNetworkServer(MyNetwork);

    /// Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    /// like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    /// could subscribe in the constructor instead.
    SubscribeToEvents();

    /// change log level
    ///logs->SetLevel(LOG_ERROR );

    /// Networking
    NetworkInitialization(3306);

    CoreInitialization();

    /// Start the interface
    PrimaryConsoleInterface -> Start();

    return;
}

/// Display simple splash screen
void GameEconomicServer::Splash(void)
{
    /// Output program information
    cout << "Headless Server Model " << endl;
    cout << "Programmer Vivienne Anthony\r\n " << endl;

    return;
}

/// Subscribe to all events
void GameEconomicServer::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(GameEconomicServer, HandleUpdate));

    /// execute console event
    SubscribeToEvent(SERVER_CONSOLEEVENT, HANDLER(GameEconomicServer,OnServerConsoleInterfaceEvent));

    return;
}

/// Handle Updates
void GameEconomicServer::HandleUpdate(StringHash eventType, Urho3D::VariantMap& eventData)
{
    // Do nothing for now, could be extended to eg. animate the display
    return;
}

/// OnServerConsoleInterfaceEvent Listener Handler
void GameEconomicServer::OnServerConsoleInterfaceEvent(StringHash eventType, Urho3D::VariantMap& eventData)
{
    /// Get Urho API level resources
    UI* ui_ = GetSubsystem<UI>();
    Engine * engine_ = GetSubsystem<Engine>();

    /// Get Datas
    String Command=eventData[server_consolecommand::P_CMD].GetString();
    String Arguments=eventData[server_consolecommand::P_ARG].GetString();

    Command=Command.ToLower().Trimmed();
    Arguments=Arguments.Trimmed();

    Vector<String> SplitPromptInput;
    SplitPromptInput = ParseCommand(Arguments.Trimmed());

    /// test command
    if(Command==String("quit"))
    {
        Stop();
        return;
    }else
    {

        ExecuteCommand(Command,SplitPromptInput);
    }


    return;
}

void GameEconomicServer::Stop(void)
{
    ///Dump resources
    engine_ -> DumpResources();

    /// Exit
    engine_->Exit();

    /// Terminate
    exit(0);

    return;
}


void GameEconomicServer::NetworkInitialization(unsigned int Port)
{
    /// Cout start networking
    cout << "Info : Starting networking on localhost(127.0.0.1)" << endl;

    /// Choose poart
    Network* network = GetSubsystem<Network>();

    /// startserver
    network->StartServer(Port);

    /// SubscribetoEvents
    SubscribeToEvent(E_NETWORKMESSAGE, HANDLER(GameEconomicServer, HandleNetworkMessage));
    SubscribeToEvent(E_CLIENTIDENTITY, HANDLER(GameEconomicServer, NewConnectionIdentity));
    SubscribeToEvent(E_CLIENTCONNECTED, HANDLER(GameEconomicServer, NewConnection));

    return;
}

void GameEconomicServer::CoreInitialization(void)
{
    /// create scene
    Scene * scene_ = new Scene(context_);

    cout << "[Info]Starting scene." << endl;

    return;
}


