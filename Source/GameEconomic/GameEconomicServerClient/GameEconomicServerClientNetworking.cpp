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

#include "GameEconomicServerClient.h"
#include "../Networking.h"


#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;


void GameEconomicServerClient::HandleNetworkMessage(StringHash eventType, Urho3D::VariantMap& eventData)
{
    Network* network = GetSubsystem<Network>();

    using namespace NetworkMessage;

    int msgID = eventData[P_MESSAGEID].GetInt();

    /// Urho related variables
    String PromptInput;
    Vector<String> SplitPromptInput;

    if(msgID == NetMessageAdminClientSendAcknowledge)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);

        String text = msg.ReadString();
        cout << "\r\n" << text.CString() <<endl;
    }
}

void GameEconomicServerClient::OnDisconnection(StringHash eventType, Urho3D::VariantMap& eventData)
{

    /// check network connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    bool success=false;

    /// If we were connected to server, disconnect
    if (serverConnection)
    {
        serverConnection->Disconnect();
        serverconnection = false;
    }

    return;
}

void GameEconomicServerClient::OnConnection(StringHash eventType, Urho3D::VariantMap& eventData)
{

    /// check network connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    bool success=false;

    /// If we were connected to server, disconnect
    if (serverConnection)
    {

        serverconnection = true;
    }

    return;
}


bool GameEconomicServerClient::LoadNetworkConfig(networkconfiguration &loadingnetwork)
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

void GameEconomicServerClient::OnConnectionFailed(StringHash eventType, VariantMap& eventData)
{
    serverconnection = false;

    return;
}


