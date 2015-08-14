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


#include "../GameEconomicComponents/GameStateHandlerComponent.h"
#include "../GameEconomicComponents/Accounts.h"

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
#include "../Networking.h"
#include "../Platform.h"

#include "../../Urho3D/Engine/DebugHud.h"

#define DEFAULTSIZE 4096

using namespace std;
using namespace Urho3D;

#include "misc.inl"

// Expands to this example's entry-point
DEFINE_APPLICATION_MAIN(GameEconomicGameClient)

GameEconomicGameClient::GameEconomicGameClient(Context* context) :
    GameEconomicApp(context), uiRoot_(GetSubsystem<UI>()->GetRoot())
{
    /// Register

    GameStateHandlerComponent::RegisterNewSubsystem(context);
    GameStateHandlerComponent::RegisterGameStates(context);

    cout << "Debig: Existence App Existence " << &applicationPtr << endl;

    /// Register states
    cout << "Debug: Existence Client Base Class Constructor context" << &context << " context_"<< &context_ << endl;

    return;
}

GameEconomicGameClient::~GameEconomicGameClient()
{
    cout << "Debug: Existence Client Base Class Deconstructor" << endl;

    return;
}


/// Get initialize information for example platform
void GameEconomicGameClient::Init(Context * context)
{

    /// Get the platform - Set global variable that determines the game
    CurrentPlatform = OSTool::GetOS(GetPlatform());

    return;
}

/// Main program execution code
void GameEconomicGameClient::Start()
{
    /// Initialize
    Init(context_);

    /// Execute base class startup
    GameEconomicApp::Start();

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    FileSystem * filesystem = GetSubsystem<FileSystem>();
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Set aApplication
    gamestatehandlercomponent_->SetApplication(applicationPtr);

    /// Get ui
    UI* ui = GetSubsystem<UI>();

    /// create variables (urho3d)
    String additionresourcePath;

    additionresourcePath.Append(filesystem->GetProgramDir().CString());
    additionresourcePath.Append("Resources/");

    /// add resource path to last
    cache -> AddResourceDir(additionresourcePath);

    /// Turn on networking
    if(LoadNetworkConfig(NetConfig)==false)
    {
        /// Set up network configuration
        NetConfig.hostport = 3632;
        NetConfig.hostserver = String("127.0.0.1");
        NetConfig.hostidentity = Unauthenticated;
    }

    /// Set up default network status
    NetOnline = NetDisconnected;
    NetStats = NetworkOffline;

    /// Set the loaded style as default style
    uiRoot_->SetDefaultStyle(style);

    CreateCursor();

    ui->GetCursor()->SetVisible(true);

    /// Initialize Console
    InitializeConsole();

    GetSubsystem<Input>()->Update();

    /// Enable OS cursor
    /// Configure rudimentary state handler
    gamestatehandlercomponent_ ->SetUIState(UI_NONE);

    GetSubsystem<Input>()->SetMouseVisible(true);

    /// load account
    ///LoadAccount();

    /// Create test value
    testvalue=911;

    /// Debug output context and testvalue
    cout << "Debug: ExistenceClient Class Test Value " << testvalue << " context_ " << &context_ << endl;

    /// Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    /// like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    /// could subscribe in the constructor instead.
    SubscribeToEvents();

    /// Randomize timer
    srand (time(NULL));

    cout << "Debig: Existence App Existence " << applicationPtr ->GetTestString()<< endl;

    /// Create test value
    testvalue=121;

    cout << "Debig: Existence App Existence " << applicationPtr ->GetTestString()<< endl;

    /// Network related
    SubscribeToEvent(E_NETWORKMESSAGE, HANDLER(GameEconomicGameClient, HandleNetworkMessage));
    SubscribeToEvent(E_SERVERCONNECTED, HANDLER(GameEconomicGameClient, HandlerServerConnected));
    SubscribeToEvent(E_SERVERDISCONNECTED, HANDLER(GameEconomicGameClient, HandlerServerDisconnected));
    SubscribeToEvent(E_CONNECTFAILED, HANDLER(GameEconomicGameClient, HandlerServerConnectionFailed));

    gamestatehandlercomponent_->Start();

    return;
}


