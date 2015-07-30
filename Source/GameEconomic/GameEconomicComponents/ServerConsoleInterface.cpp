#include <Urho3D/Urho3D.h>


#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/Thread.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Engine/EngineEvents.h"
#include "../../../Urho3D/Network/Network.h"
#include "../../../Urho3D/Network/NetworkEvents.h"


#include <iostream>

#include "ServerConsoleInterface.h"

using namespace std;
using namespace Urho3D;


ServerConsoleInterface::ServerConsoleInterface(Context * context):
    LogicComponent(context),
    isActiveFlag(false)
{
    /// clear other varaiables
    networksetup.hostserver.Clear();
    networksetup.hostport = 0;
    networksetup.hostidentity.Clear();

    /// Only the physics update event is needed: unsubscribe from the rest for optimization

    SetUpdateEventMask(USE_FIXEDUPDATE);

    return;
}

/** @brief ~ServerConsoleInterface
  *
  * @todo: document this function
  */
ServerConsoleInterface::~ServerConsoleInterface()
{



}

/// Register Object
void ServerConsoleInterface::RegisterObject(Context * context)
{
    context_ -> RegisterFactory<ServerConsoleInterface>();


    return;
}

/// Server Console Start
void ServerConsoleInterface::Start()
{
    Thread::Run();
}


/// Getline
bool ServerConsoleInterface::GetLine(String & returnstring)
{
    /// Define a String
    string getpromptcommand;

    /// write line
    bool success = getline (std::cin,getpromptcommand);

    /// Clear String
    returnstring.Clear();

    /// Append input to a string
    returnstring.Append(getpromptcommand.c_str());

    return success;
}

/// Parse command
Vector<String> ServerConsoleInterface::ParseCommand(String EnteredString)
{
    /// Vector String
    Vector<String> SplitEntered;

    /// Split
    SplitEntered = EnteredString.Split(' ');

    return SplitEntered;
}

void ServerConsoleInterface::ExecuteCommand(String FirstCommand, Vector<String> Arguments)
{
    /// String
    String NewArguments;

    /// Don't waste time adding stuff
    if(Arguments.Size()>0)
    {
        for(unsigned int i=0; i<Arguments.Size(); i++)
        {
            NewArguments.Append(Arguments[i]);
            NewArguments.Append(" ");
        }
    }

    /// Create a event
    VariantMap sendevent;

    /// create command
    sendevent[server_consolecommand::P_CMD] = FirstCommand.Trimmed();
    sendevent[server_consolecommand::P_ARG] = NewArguments.Trimmed();

    /// Send Event
    this->SendEvent(SERVER_CONSOLEEVENT,sendevent);

    return;
}


/// Actual thread function
void ServerConsoleInterface::ThreadFunction()
{
    /// turn on inteface
    isActiveFlag=true;

    /// Urho related variables
    String PromptInput;
    Vector<String> SplitPromptInput;


    /// loop
    do
    {
        // Clear prompt input
        PromptInput.Clear();
        SplitPromptInput.Clear();


        cout << endl << "Enter Command >> ";


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

        if(CleanedPromptInput==String("quit"))
        {
            Stop();
        }
    }

    while(isActiveFlag);

    return;
}


/// handle connection
bool ServerConsoleInterface::HandleConnect(networkconfiguration &tempnetwork)
{

    /// connect to server
    Network* network = GetSubsystem<Network>();

    cout << "Connecting Server Console Interface to Host ("<< tempnetwork.hostserver.CString() << " on port " << tempnetwork.hostport << ")." << endl;

    bool success=network->Connect(tempnetwork.hostserver, tempnetwork.hostport, 0);

    return success;

}

/// copy network configuration
void ServerConsoleInterface::SetHostNetworkServer(networkconfiguration &networkconfig)
{
    /// Simple.copy
    networksetup.hostserver = networkconfig.hostserver.Trimmed();
    networksetup.hostport = networkconfig.hostport;
    networksetup.hostidentity = networkconfig.hostidentity.Trimmed();

    return;

}

/// Handle message send
void ServerConsoleInterface::HandleSendMessage(String Message)
{
    /// if input is empty leave
    if (Message.Empty())
    {

        return; // Do not send an empty message

    }

    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    if(!serverConnection)
    {
        return;
    }

    if (serverConnection)
    {
        // A VectorBuffer object is convenient for constructing a message to send
        VectorBuffer msg;
        msg.WriteString(Message);

        serverConnection->SendMessage(999, true, true, msg);
    }

    return;
}

void ServerConsoleInterface::OnConnection(StringHash eventType, Urho3D::VariantMap& eventData)
{
    /// server connection
    serverconnection = true;

    return;
}

void ServerConsoleInterface::OnDisconnection(StringHash eventType, VariantMap& eventData)
{
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    serverconnection = false;

    return;
}

void ServerConsoleInterface::Stop(void)
{


    /// Terminate
    exit(0);

    return;
}
