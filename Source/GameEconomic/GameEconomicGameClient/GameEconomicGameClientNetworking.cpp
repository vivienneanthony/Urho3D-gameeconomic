//
// Copyright (c) 2008-2014 the Urho3D project.
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

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/UI/Font.h"
#include "../../../Urho3D/Input/Input.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/UI/Text.h"
#include "../../../Urho3D/UI/UI.h"
#include "../../../Urho3D/Scene/Scene.h"
#include "../../../Urho3D/Graphics/StaticModel.h"
#include "../../../Urho3D/Graphics/Octree.h"
#include "../../../Urho3D/Graphics/Model.h"
#include "../../../Urho3D/Graphics/Material.h"
#include "../../../Urho3D/Graphics/Camera.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Graphics/Renderer.h"
#include "../../../Urho3D/Graphics/Camera.h"
#include "../../../Urho3D/UI/Window.h"
#include "../../../Urho3D/UI/Button.h"
#include "../../../Urho3D/UI/LineEdit.h"
#include "../../../Urho3D/UI/UIElement.h"
#include "../../../Urho3D/Math/BoundingBox.h"
#include "../../../Urho3D/UI/UIEvents.h"
#include "../../../Urho3D/Graphics/DebugRenderer.h"
#include "../../../Urho3D/IO/File.h"
#include "../../../Urho3D/IO/FileSystem.h"
#include "../../../Urho3D/Resource/XMLFile.h"
#include "../../../Urho3D/Resource/XMLElement.h"
#include "../../../Urho3D/IO/Deserializer.h"
#include "../../../Urho3D/UI/Cursor.h"
#include "../../../Urho3D/IO/FileSystem.h"
#include "../../../Urho3D/UI/ListView.h"
#include "../../../Urho3D/Engine/Console.h"
#include "../../../Urho3D/Physics/RigidBody.h"
#include "../../../Urho3D/Physics/CollisionShape.h"
#include "../../../Urho3D/Physics/PhysicsWorld.h"
#include "../../../Urho3D/Graphics/Animation.h"
#include "../../../Urho3D/Graphics/AnimatedModel.h"
#include "../../../Urho3D/Graphics/AnimationController.h"
#include "../../../Urho3D/UI/BorderImage.h"
#include "../../../Urho3D/Graphics/GraphicsEvents.h"
#include "../../../Urho3D/Graphics/Terrain.h"
#include "../../../Urho3D/Engine/EngineEvents.h"
#include "../../../Urho3D/Graphics/Zone.h"
#include "../../../Urho3D/IO/Log.h"
#include "../../../Urho3D/Graphics/Skybox.h"
#include "../../../Urho3D/UI/Sprite.h"
#include "../../../Urho3D/Graphics/StaticModelGroup.h"
#include "../../../Urho3D/Graphics/BillboardSet.h"
#include "../../../Urho3D/Math/Random.h"
#include "../../../Urho3D/Graphics/RenderPath.h"
#include "../../../Urho3D/Math/Color.h"
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Network/Connection.h>
#include <Urho3D/IO/MemoryBuffer.h>

#include "../GameEconomicComponents/GameStateHandlerComponent.h"
#include "../GameEconomicComponents/GameStateEvents.h"
#include "../GameEconomicComponents/Accounts.h"
#include "../Networking.h"
#include "../ServerResponse.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <locale>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <algorithm>

#include "GameEconomicGameClient.h"


#include "../../Urho3D/Engine/DebugHud.h"

#ifdef WIN32
#undef SendMessage
#endif

using namespace std;
using namespace Urho3D;

