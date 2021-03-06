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
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/Renderer2D.h>

#include "../GameEconomicComponents/GameStateHandlerComponent.h"
#include "../GameEconomicComponents/GameStateEvents.h"

#include "../GameEconomicComponents/Accounts.h"
#include "../GameEconomicComponents/Starbase.h"
#include "../GameEconomicComponents/PowerComponent.h"
#include "../GameEconomicComponents/ResourceNodeComponent.h"
#include "../GameEconomicComponents/InteractObject.h"
#include "../GameEconomicComponents/Entity.h"
#include "../GameEconomicComponents/AIController.h"
#include "../GameEconomicComponents/Drone.h"
#include "../ServerResponse.h"
#include "../Networking.h"
#include "../UniqueGenerator.h"

#include "../Player.h"

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
#include "GameEconomicGameClientStateProgress.h"

#include "../../Urho3D/Engine/DebugHud.h"

#define DEFAULTSIZE 4096

using namespace std;
using namespace Urho3D;

GameEconomicGameClientStateProgress::GameEconomicGameClientStateProgress(Context * context)
    :GameEconomicGameClientStateSingleton(context)
    ,Existence(baseclass)
    ,FirstTime(false)
{
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Set Application
    Existence = gamestatehandlercomponent_->GetApplication();

    ///CHange Stater4==
    gamestatehandlercomponent_->SetUIState(UI_PROGRESSINTERFACE);

    /// Progress construct
    cout << "Debug: Progress Constructor" << endl;
}

GameEconomicGameClientStateProgress::~GameEconomicGameClientStateProgress()
{
    //dtor
    cout << "Debug: Progress Deconstructor" << endl;
}

void GameEconomicGameClientStateProgress::Enter()
{
    /// Send full request for a user name
    Network* Network_ = GetSubsystem<Network>();
    Connection* ServerConnection = Network_->GetServerConnection();

    /// Clear Everything
    Existence->EraseUI();

    ContinueGame = false;

    /// Attach a listen
    SubscribeToEvent(N_SERVER_RESPONSE, HANDLER(GameEconomicGameClientStateProgress, ServerResponseHandler));

    /// Progress screen
    ProgressScreenViewport();

    ProgressScreenUI();

    /// Request player full detail
    String ServerRequest;
    ServerRequest.Append("requestplayerdetail ");
    ServerRequest.Append(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).UniqueID);

    VectorBuffer msg;
    msg.WriteString(ServerRequest);

    ServerConnection->SendMessage(NetMessageRequest,true,true,msg,0);


    return;
}

void GameEconomicGameClientStateProgress::Exit()
{
    //dtor
}
void GameEconomicGameClientStateProgress::OnUpdate(StringHash eventType, VariantMap& eventData)
{
    // dtor
}

void GameEconomicGameClientStateProgress::SetParameter(String parameter_)
{
    /// Do Nothing
    return;
}

void GameEconomicGameClientStateProgress::Intro(void)
{
    /// Get Needed SubSystems
    UI* ui = GetSubsystem<UI>();

    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();
    Graphics * graphics_ = GetSubsystem <Graphics>();

    /// Get all Revelant resources
    ResourceCache* cache_ = GetSubsystem<ResourceCache>();
    Renderer* renderer_ = GetSubsystem<Renderer>();


    /// Get the root
    UIElement * uiRoot_ = ui->GetRoot();

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    /// Load xml file
    Existence->loadHUDFile("Resources/UI/DialogWindowIntro.xml",0,0);

    UIElement * DialogWindowIntroUIElement = uiRoot_->GetChild("DialogWindowIntroUIElement", true);

    Window * DialogWindow = (Window *) uiRoot_->GetChild("DialogWindowIntro",true);
    Text * DialogText = (Text *) uiRoot_->GetChild("DialogText",true);
    Button * DialogWindowContinueButton = (Button *) uiRoot_->GetChild("DialogWindowContinueButton",true);

    DialogWindowIntroUIElement->SetPosition((width/2)-(DialogWindow->GetWidth()/2),(height/2)-(DialogWindow->GetHeight()/2));

    DialogText -> SetText ("2837-Class B Unknown. 18273-Class M Unknown. .Log. We jumped through the jumpgate as planned but something went wrong. <shaking head> Everyone is lost. NCC Zefram.  Our ship. <shying> I guess I'm a explorer now. I've manage to savage some drones and supplies enougth to build a base with a hangar to survive but its not enougth. ");

    DialogText->SetFixedSize(490,384);

    /// Create dialog
    loadScene();

    /// Subscribe to events
    SubscribeToEvent(DialogWindowContinueButton,E_RELEASED,HANDLER(GameEconomicGameClientStateProgress,HandleContinuePressed));

    return;
}

void GameEconomicGameClientStateProgress::HandleContinuePressed(StringHash eventType, VariantMap& eventData)
{
    /// Get Needed SubSystems
    UI* ui = GetSubsystem<UI>();

    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();
    Graphics * graphics_ = GetSubsystem <Graphics>();

    /// Get all Revelant resources
    ResourceCache* cache_ = GetSubsystem<ResourceCache>();
    Renderer* renderer_ = GetSubsystem<Renderer>();

    /// if continue switch
    if(ContinueGame)
    {
        Existence->EraseUI();

        /// Create a event
        VariantMap gamestatechange;
        gamestatechange[GameState::P_CMD] = GAME_STATE_GAMEMODE;

        cout << "Debug: Attempt to send a state change" << endl;

        this->SendEvent(G_STATES_CHANGE,gamestatechange);
    }
}

