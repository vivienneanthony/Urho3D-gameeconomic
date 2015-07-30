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


using namespace std;
using namespace Urho3D;

string GameEconomicServer::TextFileToString( string filename )
{
    ostringstream dosString( ios::out | ios::binary ) ; // *** binary
    ifstream inFile( filename.c_str() ) ;

    string line;
    while( getline(inFile, line) ) dosString << line;

    return dosString.str() ;
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
        if (DatabaseElem.HasAttribute("databasehostname")) loadingdbconnector.hostname = DatabaseElem.GetAttribute("databasehostname").Trimmed().CString();
        if (DatabaseElem.HasAttribute("databaseusername")) loadingdbconnector.username= DatabaseElem.GetAttribute("databaseusername").Trimmed().CString();
        if (DatabaseElem.HasAttribute("databasepassword")) loadingdbconnector.password = DatabaseElem.GetAttribute("databasepassword").Trimmed().CString();
        if (DatabaseElem.HasAttribute("databaseschema")) loadingdbconnector.schema = DatabaseElem.GetAttribute("databaseschema").Trimmed().CString();

        success=1;
    }

    return success;
}


/// Load network configuration
bool GameEconomicServer::LoadNetworkConfig(networkconfiguration &loadingnetwork)
{
    /// Grab resources
    FileSystem * fileSystem = GetSubsystem<FileSystem>();

    bool success=false;

    /// Create String
    String configFileName;

    /// Set directory and path for network file
    configFileName.Append(fileSystem->GetProgramDir().CString());
    configFileName.Append("Configuration/");
    configFileName.Append("NetworkServer.xml");

    /// Set default cleaing all settings
    loadingnetwork.hostidentity.Clear();
    loadingnetwork.hostserver.Clear();
    loadingnetwork.hostport=0;

    /// If file does not exist exit function with null structure
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
        return false;
    }

    XMLElement NetworkElem = configElem.GetChild("NetworkServer");

    /// If no network server element return false;
    if (!NetworkElem.IsNull())
    {
        if (NetworkElem.HasAttribute("NetworkServerHost")) loadingnetwork.hostserver= NetworkElem.GetAttribute("NetworkServerHost").Trimmed();
        if (NetworkElem.HasAttribute("NetworkServerIdentity")) loadingnetwork.hostidentity= NetworkElem.GetAttribute("NetworkServerIdentity").Trimmed();
        if (NetworkElem.HasAttribute("NetworkServerPort")) loadingnetwork.hostport = NetworkElem.GetInt("NetworkServerPort");

        success=true;
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


/// Database Initialization
void GameEconomicServer::DatabaseInitialization(void)
{
    /// Initialization of the DB
    connectorDB * connectionDB= GetSubsystem<connectorDB>();

    /// check if Markets Exist
    vector<string> databasetables;

    databasetables.push_back("Markets");
    databasetables.push_back("MarketTransactions");
    databasetables.push_back("Traders");
    databasetables.push_back("CargoBays");
    databasetables.push_back("CargoBayCatalog");
    databasetables.push_back("Players");
    databasetables.push_back("Accounts");

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