/// Handler network message
void GameEconomicGameClient::HandleNetworkMessage(StringHash eventType, Urho3D::VariantMap& eventData)
{

    Network* network = GetSubsystem<Network>();

    using namespace NetworkMessage;

    unsigned int msgID = eventData[P_MESSAGEID].GetInt();

    /// Urho related variables
    String PromptInput;
    Vector<String> SplitPromptInput;

    /// Determine message
    if(msgID == NetMessageAuthenticateAcknowledge)
    {
        /// use this information to encrypt
        String SendString(NetUserAuthenticateInfo.Username+String(" ")+NetUserAuthenticateInfo.Password);


        SendNetworkMessage(NetMessageAuthenticateSend, SendString);
    }

    /// Determine message
    if(msgID == NetMessageRequestApprovedGetAccountPlayers)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;

        serverresponse[ServerResponse::P_CMD] = ServerResponse_SentAccountPlayers;

        serverresponse[ServerResponse::P_ARG] = text;

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);
    }

    if(msgID == NetMessageRequestApprovedGetFactions)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        /// Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Clear Old Date
        ThisFactionList.Empty();

        /// Get results from new player list
        ThisFactionList = LoadGetFactionsFromAuthorization(text);

        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;

        serverresponse[ServerResponse::P_CMD] = ServerResponse_SentFactions;

        serverresponse[ServerResponse::P_ARG] = text;

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);


    }

    if(msgID == NetMessageRequestApprovedGetPlayerCreationResponse)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        /// Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;

        serverresponse[ServerResponse::P_CMD] = ServerResponse_CreatePlayerResponse;

        serverresponse[ServerResponse::P_ARG] = text;

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);
    }

    if(msgID == NetMessageRequestApprovedGetStarbase)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        /// Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;

        serverresponse[ServerResponse::P_CMD] = ServerResponse_SentStarbase;

        serverresponse[ServerResponse::P_ARG] = text;

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);
    }

    if(msgID == NetMessageRequestApprovedAddStarbaseResponse)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        /// Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;

        serverresponse[ServerResponse::P_CMD] = ServerResponse_AddedStarbase;

        serverresponse[ServerResponse::P_ARG] = text;

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);
    }


    if(msgID == NetMessageRequestApprovedGetPlayerDetail)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        /// Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;

        serverresponse[ServerResponse::P_CMD] = ServerResponse_SentPlayerFullDetail;

        serverresponse[ServerResponse::P_ARG] = text;

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);
    }


    /// If we get all the aliens
    if(msgID == NetMessageRequestApprovedGetAlienRaces)
    {
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Clear Old Date
        ThisAlienRaceList.Empty();

        /// Get results from new player list
        ThisAlienRaceList = LoadGetAlienRacesFromAuthorization(text);


        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;

        serverresponse[ServerResponse::P_CMD] = ServerResponse_SentAlienRaces;

        serverresponse[ServerResponse::P_ARG] = text;

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);


    }



    /// Determine message authenicated or approved
    if(msgID == NetMessageAuthenticatedApproved
            || msgID == NetMessageAuthenticatedDenied)
    {

        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        /// Create a event and send it for login can listen to it
        VariantMap  serverresponse;
        serverresponse[ServerResponse::P_CMD] = ServerResponse_AccountAuthentication;

        /// Msg Authenticate
        if(msgID == NetMessageAuthenticatedApproved)
        {
            serverresponse[ServerResponse::P_ARG] = text;
        }
        else
        {
            serverresponse[ServerResponse::P_ARG] = String("Unauthorized");
        }

        /// Send the event
        SendEvent(N_SERVER_RESPONSE,serverresponse);
    }

    return;
}

/// Send a network message
void GameEconomicGameClient::SendNetworkMessage(NetworkMessageTypes MessageType, String Message)
{
    /// If sending message is empty
    if (Message.Empty())
    {
        return; // Do not send an empty message
    }

    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    /// this is the message to send the password
    if(MessageType == NetMessageAuthenticateSend)
    {
        /// create a new message
        VectorBuffer msg;
        msg.WriteString(Message);

        ///cout << "Sending NetMessageAuthenticateSend (" << Message.CString() << ")." <<endl;
        serverConnection->SendMessage(( int)NetMessageAuthenticateSend, true, true, msg, (int)0);
    }

    return;
}

