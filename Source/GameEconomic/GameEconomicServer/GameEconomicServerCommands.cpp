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
#include <Urho3D/Container/Str.h>
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
#include "../Accounts.h"


using namespace std;
using namespace Urho3D;


/// Parse command
Vector<String> GameEconomicServer::ParseCommand(String EnteredString)
{
    /// Vector String
    Vector<String> SplitEntered;

    /// Split
    SplitEntered = EnteredString.Split(' ');

    return SplitEntered;
}

void GameEconomicServer::ExecuteCommand(String FirstCommand, Vector<String> Arguments)
{
    /// removed old code;
    String Command=FirstCommand.ToLower().Trimmed();

    /// test command
    if(Command==String("quit"))
    {
        cout << "Shutting down." << endl;

        Stop();
    }
    else if(Command==String("name"))
    {
        cout << "My name is Caspian identification 1701-A prototype core Existence." << endl;
    }
    else if(Command==String("network"))
    {
        HandleNetworkCommands(Arguments);
    }
    else if(Command==String("accounts"))
    {
        HandleAccountCommands(Arguments);
    }

    return;
}

/// Handle All Acounts
void GameEconomicServer::HandleNetworkCommands(Vector <String> &Arguments)
{
    /// Get console connection
    if(Arguments.At(0) == String("getserverconnection"))
    {
        /// Get network info
        Network* network = GetSubsystem<Network>();
        Urho3D::Connection * serverConnection =  network->GetServerConnection();

        /// Output
        cout << "Server connection : " << serverConnection->ToString().CString();
    }

    return;
}

/// Handle All Acounts
void GameEconomicServer::HandleAccountCommands(Vector <String> &Arguments)
{

    if(Arguments.At(0) == String("addaccount"))
    {

        /// if accounts less then needed write message
        if(Arguments.Size()<7)
        {
            cout << "You need to full up all arguments";
        }

        /// create a tempaccount
        AccountInformation TempAccount;

        /// copy database
        TempAccount.Username = Arguments.At(1);
        TempAccount.Firstname = Arguments.At(2);
        TempAccount.Middlename = Arguments.At(3);
        TempAccount.Lastname = Arguments.At(4);
        TempAccount.Email = Arguments.At(5);
        TempAccount.Password = Arguments.At(6);

        insertDBAccount(TempAccount);
    }

    if(Arguments.At(0) == String("listselectaccounts"))
    {
        ListAllDBAccounts();
    }

    return;

}