void GameEconomicGameClientStateProgress::ServerResponseHandler(StringHash eventType, VariantMap& eventData)
{

    /// Get Needed SubSystems
    UI* ui = GetSubsystem<UI>();

    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    /// Get the root
    UIElement * uiRoot_ = ui->GetRoot();

    Text * ProgressText = (Text *) uiRoot_ -> GetChild("ProgressText",true);

    /// Take the frame time step, which is stored as a float
    unsigned int cmdType = eventData[ServerResponse::P_CMD].GetInt();
    String cmdArg = eventData[ServerResponse::P_ARG].GetString();

    /// if Account Authentication
    if(cmdType==ServerResponse_AddedStarbase)
    {
        if(cmdArg==String("approved"))
        {
            /// Update Progress
            if(ProgressText)
            {
                ProgressText->SetText("Requesting Starbase Parameters ... ");
            }

            /// attempt to insert a newe starbase
            String ServerRequest;
            ServerRequest.Append("requeststarbase ");
            ServerRequest.Append(Existence->ThisPlayer->UniqueID);

            VectorBuffer msg;
            msg.WriteString(ServerRequest);

            serverConnection->SendMessage(NetMessageRequest,true,true,msg,0);

        }
        else
        {
            ///cout << "Problem occurred";
            return;
        }
    }

    /// if Account Authentication
    if(cmdType==ServerResponse_SentStarbase)
    {
        ///cout << cmdArg.CString()<<endl;

        /// If unauthorized
        if(cmdArg==String("|0|0"))
        {
            /// Update Progress
            if(ProgressText)
            {
                ProgressText->SetText("Adding Starbase Parameters");
            }

            /// Flag first time
            FirstTime = true;

            /// attempt to insert a new starbase
            String ServerRequest;
            ServerRequest.Append("addstarbase ");
            ServerRequest.Append(Existence->ThisPlayer->UniqueID);

            ServerRequest.Append(" Undiscovered ");
            ServerRequest.Append("MapData ");

            VectorBuffer msg;
            msg.WriteString(ServerRequest);

            serverConnection->SendMessage(NetMessageRequest,true,true,msg,0);
        }
        else
        {
            /// Update Progress
            if(ProgressText)
            {
                ProgressText->SetText("Initialization Parameters Verified");
            }

            /// delete original starbase
            delete Existence->ThisStarbase;

            Existence->ThisStarbase = new StarbaseInformation();

            Existence->ThisStarbase = ConvertServerStringtoStarbase(cmdArg);

            cout << "Everything should be ready to go" << endl;

            ///
            ContinueGame = true;

            Intro();
        }
    }

    /// if Account Authentication
    if(cmdType==ServerResponse_SentPlayerFullDetail)
    {
        /// Update Progress
        if(ProgressText)
        {
            ProgressText->SetText("Received Player Detail Parameters");
        }

        /// If unauthorized
        if(cmdArg==String("|0|0"))
        {
            exit(0);
        }
        else
        {
            /// delete original starbase
            delete Existence->ThisPlayer;

            Existence->ThisPlayer = new PlayerObject();

            Existence->ThisPlayer = ConvertServerStringtoPlayer(cmdArg);

        }

        /// there was a error show warning then go back to the main screen
        if(cmdArg==String("|0|0")||Existence->ThisPlayer == NULL)
        {
            exit(0);
        }
        else
        {
            /// Update Progress
            if(ProgressText)
            {
                ProgressText->SetText("Requesting Starbase Parameters");
            }

            /// attempt to insert a newe starbase
            String ServerRequest;
            ServerRequest.Append("requeststarbase ");
            ServerRequest.Append(Existence->ThisPlayer->UniqueID);

            VectorBuffer msg;
            msg.WriteString(ServerRequest);

            serverConnection->SendMessage(NetMessageRequest,true,true,msg,0);
        }
    }

    return;
}


/// Get account information
StarbaseInformation * GameEconomicGameClientStateProgress::ConvertServerStringtoStarbase(String ServerString)
{
    /// Change convert
    String TempString=ServerString;
    Vector <String> ServerStringSplit;

    /// Create a pointer
    StarbaseInformation * ReturnStarbaseInformation  = new StarbaseInformation();

    /// Replace | to a space
    TempString.Replace ('|', ' ', false);

    /// Split the string
    ServerStringSplit = TempString.Split(' ');

    if(ServerStringSplit.At(1)==String("0"))
    {
        return NULL;
    }

    ReturnStarbaseInformation->Name=ServerStringSplit.At(2);
    ReturnStarbaseInformation->MapData=ServerStringSplit.At(3);
    ReturnStarbaseInformation->Owner=ServerStringSplit.At(4);
    ReturnStarbaseInformation->UniqueID=ServerStringSplit.At(5);

    return ReturnStarbaseInformation;
}

/// Get account information
PlayerObject * GameEconomicGameClientStateProgress::ConvertServerStringtoPlayer(String ServerString)
{
    /// Change convert
    String TempString=ServerString;
    Vector <String> ServerStringSplit;

    /// Create a pointer
    PlayerObject * ReturnPlayerInformation  = new PlayerObject();

    /// Replace | to a space
    TempString.Replace ('|', ' ', false);

    /// Split the string
    ServerStringSplit = TempString.Split(' ');

    if(ServerStringSplit.At(1)==String("0"))
    {
        return NULL;
    }

    /// Copy Information
    ReturnPlayerInformation->Firstname=ServerStringSplit.At(2);
    ReturnPlayerInformation->Middlename=ServerStringSplit.At(3);
    ReturnPlayerInformation->Lastname=ServerStringSplit.At(4);
    ReturnPlayerInformation->Level=atoi(ServerStringSplit.At(5).CString());
    ReturnPlayerInformation->Experience=atoi(ServerStringSplit.At(6).CString());
    ReturnPlayerInformation->Reputation=atoi(ServerStringSplit.At(7).CString());
    ReturnPlayerInformation->Reputation1=atoi(ServerStringSplit.At(8).CString());
    ReturnPlayerInformation->Reputation2=atoi(ServerStringSplit.At(9).CString());
    ReturnPlayerInformation->Reputation3=atoi(ServerStringSplit.At(10).CString());
    ReturnPlayerInformation->Reputation4=atoi(ServerStringSplit.At(11).CString());
    ReturnPlayerInformation->Reputation5=atoi(ServerStringSplit.At(12).CString());
    ReturnPlayerInformation->AlienRace=atoi(ServerStringSplit.At(13).CString());
    ReturnPlayerInformation->AlienAllianceAligned=atoi(ServerStringSplit.At(14).CString());
    ReturnPlayerInformation->Gender=atoi(ServerStringSplit.At(15).CString());
    ReturnPlayerInformation->PersonalityTrait=atoi(ServerStringSplit.At(16).CString());
    ReturnPlayerInformation->GalaxySeed=ServerStringSplit.At(17).CString();
    ReturnPlayerInformation->Credits=atoi(ServerStringSplit.At(18).CString());
    ReturnPlayerInformation->UniqueID=ServerStringSplit.At(19).CString();

    return ReturnPlayerInformation;
}


/// Setup the viewport
void GameEconomicGameClientStateProgress::ProgressScreenViewport(void)
{
    /// Get resources and clear everything
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();
    FileSystem * filesystem = GetSubsystem<FileSystem>();

    /// CLEAR_COLOR
    Existence -> EraseUI();

    /// Create a new scene
    progressScene_ = new Scene(context_);

    /// Add Octree
    progressScene_ ->CreateComponent<Octree>();

    /// Add a camera node
    Node * cameraNode_ = progressScene_ -> CreateChild("Camera");
    cameraNode_->CreateComponent<Camera>();

    /// Setup Current Viewport
    progressViewport_ = new Viewport(context_, progressScene_, cameraNode_->GetComponent<Camera>());
    renderer->SetViewport(0, progressViewport_);

    return;
}





