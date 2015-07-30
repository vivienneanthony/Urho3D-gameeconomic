#ifndef SERVERCONSOLEINTERFACE_H
#define SERVERCONSOLEINTERFACE_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/Thread.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Network/Network.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include <iostream>

using namespace Urho3D;
using namespace std;

/// Key pressed
EVENT(SERVER_CONSOLEEVENT, server_consolecommand)
{
    PARAM(P_CMD, cmdCommand);                // States
    PARAM(P_ARG, cmdArguments);
    PARAM(P_OBJ, sender);                // class
}
///}

struct networkconfiguration
{
    /// basic configuration
    String hostserver;
    unsigned int hostport;
    String hostidentity;
};


class ServerConsoleInterface : public LogicComponent, public Thread
{
    OBJECT(ServerConsoleInterface)
public:
    ServerConsoleInterface(Context * context);
    virtual ~ServerConsoleInterface();

    /// Process the connection in the worker thread until closed.
    virtual void ThreadFunction();


    /// Register
    void RegisterObject(Context * context);
    void Start();

/// Public functions
    bool GetLine(String & returnstring);
    Vector<String> ParseCommand(String EnteredString);
    void PrintLine(String);
    void ExecuteCommand(String FirstCommand, Vector<String> Arguments);
    bool isActive(void);

    /// Networking
    bool HandleConnect(networkconfiguration &tempnetwork);
    void HandleSendMessage(String Message);
    void SetHostNetworkServer(networkconfiguration &networkconfig);
    void OnConnection(StringHash eventType, Urho3D::VariantMap& eventData);
    void OnDisconnection(StringHash eventType, Urho3D::VariantMap& eventData);
    void Stop(void);

protected:
private:
    bool isActiveFlag;
    bool serverconnection;

    networkconfiguration networksetup;
};

#endif