bool GameEconomicGameClient::LoadNetworkConfig(NetworkConfiguration &loadingnetwork)
{
    /// Grab resources
    FileSystem * fileSystem = GetSubsystem<FileSystem>();

    bool success=false;

    /// Create String
    String configFileName;

    /// Set directory and path for network file
    configFileName.Append(fileSystem->GetProgramDir().CString());
    configFileName.Append("");
    configFileName.Append("NetworkServer.xml");

    /// Set default cleaing all settings
    loadingnetwork.hostidentity=Unauthenticated;
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
        ///  if (NetworkElem.HasAttribute("NetworkServerIdentity")) loadingnetwork.hostidentity= NetworkElem.GetAttribute("NetworkServerIdentity").Trimmed();
        if (NetworkElem.HasAttribute("NetworkServerPort")) loadingnetwork.hostport = NetworkElem.GetInt("NetworkServerPort");

        success=true;
    }

    return success;
}


void GameEconomicGameClient::HandlerServerConnected(StringHash eventType, Urho3D::VariantMap& eventData)
{
    /// Get current network
    Network* network = GetSubsystem<Network>();

    /// if disconnect and offline try to connect
    if(NetOnline==NetDisconnected)
    {
        Connection* serverConnection = network->GetServerConnection();

        // If we were connected to server, disconnect
        if (serverConnection)
        {
            serverConnection->Disconnect();
        }

        /// Set network to online
        NetStats=NetworkOnline;

        UpdateNetworkStatusUI(true);
    }
}

void GameEconomicGameClient::HandlerServerDisconnected(StringHash eventType, Urho3D::VariantMap& eventData)
{
    if(NetOnline==NetConnected)
    {
        NetOnline=NetDisconnected;
    }

    /// blank
}

void GameEconomicGameClient::HandlerServerConnectionFailed(StringHash eventType, Urho3D::VariantMap& eventData)
{
    /// if disconnect and offline try to connect
    if(NetOnline==NetDisconnected)
    {
        /// Set network to online
        NetStats=NetworkOffline;

        UpdateNetworkStatusUI(false);
    }
}

/// network on update
void GameEconomicGameClient::NetworkingOnUpdate(float timeStep)
{
    /// Get current network
    Network* network = GetSubsystem<Network>();

    static float testnetworkupdate=0;

    testnetworkupdate+=timeStep;

    /// if timer pass 20 seconds
    if(testnetworkupdate>15)
    {
        /// if disconnect and offline try to connect
        if(NetOnline==NetDisconnected)
        {
            /// if the hostname is not empty attempt to connect
            if(!NetConfig.hostserver.Empty())
            {
                ConnectHearbeat(NetConfig);
            }
        }

        /// Reset timer
        testnetworkupdate=0;
    }
    return;
}


/// handle connection
void GameEconomicGameClient::ConnectHearbeat(NetworkConfiguration &tempnetwork)
{
    /// Get Urho subsystem
    Network* network = GetSubsystem<Network>();

    /// Define Variables
    bool success=true;

    /// Get currenttime
    Urho3D::Time systemtime(context_);
    unsigned int currenttime = systemtime.GetSystemTime();

    /// Get eventmap and time
    Urho3D::VariantMap NetworkClientIdentity;

    NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTYPE] = RequestHearbeat;
    NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTARRIVAL]= currenttime;

    /// Set Identity
    network->Connect(tempnetwork.hostserver, tempnetwork.hostport, 0, NetworkClientIdentity);

    return;
}

/// handle connection
void GameEconomicGameClient::ConnectLogin(NetworkConfiguration &tempnetwork)
{
    /// Get Urho subsystem
    Network* network = GetSubsystem<Network>();

    /// Define Variables
    bool success=true;

    /// Get currenttime
    Urho3D::Time systemtime(context_);
    unsigned int currenttime = systemtime.GetSystemTime();

    /// Get eventmap and time
    Urho3D::VariantMap NetworkClientIdentity;

    NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTYPE] = Unauthenticated;
    NetworkClientIdentity[NetworkClientIdentity::NETWORK_CLIENTARRIVAL]= currenttime;

    /// Set Identity
    if(network->Connect(tempnetwork.hostserver, tempnetwork.hostport, 0, NetworkClientIdentity))
    {
        NetOnline=NetConnected;
    }

    return;
}