/// Create progress screen UI
void GameEconomicGameClientStateProgress::ProgressScreenUI(void)
{
    /// Get Needed SubSystems
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * progressuiRoot_ = ui_-> GetRoot();

    /// Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

    Sprite* JumpGateSprite = new Sprite(context_);
    JumpGateSprite->SetName("JumpGateSprite");

    /// Get letter box image
    Texture2D* texture2 = cache ->GetResource<Texture2D>("Resources/Textures/JumpGate.png");

    /// Set letter box properties
    JumpGateSprite->SetTexture(texture2); // Set texture
    JumpGateSprite->SetSize(width,height);
    JumpGateSprite->SetAlignment(HA_CENTER, VA_CENTER);
    JumpGateSprite->SetBlendMode(BLEND_ALPHA);

    /// Create letter box image to UIElement
    UIElement * JumpGateUIElement = new UIElement(context_);
    JumpGateUIElement->AddChild(JumpGateSprite);

    /// Add letter box UIElement to ui
    progressuiRoot_->AddChild(JumpGateUIElement);

    /// Set style of UIElements
    JumpGateUIElement->SetOpacity(.4);

    JumpGateSprite->SetStyleAuto();
    JumpGateUIElement->SetStyleAuto();


    Sprite* LetterBoxSprite = new Sprite(context_);
    LetterBoxSprite->SetName("LetterBoxSprite");

    /// Get letter box image
    Texture2D* texture3 = cache ->GetResource<Texture2D>("Resources/Textures/LetterBox.png");

    /// Set letter box properties
    LetterBoxSprite->SetTexture(texture3); // Set texture
    LetterBoxSprite->SetSize(width,height);
    LetterBoxSprite->SetAlignment(HA_CENTER, VA_CENTER);
    LetterBoxSprite->SetBlendMode(BLEND_ALPHA);

    /// Create letter box image to UIElement
    UIElement * LetterBoxUIElement = new UIElement(context_);
    LetterBoxUIElement->AddChild(LetterBoxSprite);

    /// Add letter box UIElement to ui
    progressuiRoot_->AddChild(LetterBoxUIElement);

    /// Set style of UIElements
    LetterBoxUIElement->SetOpacity(.4);

    LetterBoxSprite->SetStyleAuto();
    LetterBoxUIElement->SetStyleAuto();

    UIElement* progressBarUIElement = new UIElement(context_);
    Text* progressText=new Text(context_);

    /// Create Window 'titlebar' container
    progressBarUIElement ->SetMinSize(0,32);
    progressBarUIElement ->SetPosition(16,0);
    progressBarUIElement ->SetVerticalAlignment(VA_BOTTOM);
    progressBarUIElement ->SetHorizontalAlignment(HA_LEFT);
    progressBarUIElement ->SetLayoutMode(LM_FREE);

    progressText->SetName("ProgressText");
    progressText->SetText("Accessing ..... ");

    /// addlones
    /// Add the controls to the title bar
    progressBarUIElement->AddChild(progressText);

    progressuiRoot_ -> AddChild(progressBarUIElement);

    /// Apply styles
    progressBarUIElement->SetStyleAuto();
    progressText->SetStyleAuto();

    return;
}

/// convert map to grid
void GameEconomicGameClientStateProgress::GenerateMapDataToGameMap(string MapData)
{
    /// all necessary string
    std::size_t FindFirstCluster;
    std::size_t FindFirstNonCluster;
    std::size_t FindEnd;

    bool decluster = true;

    /// find first cluster
    FindFirstCluster= MapData.find("!");
    FindFirstNonCluster= MapData.find("#");

    /// random timer
    srand (time(NULL));

    /// primary loop going through clusters
    do
    {
        /// test first cluser
        if(FindFirstCluster!=string::npos)
        {
            /// find the cluster end
            FindEnd=MapData.find("!",FindFirstCluster+1);

            /// test if another cluster was find but not
            if(FindEnd==string::npos)
            {
                /// copy substring
                string NewString = MapData.substr(FindFirstCluster+1,(unsigned int)FindFirstNonCluster-(FindFirstCluster+1));

                GenerateMapDataConvertIntoGameObject(MapData_Clustered, NewString);

                break;
            }
            else
            {
                /// copy substring
                string NewString = MapData.substr(FindFirstCluster+1,(unsigned int)FindEnd-(FindFirstCluster+1));

                GenerateMapDataConvertIntoGameObject(MapData_Clustered, NewString);

                /// find a new cluster
                FindFirstCluster = MapData.find("!",FindFirstCluster+1);
            }
        }
    }
    while(FindFirstCluster<=FindFirstNonCluster);

    /// Find all non clustered
    do
    {
        /// test first cluser
        if(FindFirstNonCluster!=string::npos)
        {
            /// find the cluster end
            FindEnd=MapData.find("#",FindFirstNonCluster+1);

            /// test if another cluster was find but not
            if(FindEnd==string::npos)
            {
                /// copy substring
                string NewString = MapData.substr(FindFirstNonCluster+1,(unsigned int)MapData.size()-(FindFirstNonCluster+1));

                GenerateMapDataConvertIntoGameObject(MapData_NonClustered, NewString);

                break;
            }
            else
            {
                /// copy substring
                string NewString = MapData.substr(FindFirstNonCluster+1,(unsigned int)FindEnd-(FindFirstNonCluster+1));

                /// find a new cluster
                FindFirstNonCluster = MapData.find("#",FindFirstNonCluster+1);

                GenerateMapDataConvertIntoGameObject(MapData_NonClustered, NewString);
            }
        }
    }

    while(FindFirstNonCluster<=MapData.size());

    return;
}

