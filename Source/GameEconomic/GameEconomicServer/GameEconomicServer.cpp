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
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/IO/File.h"
#include "../../../Urho3D/IO/FileSystem.h"
#include "../../../Urho3D/Resource/XMLFile.h"
#include "../../../Urho3D/Resource/XMLElement.h"

#include <iostream>
#include  <signal.h>
#include <sstream>
#include <fstream>
#include <vector>

#include "GameEconomicServer.h"

#include "../GameEconomicComponents/ServerConsoleInterface.h"
#include "../GameEconomicComponents/connectorDB.h"

#include "signalHandler.hpp"

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

    /// Execute base class startup
    GameEconomicApp::Start();

    SignalHandler signalHandler;

    // Register signal handler to handle kill signal
    signalHandler.setupSignalHandlers();

    /// Open the operating system console window (for stdin / stdout) if not open yet
    OpenConsoleWindow();

    /// Create a interface point
    ServerConsoleInterface * PrimaryConsoleInterface;


    /// odbc stuff here
    databaseconnection MyDB;


    cout << "Info: Loading database configuration ." << endl;

    bool  success = LoadDatabaseConfig(MyDB);

    /// if configuration loaded
    if(success)
    {

        cout << "Info: Loading database configuration successful." << endl;

        /// configure database
        connectionDB->setHost(MyDB.hostname,MyDB.username,MyDB.password);
        connectionDB->setDB(MyDB.schema);

        /// connect to host
        if(connectionDB->connectHost())
        {
            cout << "Info: Database connection succesful. "  << endl;

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


    /// show splash
    Splash();

    /// Create Interface Context
    PrimaryConsoleInterface = new ServerConsoleInterface(context_);

    /// Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    /// like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    /// could subscribe in the constructor instead.
    SubscribeToEvents();

    /// Start the interface
    PrimaryConsoleInterface -> Start();
}

/// Splash
void GameEconomicServer::Splash(void)
{
    cout << "Headless Server Model " << endl;
    cout << "Programmer Vivienne Anthony\r\n " << endl;
}

/// Subscribe to events
void GameEconomicServer::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(GameEconomicServer, HandleUpdate));

    /// execute console event
    SubscribeToEvent(SERVER_CONSOLEEVENT, HANDLER(GameEconomicServer,OnServerConsoleInterfaceEvent));

}

void GameEconomicServer::HandleUpdate(StringHash eventType, Urho3D::VariantMap& eventData)
{
    // Do nothing for now, could be extended to eg. animate the display
}

void GameEconomicServer::OnServerConsoleInterfaceEvent(StringHash eventType, Urho3D::VariantMap& eventData)
{
// Resources
    UI* ui_ = GetSubsystem<UI>();
    Engine * engine_ = GetSubsystem<Engine>();

    /// Get Datas
    String Command=eventData[server_consolecommand::P_CMD].GetString();
    String Arguments=eventData[server_consolecommand::P_ARG].GetString();

    /// Modify and trim
    Command = Command.Trimmed().ToLower();
    Arguments = Arguments.Trimmed();

    /// test command
    if(Command==String("quit"))
    {
        Stop();
        return;
    }

    /// Output
    cout << "Test entered" << Command.CString() << " " << Arguments.CString() << endl;
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


bool GameEconomicServer::LoadDatabaseConfig(databaseconnection &loadingdbconnector)
{
    /// Grab resources
    FileSystem * fileSystem = GetSubsystem<FileSystem>();

    bool success=0;

    /// Create String
    String configFileName;

    /// Set directory
    configFileName.Append(fileSystem->GetProgramDir().CString());
    configFileName.Append("Configuration/");
    configFileName.Append("Database.xml");

    /// If file does not exist exit function
    if (!fileSystem->FileExists(configFileName))
    {
        return false;
    }

    /// Flag file for loading and load
    File loadFile(context_, configFileName, FILE_READ);

    XMLFile * configurationXML = new XMLFile(context_);

    configurationXML -> Load(loadFile);

    XMLElement configElem = configurationXML->GetRoot();

    /// If no configuration is set or no root
    if (configElem.IsNull())
    {
        return  false;
    }

    XMLElement DatabaseElem = configElem.GetChild("Database");

    /// If no database element return false;
    if (!DatabaseElem.IsNull())
    {
        if (DatabaseElem.HasAttribute("databasehostname")) loadingdbconnector.hostname = DatabaseElem.GetAttribute("databasehostname").CString();
        if (DatabaseElem.HasAttribute("databaseusername")) loadingdbconnector.username= DatabaseElem.GetAttribute("databaseusername").CString();
        if (DatabaseElem.HasAttribute("databasepassword")) loadingdbconnector.password = DatabaseElem.GetAttribute("databasepassword").CString();
        if (DatabaseElem.HasAttribute("databaseschema")) loadingdbconnector.schema = DatabaseElem.GetAttribute("databaseschema").CString();

        success=1;
    }

    return success;
}

/// Load the Mysql file
string GameEconomicServer::LoadMysqlFile(String MySqlFile)
{
    /// get system
    FileSystem * fileSystem = GetSubsystem<FileSystem>();

    /// create path
    String ConfigFilename= fileSystem->GetProgramDir()+String("SQL/")+MySqlFile+String(".mysql");


    /// if config does not exist
    if (!fileSystem->FileExists(ConfigFilename))
    {
        return false;
    }

    string returnString = TextFileToString(ConfigFilename.CString());


    return returnString;

}

void GameEconomicServer::DatabaseInitialization(void)
{
    /// Initialization of the DB
    connectorDB * connectionDB= GetSubsystem<connectorDB>();

    /// check if Markets Exist
    vector<string> databasetables;

    databasetables.push_back("Markets");
    databasetables.push_back("MarketsTransactions");
    databasetables.push_back("Trader");
    databasetables.push_back("CargoBay");
    databasetables.push_back("CargoBayCatalog");

    /// loop through all database tables
    for(unsigned int i=0; i<databasetables.size(); i++)
    {
        /// check if the primary database is created for markets
        if(connectionDB->checkTable(databasetables.at(i)))
        {
            cout << "Info: Database "<< databasetables.at(i)<< " table found." << endl;
        }
        else
        {
            cout << "Info: Database "<< databasetables.at(i) << " table not found." << endl;


            cout << "Info: Database "<< databasetables.at(i) << " table loading mysql file." << endl;

            /// attempting to load sql and run
            string marketstable = LoadMysqlFile(databasetables.at(i).c_str());

            if(!marketstable.empty())
            {
                cout << "Info: Database "<< databasetables.at(i) << " table executing build table." << endl;

                connectionDB->executePreparedStatement(marketstable);
            }
            else
            {
                cout << "Info: Database "<< databasetables.at(i) << " table problem loading mysql file." << endl;

            }
        }

    }

    return;
}

string GameEconomicServer::TextFileToString( string filename )
{
    ostringstream dosString( ios::out | ios::binary ) ; // *** binary
    ifstream inFile( filename.c_str() ) ;

    string line;
    while( getline(inFile, line) ) dosString << line;

    return dosString.str() ;
}