/// Subscribe to all events
void GameEconomicGameClient::SubscribeToEvents()
{
    /// Subscribe () function for console input
    SubscribeToEvent(E_CONSOLECOMMAND, HANDLER(GameEconomicGameClient, HandleConsoleCommand));

    /// Subscribe () function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(GameEconomicGameClient, HandleUpdate));

    /// Add to Event (Key Input)
    SubscribeToEvent(E_KEYDOWN, HANDLER(GameEconomicGameClient, HandlerFunctionKeyDown));

}

/// Setup the main viewport
void GameEconomicGameClient::SetupScreenViewport()
{
    /// Get Needed SubSystems
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    /// Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();


    renderer -> SetTextureQuality (QUALITY_HIGH);
    renderer ->SetMaterialQuality (QUALITY_HIGH);
    renderer ->SetShadowQuality (SHADOWQUALITY_HIGH_24BIT);

    /// create a new scene
    scene_= new Scene(context_);
    scene_-> CreateComponent<Octree>();
    scene_-> CreateComponent<DebugRenderer>();

    Node* existencelogobackgroundNode = scene_->CreateChild("Plane");
    existencelogobackgroundNode ->SetScale(Vector2(width*.012,height*.012));
    existencelogobackgroundNode ->SetPosition(Vector3(0.0,0.0,-2.93435));
    existencelogobackgroundNode ->SetRotation(Quaternion(0.0,0.0,0.0));
    StaticModel* existencelogobackgroundObject = existencelogobackgroundNode->CreateComponent<StaticModel>();
    existencelogobackgroundObject->SetModel(cache->GetResource<Model>("Resources/Models/existencelogobackground.mdl"));
    existencelogobackgroundObject->SetMaterial(cache->GetResource<Material>("Resources/Materials/existencelogobackground.xml"));

    /// Create a directional light to the world so that we can see something. The light scene node's orientation controls the
    /// light direction; we will use the SetDirection() function which calculates the orientation from a forward direction vector.
    /// The light will use default settings (white light, no shadows)
    Node* lightNode = scene_->CreateChild("DirectionalLight");
    //lightNode->SetDirection(Vector3(0.0,0.9,2)); /// The direction vector does not need to be normalized
    lightNode->SetRotation(Quaternion(0.0,160.0,0.0));
    Light* lightObject = lightNode->CreateComponent<Light>();
    lightObject->SetLightType(LIGHT_DIRECTIONAL);
    lightObject->SetBrightness(.7);
    lightObject->SetSpecularIntensity(0);

    /// Add a component
    Node* lightNode2 = scene_->CreateChild("DirectionalLight");
    //lightNode2->SetDirection(Vector3(0.0,0.9,0.0)); /// The direction vector does not need to be normalized
    lightNode2->SetRotation(Quaternion(0.0,200.0,0.0));
    Light* lightObject2 = lightNode2->CreateComponent<Light>();
    lightObject2->SetLightType(LIGHT_DIRECTIONAL);
    lightObject2->SetBrightness(.4);
    lightObject2->SetSpecularIntensity(0);

    Node* lightNode3 = scene_->CreateChild("DirectionalLight");
    //lightNode3->SetDirection(Vector3(0.0,12,0.0)); /// The direction vector does not need to be normalized
    //lightNode3->SetRotation(Quaternion(0.0,180.0,0.0));
    lightNode3->LookAt(Vector3(0.0f,0.0f,0.0f));
    Light* lightObject3 = lightNode3->CreateComponent<Light>();
    lightObject3->SetLightType(LIGHT_DIRECTIONAL);
    lightObject3->SetBrightness(.1);
    lightObject3->SetSpecularIntensity(1);

    /// Create a scene node for the camera, which we will move around
    /// The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
    cameraNode_ = scene_->CreateChild("Camera");

    /// Set an initial position for the camera scene node above the plane
    cameraNode_->SetPosition(Vector3(0.0,0.0,5.0));
    cameraNode_->SetRotation(Quaternion(0.0,-180.0,0.0));
    Camera* cameraObject = cameraNode_->CreateComponent<Camera>();
    cameraObject->SetOrthographic(1);
    cameraObject->SetZoom(3);

    /// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen. We need to define the scene and the camera
    /// at minimum. Additionally we could configure the viewport screen size and the rendering path (eg. forward / deferred) to
    /// use, but now we just use full screen and default render path configured	SetOrthographic ( in the engine command line options
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);

    return;
}