/// get values
void GameEconomicGameClientStateProgress::GenerateMapDataConvertIntoGameObject(MapDataType inputtype, string InputString)
{
    /// Get resources and clear everything
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();
    FileSystem * filesystem = GetSubsystem<FileSystem>();

    /// sub Split the string further
    vector<string> results = Split(InputString,':');

    int x = 0;
    int y = 0;
    char type = 0;
    unsigned int dupx = 0;
    unsigned int dupy = 0;

    /// Get the type first - Common
    type = (char) results.at(2).at(0);
    x = atoi(results.at(0).c_str());
    y = atoi(results.at(1).c_str());

    /// Convert type into a resource value
    unsigned int resourcevalue=(type-'$')+1;

    MapDataElementType ResourceType = MapDataElement_Unknown;

    /// change resourcetype based on resourcevalue
    if(resourcevalue==1)
    {
        ResourceType = MapDataElement_StructureFloor;
    }
    else if(resourcevalue>1&&resourcevalue<19)
    {
        ResourceType = MapDataElement_StructureWall;
    }
    else if(resourcevalue==19)
    {
        ResourceType = MapDataElement_StructureCeiling;
    }
    else if(resourcevalue==28)
    {
        ResourceType = MapDataElement_ElementCeilingLight;
    }
    else if(resourcevalue==22)
    {
        ResourceType = MapDataElement_ForceFieldBayDoor;
    }


    /// convert and loop if clustered
    if(inputtype==MapData_Clustered)
    {
        dupx = strtoul(results.at(3).c_str(),NULL,0);
        dupy = strtoul(results.at(4).c_str(),NULL,0);
    }

    /// Get integer of lookup
    String ResourceNamed=String("GenericType");
    ResourceNamed.Append(String(resourcevalue));

    /// Get Info from DB
    TranslationTableResourceInformation LookupInfo= Existence->ResourcesManager->GetResourceSymbolLookup(ResourceNamed);

    if(LookupInfo.ResourceID==-1)
    {
        LookupInfo.ResourceType=RCType_None;
    }


    /// generate generalfile
    String temporaryfilename;
    String temporarycellfilename;

    temporaryfilename.Append(filesystem->GetProgramDir().CString());

    /// if touch enabled
    if(Existence->touchenabled_==false)
    {
        temporaryfilename.Append("Resources/Models/");
    }
    else
    {
        temporaryfilename.Append("Resources/Textures2D/");
    }

    temporaryfilename.Append("Generic_Type");
    temporarycellfilename=temporaryfilename;

    /// Create a resource filename
    temporaryfilename.Append(String(resourcevalue));
    temporarycellfilename.Append(String(19));

    /// Create needed filenames
    String temporarymodelfilename;
    String temporarytexturefilename;

    String temporarycellmodelfilename;
    String temporarycelltexturefilename;

    if(Existence->touchenabled_==false)
    {
        /// wall or other component
        temporarymodelfilename=temporaryfilename+String(".mdl");
        temporarytexturefilename=temporaryfilename+String(".txt");

        /// cell filename
        temporarycellmodelfilename=temporarycellfilename+String(".mdl");
        temporarycelltexturefilename=temporarycellfilename+String(".txt");
    }
    else
    {
        temporarymodelfilename=temporaryfilename+String(".mdl");
        temporarytexturefilename=temporaryfilename+String(".png");
    }

    /// Node for object model
    Node * ObjectStaticNode;
    StaticModel * ObjectStaticModel;

    /// Node for object model
    Node * ObjectCellStaticNode;
    StaticModel * ObjectCellStaticModel;

    /// Additional node for 2D
    Node * ObjectSpriteNode;
    Texture2D * ObjectSpriteTexture;
    StaticSprite2D * ObjectSpriteComponent;
    Sprite2D * ObjectSprite;

    /// Load StarbaseNode
    Node * StarbaseNode = Existence->scene_->GetChild("StarBaseNode", true);

    /// Copy node to this componet
    Starbase * StarbaseNodeComponent = StarbaseNode->GetComponent<Starbase>();

    /// set zposition
    float zposition=0.0f;

    if(LookupInfo.ResourceType==RCType_Light)
    {
        zposition = strtof(results.at(3).c_str(),NULL);
    }

    if(ResourceType == MapDataElement_StructureCeiling)
    {
        zposition=4.0f;
    }

    /// If clustered
    if(inputtype==MapData_Clustered)
    {
        for(unsigned int i=0; i<dupy; i++)
        {
            for(unsigned int j=0; j<dupx; j++)
            {
                if(Existence->touchenabled_==false)
                {
                    GenerateSceneNode3D(StarbaseNode ,(float)x+(j*2),zposition,(float)y+(i*2),LookupInfo.ResourceString, LookupInfo.ResourceType, false, temporarymodelfilename, temporarytexturefilename,false);
                }
                else
                {
                    /// Load texture
                    ObjectSpriteTexture = new Texture2D(context_);
                    ObjectSpriteTexture = cache->GetResource<Texture2D>(temporarytexturefilename);

                    ObjectSprite = new Sprite2D(context_);
                    ObjectSprite ->	SetTexture (ObjectSpriteTexture);
                    ObjectSprite -> SetRectangle(IntRect(0.0f,0.0f,32.0f,32.0f));

                    ObjectSpriteNode =  StarbaseNode->CreateChild("Generic_Type");

                    ObjectSpriteComponent = ObjectSpriteNode-> CreateComponent<StaticSprite2D>();
                    ObjectSpriteComponent ->SetSprite(ObjectSprite);
                    ObjectSpriteComponent ->SetBlendMode(BLEND_ALPHA);
                    ObjectSpriteComponent ->SetLayer(0);

                    ObjectSpriteNode->SetPosition(Vector3(((float)x+(j*2))*.16,((float)y+(i*2))*.16,0.0f));

                    /// Add a resource node
                    ObjectSpriteNode->CreateComponent<ResourceNodeComponent>();

                    StarbaseNodeComponent -> PushNode(ObjectSpriteNode, LookupInfo.ResourceType);
                }
            }
        }
    }

    /// If non-clustered
    if(inputtype==MapData_NonClustered)
    {
        if(Existence->touchenabled_==false)
        {
            GenerateSceneNode3D(StarbaseNode ,(float)x, zposition, (float)y,LookupInfo.ResourceString, LookupInfo.ResourceType, false, temporarymodelfilename, temporarytexturefilename,false);
        }
        else
        {
            /// Load texture
            ObjectSpriteTexture = new Texture2D(context_);
            ObjectSpriteTexture = cache->GetResource<Texture2D>(temporarytexturefilename);

            ObjectSprite = new Sprite2D(context_);
            ObjectSprite -> SetTexture (ObjectSpriteTexture);
            ObjectSprite ->SetRectangle(IntRect(0.0f,0.0f,32.0f,32.0f));

            ObjectSpriteNode =  StarbaseNode->CreateChild("Generic_Type");

            ObjectSpriteComponent = ObjectSpriteNode-> CreateComponent<StaticSprite2D>();
            ObjectSpriteComponent ->SetSprite(ObjectSprite);
            ObjectSpriteComponent ->SetBlendMode(BLEND_ALPHA);
            ObjectSpriteComponent ->SetLayer(0);

            ObjectSpriteNode->SetPosition(Vector3(x*.16,y*.16,0.0f));

            /// Add a resource node
            ObjectSpriteNode->CreateComponent<ResourceNodeComponent>();

            /// Add to starbase if it is a power node
            if(LookupInfo.ResourceType==RCType_Light)
            {
                StarbaseNodeComponent -> PushNode(ObjectSpriteNode, LookupInfo.ResourceType);
            }
            /// Add to starbase if it is a power node
            if(LookupInfo.ResourceType==RCType_Forcefield)
            {
                StarbaseNodeComponent -> PushNode(ObjectSpriteNode, LookupInfo.ResourceType);
            }
        }
    }

    /// if touch enabled add additional elements
    if(Existence->touchenabled_==false)
    {
        /// if node was a light
        /*   if(LookupInfo.ResourceType==RCType_Light)
           {
               float zposition = strtof(results.at(3).c_str(),NULL);

               ObjectStaticNode->SetPosition(Vector3(x,zposition,y));

               /// Create a directional light to the world. Enable cascaded shadows on it
               Node * LightNode= ObjectStaticNode -> CreateChild("Generic_Light");
               LightNode->SetDirection(Vector3(0.0f, 0.0f, 0.0f));
               Light* light = LightNode ->CreateComponent<Light>();
               light->SetLightType(LIGHT_POINT);
               light->SetCastShadows(true);
               light->SetSpecularIntensity(2.0f);
               light->SetBrightness(0.0f);
               light->SetColor(Color(0.91f, 0.9f,0.9f));
               light->SetFov(30);
               light->SetRange(10);

               /// Move light on light node
               LightNode->SetPosition(Vector3(0,-1,0));
               LightNode->SetName("Generic_Light");

               /// Create a resource componet
               ResourceNodeComponent * ObjectResourceComponent = ObjectStaticNode->GetComponent<ResourceNodeComponent>();
               PowerComponent * ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();

               /// Set resource type to light
               ObjectResourceComponent->SetResourceComponentType(RCType_Light);
               ObjectResourcePower->Initialize();
           }

           /// if node was a light
           if(LookupInfo.ResourceType==RCType_Forcefield)
           {
               ResourceNodeComponent * ObjectResourceComponent = ObjectStaticNode->GetComponent<ResourceNodeComponent>();
               PowerComponent * ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();

               /// Set resource type to light
               ObjectResourceComponent->SetResourceComponentType(RCType_Forcefield);
               ObjectResourcePower->Initialize();
           }
        */

    }

    return;
}


