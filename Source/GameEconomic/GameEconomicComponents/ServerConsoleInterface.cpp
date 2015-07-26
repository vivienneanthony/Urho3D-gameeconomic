#include <Urho3D/Urho3D.h>


#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Engine/EngineEvents.h"

#include <iostream>

#include "ServerConsoleInterface.h"

using namespace std;
using namespace Urho3D;


ServerConsoleInterface::ServerConsoleInterface(Context * context):
    LogicComponent(context),
    isActiveFlag(false)
{

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
    /// turn on inteface
    isActiveFlag=true;

    /// Urho related variables
    String PromptInput;
    Vector<String> SplitPromptInput;

    /// loop
    do
    {
        /// Display prompt to the user
        cout << "Enter Command >> ";

        /// Get input from the user
        GetLine(PromptInput);

        ///PromptInput=GetConsoleInput();


        /// parse command
        SplitPromptInput = ParseCommand(PromptInput.Trimmed());

        if(SplitPromptInput.Empty())
        {
            cout << "Error. Invalid  command" << endl;
            continue;
        }

        /// First command
        String FirstCommand = SplitPromptInput[0];

        /// Check sisze for arguments
        if(SplitPromptInput.Size()>0)
        {
            /// Remove first element since its not needed
            SplitPromptInput.Erase(0);
        }

        /// ExecuteCommand
        ExecuteCommand(FirstCommand, SplitPromptInput);

    }
    while(isActiveFlag);

    return;
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
