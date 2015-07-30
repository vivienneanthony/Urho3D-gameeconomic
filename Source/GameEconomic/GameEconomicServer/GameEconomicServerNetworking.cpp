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
#include <Urho3D/Network/Connection.h>

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

class GameEconomicServer;

void GameEconomicServer::HandleNetworkMessage(StringHash eventType, Urho3D::VariantMap& eventData)
{
    Network* network = GetSubsystem<Network>();

    using namespace NetworkMessage;

    int msgID = eventData[P_MESSAGEID].GetInt();

    /// Urho related variables
    String PromptInput;
    Vector<String> SplitPromptInput;

    if (msgID == 999)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);


        String text = msg.ReadString();

        Urho3D::Connection* sender = static_cast<Urho3D::Connection*>(eventData[P_CONNECTION].GetPtr());

        /// parse command
        SplitPromptInput = ParseCommand(text.Trimmed());

        /// First command
        String FirstCommand = SplitPromptInput[0];

        /// Check sisze for arguments
        if(SplitPromptInput.Size()>0)
        {
            /// Remove first element since its not needed
            SplitPromptInput.Erase(0);
        }

        cout << "message received" << endl;

        /// ExecuteCommand
        ExecuteCommand(FirstCommand, SplitPromptInput);
    }
}

void GameEconomicServer::NewConnectionIdentity(StringHash eventType, Urho3D::VariantMap& eventData)
{
    /// get connection
    Urho3D::Connection * newConnection = (Urho3D::Connection *) eventData[ClientConnected::P_CONNECTION].GetPtr();

    cout << "GetIdentity" <<endl;

    return;
}


void GameEconomicServer::NewConnection(StringHash eventType, Urho3D::VariantMap& eventData)
{

    /// get connection
    Urho3D::Connection * newConnection = (Urho3D::Connection *) eventData[ClientConnected::P_CONNECTION].GetPtr();

    cout << "New connection established {Clent}" << newConnection->ToString().CString() <<endl;


    return;
}