/// Add logo to the viewport
void GameEconomicGameClient::AddLogoViewport(void)
{
    /// Get Needed SubSystems
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    /// Create logo object
    Node* existencelogoNode = scene_->CreateChild("ExistenceLogo");

    /// Setup logo object properities
    existencelogoNode ->SetPosition(Vector3(0.0,2.0,2.0));
    existencelogoNode ->SetRotation(Quaternion(0.0, 0.0,0.0));
    existencelogoNode ->SetName("ExistenceLogo");

    StaticModel* existencelogoObject = existencelogoNode->CreateComponent<StaticModel>();
    existencelogoObject->SetModel(cache->GetResource<Model>("Resources/Models/existencelogo.mdl"));
    existencelogoObject->SetMaterial(cache->GetResource<Material>("Resources/Materials/existencelogo.xml"));

    return;
}


/// Handle updates
void GameEconomicGameClient::HandleUpdate(StringHash eventType, VariantMap& eventData)
{

    using namespace Update;

    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    /// network updating
    NetworkingOnUpdate(timeStep);

    return;
}

/// Print to output buffe console
void GameEconomicGameClient::Print(const String& output)
{
    /// Logging appears both in the engine console and stdout
    LOGRAW(output + "\n");
}

/// Create a cursor
int  GameEconomicGameClient::CreateCursor(void)
{

    /// Define Resouces
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();
    FileSystem * filesystem = GetSubsystem<FileSystem>();

    /// Create a Cursor UI element because we want to be able to hide and show it at will. When hidden, the mouse cursor will
    /// control the camera, and when visible, it will point the raycast target
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    SharedPtr<Cursor> cursor(new Cursor(context_));

    ui->SetCursor(cursor);
    cursor->SetStyle("Cursor",style);

    /// Set starting position of the cursor at the rendering window center
    cursor->SetPosition(graphics->GetWidth() / 2, graphics->GetHeight() / 2);

    return 1;

}

/// Setup the screen UI
void GameEconomicGameClient::SetupScreenUI(void)
{
    /// Create a new scene UI
    scenePlayerUI_= new Scene(context_);
    scenePlayerUI_-> CreateComponent<Octree>();
    scenePlayerUI_-> CreateComponent<DebugRenderer>();

    /// Add a lightdelightNode
    Node* lightNode = scenePlayerUI_->CreateChild("uidirectionallight");
    lightNode->SetDirection(Vector3(0.0f, .8f, 3.20f)); /// The direction vector does not need to be normalized
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);

    /// Create a Zone component for ambient lighting & fog control
    Node* zoneNode = scenePlayerUI_->CreateChild("uizone");
    Zone* zone = zoneNode->CreateComponent<Zone>();

    Vector3 boundingBoxMin(-20.0f,-20.0f,-20.0f);
    Vector3 boundingBoxMax(20.0f,20.0f,20.0f);

    /// change bounding box to something more accurate
    zone->SetBoundingBox(BoundingBox(boundingBoxMin,boundingBoxMax));
    zone->SetFogColor(Color(.08f, .08f, .08f));
    zone->SetFogStart(10.0f);
    zone->SetFogEnd(20.0f);
    zone->SetHeightFog (false);

    /// Add Camera
    Node * cameraNodePlayerUI_ = scenePlayerUI_->CreateChild("uicamera");
    cameraNodePlayerUI_->CreateComponent<Camera>();

    /// Set an initial position for the camera scene node above the plane
    cameraNodePlayerUI_->SetPosition(Vector3(0.0f, 0.8f, 3.0f));

    Node * emptyNode= scenePlayerUI_->CreateChild("uiempty");
    emptyNode->SetPosition(Vector3(0.0f,0.73f,0.0f));

    /// Create character node;
    Node * characterNode= scenePlayerUI_->CreateChild("Character");
    characterNode->SetPosition(Vector3(0.0f,0.0f,0.0f));

    cameraNodePlayerUI_ -> LookAt(Vector3(emptyNode->GetPosition()));
    lightNode -> Rotate(Quaternion(.398377,0.854323,0.141073,-0.302532));
}


