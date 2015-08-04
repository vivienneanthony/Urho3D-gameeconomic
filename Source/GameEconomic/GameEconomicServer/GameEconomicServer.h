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
#include "../Administrator.h"
#include "../GameEconomicComponents/Player.h"
#include "../GameEconomicServer/Networking.h"

using namespace std;

enum  DBTable
{
    DBAccount,
    DBAdministrator
};

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
    void ExecuteCommand(String FirstCommand, Vector<String> Arguments, Urho3D::Connection * sender);
    Vector<String> ParseCommand(String EnteredString);

    /// Handle Commands
    void HandleNetworkCommands(Vector <String> &Arguments, Urho3D::Connection * sender);
    void HandleAccountCommands(Vector <String> &Arguments, Urho3D::Connection * sender);
    void HandleAdministratorCommands(Vector <String> &Arguments, Urho3D::Connection * sender);
    void HandlePlayerCommands(Vector <String> &Arguments, Urho3D::Connection * sender);

    /// Database functions
    bool insertDBAccount(AccountInformation &TempAccount);
    bool deleteDBAccount(Vector<String> TableName,Vector<String> TableNameParameter);
    bool editDBAccount(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID);
    bool verifyDBAccount(Vector<String> TableName,Vector<String> TableNameParameter);
    Vector<String> selectDBAccount(Vector<String> TableName,Vector<String> TableNameParameter);

    String ListAllDBAccounts(Urho3D::Connection * sender);
    bool UpdateSingleDBAccount(AccountInformation &TempAccount);
    bool DeleteSingleDBAccount(AccountInformation &TempAccount);

    /// Database functions related to administrator
    bool insertDBAdministrator(AdministratorInformation &TempAdministrator);
    bool deleteDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter);
    bool editDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID);
    bool verifyDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter);
    Vector<String> selectDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter);

    String ListAllDBAdministrators(void);
    bool UpdateSingleDBAdministrator(AdministratorInformation &TempAdministrator);
    bool DeleteSingleDBAdministrator(AdministratorInformation &TempAdministrator);

    bool insertDBPlayer(PlayerObject &TempPlayer);
    bool deleteDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter);
    bool editDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID);
    bool verifyDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter);
    Vector<String> selectDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter);

    String ListAllDBPlayers(Urho3D::Connection * sender);
    bool UpdateSingleDBPlayer(PlayerObject &TempPlayer);
    bool DeleteSingleDBPlayer(PlayerObject &TempPlayer);

    /// Networking
    void NewConnectionIdentity(StringHash eventType, Urho3D::VariantMap& eventData);
    void NewConnection(StringHash eventType, Urho3D::VariantMap& eventData);

    /// Push connection
    void NetworkingOnUpdate(float timeStep);

    void SendNetworkMessage(NetworkMessageTypes NetworkMessageType, bool flag1, bool flag2, String MessageText, Urho3D::Connection * SenderTo);

    bool EmailValidCheck(String EmailAddress);
    bool VerifyIdentityDB(DBTable mode, String username, String password);

    Vector<SharedPtr<Urho3D::Connection> > allConnections;

};

#endif