/// Split the string up
vector<string> GameEconomicGameClientStateProgress::Split(const string &s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim))
    {
        tokens.push_back(item);
    }
    return tokens;
}

bool GameEconomicGameClientStateProgress::loadScene(void)
{
    /// get resources
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();
    FileSystem * filesystem = GetSubsystem<FileSystem>();

    Existence->scene_->Clear();


    /// Create Scene components
    Existence->scene_-> CreateComponent<Octree>();

    Existence->scene_-> CreateComponent<DebugRenderer>();

    if(Existence->touchenabled_)
    {
        Existence->scene_-> CreateComponent<Renderer2D>();
    }
    else
    {
        Existence->scene_-> CreateComponent<PhysicsWorld>();
    }
    \
    /// Position character
    Node * characternode_ = Existence->scene_->CreateChild("Character");
    characternode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

    /// Get the Camera Node and setup the viewport - create
    if(!(Existence->cameraNode_ = Existence->scene_->GetChild("Camera")))
    {
        /// Create a scene node for the camera, which we will move around
        /// The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
        Existence->cameraNode_ = new Node(context_);

        Existence->cameraNode_ = Existence->scene_->CreateChild("Camera");
        Existence->cameraNode_->CreateComponent<Camera>();

        Camera* camera = Existence->cameraNode_->CreateComponent<Camera>();
        camera->SetFarClip(1000.0f);

        ///Set an initial position for the camera scene node above the ground
        Existence->cameraNode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        Existence->cameraNode_->SetRotation(Quaternion(0.0f, 0.0f, 0.0f));

    }

    /// Add Starbase node
    Node * StarbaseNode = new Node(context_);
    Existence->scene_->AddChild(StarbaseNode);

    /// Copy node to this componet
    Starbase * StarbaseNodeComponent = StarbaseNode->CreateComponent<Starbase>();


    Existence->ThisStarbase->ThisComponent = StarbaseNodeComponent;

    /// Save this starbase to this component
    StarbaseNode->CreateComponent<Starbase>();

    /// Set Position
    StarbaseNode -> SetName("StarBaseNode");
    StarbaseNode -> SetPosition(Vector3(0.0f,0.0f,0.0f));
    StarbaseNode -> SetRotation(Quaternion(0.0f,0.0f,0.0f));

    /// Create map
    string Map;

    /// Add ResourceComponents Base Structure
    Map.append("!-6:-4:$:3:4!2:-4:$:3:4!-6:-6:(:7:1!-6:4:&:7:1!-8:-4:':1:4!8:-2:2:1:2!-6:-4:6:3:4!2:-4:6:3:4");
    Map.append("#0:-2:6#0:0:6#0:-2:$#0:0:$#0:-4:-#0:2:/#8:-4:%#8:2:%#4:0:?:3.95#4:-2:?:3.95#-4:0:?:3.95#-4:-2:?:3.95#8:2:7#8:-4:8#8:-2:9");

    /// Generate Map
    GenerateMapDataToGameMap(Map);

    Node * EmptyNode = Existence->scene_->CreateChild("EmptyNode");
    EmptyNode->SetPosition(Vector3(2.0f,2.0f,2.0f));
    EmptyNode->SetName("EmptyNode");

    /// Add extra temporary this way
    if(Existence->touchenabled_==false)
    {
        /// generate generalfile
        String temporaryfilename;

        /// Create needed filenames
        String temporarymodelfilename;
        String temporarytexturefilename;
        Node * ObjectStaticNode;
        StaticModel * ObjectStaticModel;
        /// first one
        temporaryfilename.Clear();

        temporaryfilename.Append("Resources/Models/");

        /// Create a resource filename
        temporaryfilename.Append("ExoComp2");

        /// wall or other component
        temporarymodelfilename=temporaryfilename+String(".mdl");
        temporarytexturefilename=temporaryfilename+String(".txt");

        ObjectStaticNode= Existence->scene_ -> CreateChild("ExoComp2");

        ObjectStaticModel = ObjectStaticNode->CreateComponent<StaticModel>();

        ObjectStaticModel ->SetModel(cache->GetResource<Model>(temporarymodelfilename));
        ObjectStaticModel ->ApplyMaterialList(temporarytexturefilename);

        ObjectStaticModel ->SetCastShadows(true);

        ObjectStaticNode->SetPosition(Vector3(4,1.5f,1));

        /// Create rigidbody, and set non-zero mass so that the body becomes dynamic
        RigidBody * ObjectRigidBody = ObjectStaticNode->CreateComponent<RigidBody>();
        ObjectRigidBody->SetCollisionLayer(1);

        /// Turn off Gravity
        ObjectRigidBody->SetMass(1.2f);
        ObjectRigidBody->SetUseGravity(true);
        ObjectRigidBody->SetAngularDamping(0.6f);
        ObjectRigidBody->SetLinearDamping(0.6f);

        /// Set zero angular factor so that physics doesn't turn the character on its own.
        /// Instead we will control the character yaw manually
        ObjectRigidBody->SetAngularFactor(Vector3::ONE);

        /// Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
        ObjectRigidBody->SetCollisionEventMode(COLLISION_ALWAYS);

        Model * ExoComp2Collision = cache->GetResource<Model>(String("Resources/Models/ExoComp2Collision.mdl"));

        /// Set a capsule shape for collision
        CollisionShape* ObjectShape = ObjectStaticNode->CreateComponent<CollisionShape>();

        /// Set shape collision
        ObjectShape->SetConvexHull(ExoComp2Collision);
        ObjectShape->SetLodLevel(1);
        ObjectShape->SetMargin(0.1f);

        /// Created Drone
        Drone * CreatedDrone = ObjectStaticNode->CreateComponent<Drone>();

        /// Create Drone information
        DroneInformation SetDrone;
        SetDrone.AlienAllianceAligned=0;
        SetDrone.AlienRace=0;
        SetDrone.DroneType=Drone100Beta;
        SetDrone.Name = String("test");

        CreatedDrone->SetNode(ObjectStaticNode);
        CreatedDrone->SetParameters(SetDrone);
        CreatedDrone->Initialize();

        ResourceNodeComponent * DroneResourceComponent2=ObjectStaticNode -> CreateComponent<ResourceNodeComponent>();
        DroneResourceComponent2->SetResourceComponentNameType(String("ExoComp1"), RCType_Drone);
        DroneResourceComponent2->MapResources(Existence->ResourcesManager);

        ObjectStaticNode->CreateComponent<InteractObject>();
        /*
                /// first one
                temporaryfilename.Clear();

                temporaryfilename.Append("Resources/Models/");

                /// Create a resource filename
                temporaryfilename.Append("ExoComp1");

                /// wall or other component
                temporarymodelfilename=temporaryfilename+String(".mdl");
                temporarytexturefilename=temporaryfilename+String(".txt");

                ObjectStaticNode= Existence->scene_ -> CreateChild("ExoComp1");

                ObjectStaticModel = ObjectStaticNode->CreateComponent<StaticModel>();

                ObjectStaticModel ->SetModel(cache->GetResource<Model>(temporarymodelfilename));
                ObjectStaticModel ->ApplyMaterialList(temporarytexturefilename);

                ObjectStaticModel ->SetCastShadows(true);

                ObjectStaticNode->SetPosition(Vector3(4,2.0f,-1));

                /// Create rigidbody, and set non-zero mass so that the body becomes dynamic
                ObjectRigidBody = ObjectStaticNode->CreateComponent<RigidBody>();
                ObjectRigidBody->SetCollisionLayer(1);

                /// Turn off Gravity
                ObjectRigidBody->SetMass(1.0f);
                ObjectRigidBody->SetUseGravity(true);

                /// Set zero angular factor so that physics doesn't turn the character on its own.
                /// Instead we will control the character yaw manually
                ObjectRigidBody->SetAngularFactor(Vector3::ONE);

                /// Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
                ObjectRigidBody->SetCollisionEventMode(COLLISION_ALWAYS);

                /// Get static model and bounding box, calculate offset
                ReferenceModel=ObjectStaticModel->GetModel();

                /// Set a capsule shape for collision
                ObjectShape = ObjectStaticNode->CreateComponent<CollisionShape>();

                /// Set shape collision
                ObjectShape->SetBox(Vector3::ONE);
                ObjectShape->SetLodLevel(1);

                CreatedDrone = ObjectStaticNode->CreateComponent<Drone>();

                /// Create Drone information
                SetDrone.AlienAllianceAligned=0;
                SetDrone.AlienRace=0;
                SetDrone.DroneType=Drone200Alpha;
                SetDrone.Name = String("test");

                CreatedDrone->SetParameters(SetDrone);

                ResourceNodeComponent * DroneResourceComponent=ObjectStaticNode -> CreateComponent<ResourceNodeComponent>();
                DroneResourceComponent->SetResourceComponentNameType(String("ExoComp1"), RCType_Drone);
                DroneResourceComponent->MapResources(Existence->ResourcesManager);

                ObjectStaticNode->CreateComponent<InteractObject>();*/

        /// first one
        temporaryfilename.Clear();

        temporaryfilename.Append("Resources/Models/");

        /// Create a resource filename
        temporaryfilename.Append("GenericReplicationPrinter1");

        /// wall or other component
        temporarymodelfilename=temporaryfilename+String(".mdl");
        temporarytexturefilename=temporaryfilename+String(".txt");

        ObjectStaticNode= Existence->scene_ -> CreateChild("GenericReplicationPrinter1");

        ///ObjectStaticNode->SetName(String("ReplicationPrinter1"));

        ObjectStaticModel = ObjectStaticNode->CreateComponent<StaticModel>();

        ObjectStaticModel ->SetModel(cache->GetResource<Model>(temporarymodelfilename));
        ObjectStaticModel ->ApplyMaterialList(temporarytexturefilename);

        ObjectStaticModel ->SetCastShadows(true);

        ObjectStaticNode->SetPosition(Vector3(2.0f,1.0f,-4.0f));
        ObjectStaticNode->Yaw(90);

        ObjectRigidBody = ObjectStaticNode->CreateComponent<RigidBody>();
        ObjectRigidBody->SetCollisionLayer(1);

        /// Turn off Gravity
        ObjectRigidBody->SetMass(0.0f);
        ObjectRigidBody->SetUseGravity(false);

        /// Set zero angular factor so that physics doesn't turn the character on its own.
        /// Instead we will control the character yaw manually
        ObjectRigidBody->SetAngularFactor(Vector3::ZERO);

        /// Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
        ObjectRigidBody->SetCollisionEventMode(COLLISION_ALWAYS);

        /// Get static model and bounding box, calculate offset
        Model * ReferenceModel=ObjectStaticModel->GetModel();

        /// Set a capsule shape for collision
        ObjectShape = ObjectStaticNode->CreateComponent<CollisionShape>();

        /// Set shape collision
        ObjectShape->SetConvexHull(ReferenceModel,1,Vector3::ONE);
        ObjectShape->SetLodLevel(1);

        /// Create a directional light to the world. Enable cascaded shadows on it
        Node* PrinterlightNode = ObjectStaticNode->CreateChild("Generic_Light");
        PrinterlightNode->SetDirection(Vector3(0.0f, 0.0f, 0.0f));
        Light* Printerlight = PrinterlightNode->CreateComponent<Light>();
        Printerlight->SetLightType(LIGHT_POINT);
        Printerlight->SetCastShadows(true);
        Printerlight->SetSpecularIntensity(0.0f);
        Printerlight->SetBrightness(0.0f);
        Printerlight->SetColor(Color(1.0f,1.0f,0.0f));
        Printerlight->SetFov(30);
        Printerlight->SetRange(3);

        PrinterlightNode->SetPosition(Vector3(0.0f,.5f,0.0f));
        PrinterlightNode->SetName("Generic_Light");

        /// create components
        StarbaseNodeComponent->PushNode(ObjectStaticNode, RCType_ReplicationPrinter);

        ResourceNodeComponent * ObjectResourceComponent = ObjectStaticNode->CreateComponent<ResourceNodeComponent>();
        PowerComponent * ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();
        ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();

        ObjectResourceComponent->SetResourceComponentNameType(String("GenericReplicationPrinter1"), RCType_ReplicationPrinter);
        ObjectResourcePower->Initialize();

        ObjectStaticNode->CreateComponent<InteractObject>();

        ObjectRigidBody = ObjectStaticNode->CreateComponent<RigidBody>();
        ObjectRigidBody->SetCollisionLayer(1);

        /// Turn off Gravity
        ObjectRigidBody->SetMass(0.0f);
        ObjectRigidBody->SetUseGravity(false);

        /// Set zero angular factor so that physics doesn't turn the character on its own.
        /// Instead we will control the character yaw manually
        ObjectRigidBody->SetAngularFactor(Vector3::ZERO);

        /// Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
        ObjectRigidBody->SetCollisionEventMode(COLLISION_ALWAYS);

        /// Get static model and bounding box, calculate offset
        ReferenceModel=ObjectStaticModel->GetModel();

        /// Set a capsule shape for collision
        ObjectShape = ObjectStaticNode->CreateComponent<CollisionShape>();

        /// Set shape collision
        ObjectShape->SetConvexHull(ReferenceModel,1,Vector3::ONE);
        ObjectShape->SetLodLevel(1);


        ObjectResourceComponent->SetResourceComponentNameType(String("GenericReplicationPrinter1"), RCType_ReplicationPrinter);
        ObjectResourceComponent->MapResources(Existence->ResourcesManager);

        /// first one
        temporaryfilename.Clear();

        temporaryfilename.Append("Resources/Models/");

        /// Create a resource filename
        temporaryfilename.Append("GenericPowerGenerator1");

        /// wall or other component
        temporarymodelfilename=temporaryfilename+String(".mdl");
        temporarytexturefilename=temporaryfilename+String(".txt");

        ObjectStaticNode= Existence->scene_ -> CreateChild("GenericPowerGenerator1");

        ObjectStaticModel = ObjectStaticNode->CreateComponent<StaticModel>();

        ObjectStaticModel ->SetModel(cache->GetResource<Model>(temporarymodelfilename));
        ObjectStaticModel ->ApplyMaterialList(temporarytexturefilename);

        ObjectStaticModel ->SetCastShadows(true);

        ObjectStaticNode->SetPosition(Vector3(5.0f,0.5f,-4.0f));
        ObjectStaticNode->Yaw(180);

        ObjectRigidBody = ObjectStaticNode->CreateComponent<RigidBody>();
        ObjectRigidBody->SetCollisionLayer(1);

        /// Turn off Gravity
        ObjectRigidBody->SetMass(0.0f);
        ObjectRigidBody->SetUseGravity(false);

        /// Set zero angular factor so that physics doesn't turn the character on its own.
        /// Instead we will control the character yaw manually
        ObjectRigidBody->SetAngularFactor(Vector3::ONE);

        /// Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
        ObjectRigidBody->SetCollisionEventMode(COLLISION_ALWAYS);

        /// Get static model and bounding box, calculate offset
        ReferenceModel=ObjectStaticModel->GetModel();

        /// Set a capsule shape for collision
        ObjectShape = ObjectStaticNode->CreateComponent<CollisionShape>();

        /// Set shape collision
        ObjectShape->SetConvexHull(ReferenceModel,1,Vector3::ONE);
        ObjectShape->SetLodLevel(1);

        /// Create a directional light to the world. Enable cascaded shadows on it
        Node* GeneratorlightNode = ObjectStaticNode->CreateChild("Generic_Light");
        GeneratorlightNode->SetDirection(Vector3(0.0f, 0.0f, 0.0f));
        Light* Generatorlight = GeneratorlightNode->CreateComponent<Light>();
        Generatorlight->SetLightType(LIGHT_POINT);
        Generatorlight->SetCastShadows(true);
        Generatorlight->SetSpecularIntensity(0.1f);
        Generatorlight->SetBrightness(0);
        Generatorlight->SetColor(Color(0.3f, 0.3f,0.9f));
        Generatorlight->SetFov(30);
        Generatorlight->SetRange(2);

        GeneratorlightNode->SetPosition(Vector3(0.0f,.53f,0.0f));
        GeneratorlightNode->SetName("Generic_Light");

        /// create components
        StarbaseNodeComponent->PushNode(ObjectStaticNode, RCType_PowerSource);

        ObjectResourceComponent = ObjectStaticNode->CreateComponent<ResourceNodeComponent>();
        ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();

        ObjectResourceComponent->SetResourceComponentType(RCType_PowerSource);
        ObjectResourcePower->Initialize();
        ObjectResourcePower->SetPower(14500);

        ObjectResourceComponent->SetResourceComponentNameType(String("GenericPowerGenerator1"), RCType_PowerSource);
        ObjectResourceComponent->MapResources(Existence->ResourcesManager);


        /// first one
        temporaryfilename.Clear();

        temporaryfilename.Append("Resources/Models/");

        /// Create a resource filename
        temporaryfilename.Append("GenericRefrigerationUnit1");

        /// wall or other component
        temporarymodelfilename=temporaryfilename+String(".mdl");
        temporarytexturefilename=temporaryfilename+String(".txt");

        ObjectStaticNode= Existence->scene_ -> CreateChild("GenericRefrigerationUnit1");

        ObjectStaticModel = ObjectStaticNode->CreateComponent<StaticModel>();

        ObjectStaticModel ->SetModel(cache->GetResource<Model>(temporarymodelfilename));
        ObjectStaticModel ->ApplyMaterialList(temporarytexturefilename);

        ObjectStaticModel ->SetCastShadows(true);

        ObjectStaticNode->SetPosition(Vector3(-3.0f,1.0f,-4.0f));
        ObjectStaticNode->Yaw(180);


        ObjectRigidBody = ObjectStaticNode->CreateComponent<RigidBody>();
        ObjectRigidBody->SetCollisionLayer(1);

        /// Turn off Gravity
        ObjectRigidBody->SetMass(0.0f);
        ObjectRigidBody->SetUseGravity(false);

        /// Set zero angular factor so that physics doesn't turn the character on its own.
        /// Instead we will control the character yaw manually
        ObjectRigidBody->SetAngularFactor(Vector3::ZERO);

        /// Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
        ObjectRigidBody->SetCollisionEventMode(COLLISION_ALWAYS);

        /// Get static model and bounding box, calculate offset
        ReferenceModel=ObjectStaticModel->GetModel();

        /// Set a capsule shape for collision
        ObjectShape = ObjectStaticNode->CreateComponent<CollisionShape>();

        /// Set shape collision
        ObjectShape->SetConvexHull(ReferenceModel,1,Vector3::ONE);
        ObjectShape->SetLodLevel(1);


        /// create components
        StarbaseNodeComponent->PushNode(ObjectStaticNode, RCType_RefrigerationUnit);

        ObjectResourceComponent = ObjectStaticNode->CreateComponent<ResourceNodeComponent>();
        ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();
        ObjectStaticNode->CreateComponent<InteractObject>();

        ObjectResourceComponent->SetResourceComponentNameType(String("GenericRefrigerationUnit1"), RCType_RefrigerationUnit);
        ObjectResourcePower->Initialize();

        ObjectResourceComponent->MapResources(Existence->ResourcesManager);


    }

    return 1;
}

