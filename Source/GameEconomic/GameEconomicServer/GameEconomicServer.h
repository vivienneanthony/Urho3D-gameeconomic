#ifndef GAMEECONOMICSERVER_H
#define GAMEECONOMICSERVER_H


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

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/IOEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Network/Connection.h>


#include <Urho3D/IO/VectorBuffer.h>


#include "../GameEconomicApp.h"

#include "../GameEconomicComponents/connectorDB.h"
#include "../GameEconomicComponents/ServerConsoleInterface.h"

#include "../Accounts.h"
#include "../GameEconomicComponents/Player.h"

using namespace std;

class knet;

/// This first example, maintaining tradition, prints a "Hello World" message.
/// Furthermore it shows:
///     - Using the Sample / Application classes, which initialize the Urho3D engine and run the main loop
///     - Adding a Text element to the graphical user interface
///     - Subscribing to and handling of update events
class GameEconomicServer : public GameEconomicApp
{
    OBJECT(GameEconomicServer);

public:
    /// Construct.
    GameEconomicServer(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const
    {
        return
            "<patch>"
            "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Hat0']]\">"
            "        <attribute name=\"Is Visible\" value=\"false\" />"
            "    </add>"
            "</patch>";
    }

private:
    void Splash(void);
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, Urho3D::VariantMap& eventData);
    void OnServerConsoleInterfaceEvent(StringHash eventType, Urho3D::VariantMap& eventData);
    void Stop(void);

    /// configuration related
    bool LoadNetworkConfig(networkconfiguration &loadingnetwork);
    bool LoadDatabaseConfig(databaseconnection &loadingdbconnector);
    void DatabaseInitialization(void);

    string LoadMysqlFile(String MySqlFile);
    string TextFileToString(string filename);
    void CoreInitialization(void);

    /// Networking
    void NetworkInitialization(unsigned int Port);
    void HandleNetworkMessage(StringHash eventType, Urho3D::VariantMap& eventData);

    /// Command line
    void ExecuteCommand(String FirstCommand, Vector<String> Arguments);
    Vector<String> ParseCommand(String EnteredString);

    /// Handle Commands
    void HandleNetworkCommands(Vector <String> &Arguments);
    void HandleAccountCommands(Vector <String> &Arguments);

    /// Database functions
    bool insertDBAccount(AccountInformation &TempAccount);
    bool deleteDBAccount(Vector<String> TableName,Vector<String> TableNameParameter);
    bool editDBAccount(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID);
    bool verifyDBAccount(Vector<String> TableName,Vector<String> TableNameParameter);
    Vector<String> selectDBAccount(Vector<String> TableName,Vector<String> TableNameParameter);

    bool ListAllDBAccounts(void);

    /// testing
    void insertDBPlayer(Player &TempPlayer);
    void deleteDBPlayer(Player &TempPlayer);
    void editDBPlayer(Player &TempPlayer);
    void verifyDBPlayer(Player &TempPlayer);
    void insertDBTrader(void);
    void deleteDBTrader(void);
    void editDBTrader(void);
    void verifyDBTrader(void);
    void insertDBCargo(void);
    void deleteDBCargo(void);
    void editDBCargo(void);
    void verifyDBCargo(void);
    void insertDBCargoBay(void);
    void deleteDBCargoBay(void);
    void editDBCargoBay(void);
    void verifyDBCargoBay(void);
    void insertDBMarket(void);
    void deleteDBMarket(void);
    void editDBMarket(void);
    void verifyDBMarket(void);
    void insertDBMarketTransaction(void);
    void deleteDBMarketTransaction(void);
    void editDBMarketTransaction(void);
    void verifyDBMarketTransaction(void);

    void NewConnectionIdentity(StringHash eventType, Urho3D::VariantMap& eventData);
    void NewConnection(StringHash eventType, Urho3D::VariantMap& eventData);
};

#endif