/// Temporary Network
bool GameEconomicGameClient::IsClientConnected(void)
{
    return ServerConnection;
}

/// Temporary online
bool GameEconomicGameClient::ClientConnect(void)
{
    /// Set Connection
    ServerConnection = false;

    return false;
}
/// Temporary online
bool GameEconomicGameClient::SetServerSettings(void)
{
    /// Set Connection
    ServerConnection = false;

    return false;
}


/// Get subsystems
Renderer * GameEconomicGameClient::GetRenderSubsystems(void) const
{
    /// Define Resouces
    Renderer* CurrentRendererSubsystem = GetSubsystem<Renderer>();

    return CurrentRendererSubsystem;
}

/// Get subsystems
Graphics * GameEconomicGameClient::GetGraphicsSubsystems(void) const
{
    /// Define Resouces
    Graphics* CurrentGraphics = GetSubsystem<Graphics>();

    return CurrentGraphics;
}

/// Get subsystems
UI * GameEconomicGameClient::GetUISubsystems(void) const
{
    /// Define Resouces
    UI * CurrentUISubsystem = GetSubsystem<UI>();

    return CurrentUISubsystem;
}

/// Get subsystems
ResourceCache * GameEconomicGameClient::GetResourceCacheSubsystems(void) const
{
    /// Define Resouces
    ResourceCache * CurrentResourceCacheSubsystem = GetSubsystem<ResourceCache>();

    return CurrentResourceCacheSubsystem;
}

Window * GameEconomicGameClient::GetSharedWindow(void) const
{
    return window_;
}

void GameEconomicGameClient::Exit()
{
    engine_->Exit();
}





/// code to handle console command inputs
void GameEconomicGameClient::HandleConsoleCommand(StringHash eventType, VariantMap& eventData)
{
    using namespace ConsoleCommand;

    if (eventData[P_ID].GetString() == GetTypeName())        HandleInput(eventData[P_COMMAND].GetString());
}


/// Code to handle actual commans
void GameEconomicGameClient::HandleInput(const String& input)
{
    /// Get needed resources
    Renderer* renderer = GetSubsystem<Renderer>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();
    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();


}


/// Initialize the main console
void GameEconomicGameClient::InitializeConsole(void)
{
    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// create basic console
    Console* console = GetSubsystem<Console>();
    console->SetNumRows(2);
    console->SetNumBufferedRows(32);
    console->SetCommandInterpreter(GetTypeName());
    console->SetVisible(false);
    console->GetCloseButton()->SetVisible(false);

    gamestatehandlercomponent_ ->SetConsoleState(UI_CONSOLEOFF);
}


/// Handler for keydown
void GameEconomicGameClient::HandlerFunctionKeyDown(StringHash eventType, VariantMap& eventData)
{
    /// Get Urho3D Subsystem
    UI* ui = GetSubsystem<UI>();

    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Set component state to current visibility
    gamestatehandlercomponent_->SetConsoleState(GetSubsystem<Console>()->IsVisible());


    return;
}


/// Erase the UI
void GameEconomicGameClient::EraseUI(void)
{
    /// Get resources
    UI * ui_ = GetSubsystem<UI>();

    /// Clear
    ui_ -> Clear();

    return;
}

void GameEconomicGameClient::UpdateNetworkStatusUI(bool online)
{
    /// Get Urho3D Subsystem
    UI* ui_ = GetSubsystem<UI>();

    /// load window
    UIElement * uiroot = ui_->	GetRoot ();

    Text * NetworkStatusUpdateText = (Text *) uiroot->GetChild("ServerStatusUpdateText",true);

    if(NetworkStatusUpdateText)
    {
        if(online)
        {
            NetworkStatusUpdateText->SetText("Online");
        }
        else
        {
            NetworkStatusUpdateText->SetText("Offline");
        }
    }

    return;
}