/// Create a node
void GameEconomicGameClientStateProgress::GenerateSceneNode3D(Node * BaseNode,const float & x, const float &z, const float & y,const String & TempResourceString, const ResourceComponentType &TempResourceType,  const  bool IsSubComponent,  const   String &TempModelFilename, const  String & TempTextureFilename,  bool  IsClustered=false)
{
    /// Get resources and clear everything
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();
    FileSystem * filesystem = GetSubsystem<FileSystem>();

    /// Load StarbaseNode
    Node * StarbaseNode = Existence->scene_->GetChild("StarBaseNode", true);

    /// Copy node to this componet
    Starbase * StarbaseNodeComponent = StarbaseNode->GetComponent<Starbase>();

    /// Add node type
    string * UniqueID = UniqueGenerator::GenerateUniqueID(4);

    String UniqueIDString=String("Generic_Type:")+String(UniqueID->c_str());

    Node * ObjectStaticNode= BaseNode -> CreateChild(UniqueIDString);


    unsigned int i=0;

    StaticModel * ObjectStaticModel = ObjectStaticNode->CreateComponent<StaticModel>();

    /// Use filename information
    ObjectStaticModel ->SetModel(cache->GetResource<Model>(TempModelFilename));
    ObjectStaticModel ->ApplyMaterialList(TempTextureFilename);

    ObjectStaticModel ->SetCastShadows(true);

    ObjectStaticNode->SetPosition(Vector3((float)x,z,(float)y));

    /// Add a resource node
    ResourceNodeComponent * ResourceSet = ObjectStaticNode->CreateComponent<ResourceNodeComponent>();
    ResourceSet -> SetResourceComponentNameType(TempResourceString, TempResourceType);

    /// If nod is not a subcomponent
    if(IsSubComponent==false)
    {
        /// If the element is a structural element
        if(TempResourceType==RCType_Structural||TempResourceType==RCType_Forcefield)
        {
            /// Create rigidbody, and set non-zero mass so that the body becomes dynamic
            RigidBody* ObjectRigidBody = ObjectStaticNode->CreateComponent<RigidBody>();
            ObjectRigidBody->SetCollisionLayer(1);

            /// Turn off Gravity
            ObjectRigidBody->SetMass(0.0f);
            ObjectRigidBody->SetUseGravity(false);

            /// Set zero angular factor so that physics doesn't turn the character on its own.
            /// Instead we will control the character yaw manually
            ObjectRigidBody->SetAngularFactor(Vector3::ZERO);

            /// Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
            ObjectRigidBody->SetCollisionEventMode(COLLISION_ALWAYS);

            /// Get static model and bounding box, calculate offset
            Model * ReferenceModel=ObjectStaticModel->GetModel();

            /// Set a capsule shape for collision
            CollisionShape* ObjectShape = ObjectStaticNode->CreateComponent<CollisionShape>();

            /// Set shape collision
            ObjectShape->SetConvexHull(ReferenceModel,1,Vector3::ONE);
            ObjectShape->SetLodLevel(1);
        }

        /// If the node component exist - If poart of the starbase
        if(StarbaseNodeComponent)
        {
            /// Add to starbase if it is a power node
            if(TempResourceType==RCType_Light)
            {
                StarbaseNodeComponent -> PushNode(ObjectStaticNode, TempResourceType);
            }
            /// Add to starbase if it is a power node
            if(TempResourceType==RCType_Forcefield)
            {
                StarbaseNodeComponent -> PushNode(ObjectStaticNode, TempResourceType);
            }
        }

        /// if node was a light
        if(TempResourceType==RCType_Light)
        {
            /// Set Position
            ObjectStaticNode->SetPosition(Vector3(x,z,y));

            /// Create a directional light to the world. Enable cascaded shadows on it
            Node * LightNode= ObjectStaticNode -> CreateChild("Generic_Light");
            LightNode->SetDirection(Vector3(0.0f, 0.0f, 0.0f));
            Light* light = LightNode ->CreateComponent<Light>();
            light->SetLightType(LIGHT_POINT);
            light->SetCastShadows(false);
            light->SetSpecularIntensity(2.0f);
            light->SetBrightness(0.0f);
            light->SetColor(Color(0.91f, 0.9f,0.9f));
            light->SetFov(60);
            light->SetRange(6);
            light->SetFadeDistance(6);
            light->SetShadowResolution(1.0f);

            /// Move light on light node
            LightNode->SetPosition(Vector3(0,-1,0));
            LightNode->SetName("Generic_Light");

            /// Create a resource componet
            ResourceNodeComponent * ObjectResourceComponent = ObjectStaticNode->GetComponent<ResourceNodeComponent>();
            PowerComponent * ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();

            /// Set resource type to light
            ObjectResourceComponent->SetResourceComponentType(RCType_Light);
            ObjectResourcePower->Initialize();

            ObjectStaticNode->CreateComponent<InteractObject>();

        }

        /// if node was a light
        if(TempResourceType==RCType_Forcefield)
        {
            ResourceNodeComponent * ObjectResourceComponent = ObjectStaticNode->GetComponent<ResourceNodeComponent>();
            PowerComponent * ObjectResourcePower  = ObjectStaticNode->CreateComponent<PowerComponent>();

            /// Set resource type to light
            ObjectResourceComponent->SetResourceComponentType(RCType_Forcefield);
            ObjectResourcePower->Initialize();
        }
    }

    /*

    /// Get component information if available works
    unsigned int a = Existence->ResourcesManager->GetResourceSymbolIdxInt(TempResourceString);
    ResourceComponentInformation * ResourceInfo = Existence->ResourcesManager -> GetResourceInfo(a);

    //unsigned int a = Existence->ResourcesManager->GetResourceSymbolIdxInt(TempResourceString);
    //ResourceComponentInformation * ResourceInfo = Existence->ResourcesManager -> GetResourceInfo(a);

    /// If ResourceSet
    if(ResourceInfo&&ResourceSet)
    {
        /// split inot a string
        vector<string> results = Split(ResourceInfo->ComponentResource.CString(),':');

        for(unsigned int i=0;i<results.size();i+2)
        {
            String Name=String(results.at(i).c_str());
            String Quantity=String(results.at(i+1).c_str());

            ResourceNodeInformation NewResource;

            NewResource.Resource.ResourceName = Name;
            NewResource.Resource.Quantity = strtoul(Quantity.CString(),NULL,0);
            NewResource.IntregrationType=RI_Permament;

            ResourceSet->AddNodeResource(NewResource);
        }
    }


    */
    return;
}

