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
#include "../../../Urho3D/Input/InputEvents.h"

#include "../GameEconomicComponents/GameStateHandlerComponent.h"
#include "../GameEconomicComponents/GameStateEvents.h"
#include "../Player.h"
#include "../GameEconomicComponents/Starbase.h"
#include "../GameEconomicComponents/PowerComponent.h"
#include "../GameEconomicComponents/InteractObject.h"
#include "../CommunicationLogs.h"

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
#include "GameEconomicGameClientStateGameMode.h"

#include "../../Urho3D/Engine/DebugHud.h"

#define DEFAULTSIZE 4096

using namespace std;
using namespace Urho3D;
using namespace Update;

/// Client Game Mode
GameEconomicGameClientStateGameMode::GameEconomicGameClientStateGameMode(Context* context):
    GameEconomicGameClientStateSingleton (context)
    ,Existence(NULL)
{

    /// Debug
    cout << "Debug: State Game Mode Constructor" << endl;

    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Set aApplication
    Existence = gamestatehandlercomponent_->GetApplication();

    /// Set UI State
    gamestatehandlercomponent_->SetUIState(UI_GAMECONSOLE);

    /// Subscribe () function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(GameEconomicGameClientStateGameMode, HandleUpdate));

    ///  Handle Post Updates
    SubscribeToEvent(E_POSTUPDATE,HANDLER(GameEconomicGameClientStateGameMode,HandlerPostUpdates));

    /// Subscribe to event (Listen to Load Change)
    SubscribeToEvent(INTERACTEVENT, HANDLER(GameEconomicGameClientStateGameMode, InteractListener));

    return;
}

/// Deconstructor
GameEconomicGameClientStateGameMode::~GameEconomicGameClientStateGameMode()
{
    /// Debug
    cout << "Debug: State Game Mode Deconstructor" << endl;

    return;
}

/// Enter
void GameEconomicGameClientStateGameMode::Enter(void)
{
    /// Debug
    cout << "Debug: State Game Mode Enter" << endl;

    /// on ENter
    GameMode();

    return;
}

/// Exit
void GameEconomicGameClientStateGameMode::Exit(void)
{

    /// Debug
    cout << "Debug: State Game Mode Exit" << endl;

    return;
}

/// Null for now
void GameEconomicGameClientStateGameMode::SetParameter(String parameter_)
{

    /// Does Nothing

    return;
}

/// Game Mode code
void GameEconomicGameClientStateGameMode::GameMode(void)
{
    /// Get all Revelant resources
    ResourceCache* cache_ = GetSubsystem<ResourceCache>();
    Renderer* renderer_ = GetSubsystem<Renderer>();
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI* ui_ = GetSubsystem<UI>();
    FileSystem * filesystem_ = GetSubsystem<FileSystem>();

    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Temporary for use flag of states
    bool CurrentStateIsGameMode = true;

    /// Set Camera mode to first person
    gamestatehandlercomponent_->SetCameraMode(CAMERAMODE_FIRSTPERSON);

    Existence->cameraNode_ = Existence->scene_->GetChild("Camera");

    UIElement * uiRoot_ = ui_ -> GetRoot();

    /// Create UI
    UIElement * GameUIElement = new UIElement(context_);
    GameUIElement -> SetName("GameUI");

    uiRoot_ ->AddChild(GameUIElement);

    /// Set an initial position for the camera scene node above the plane
    Existence->cameraNode_->SetPosition(Vector3(0.0f,1.0f,0.0f));
    Existence->cameraNode_->SetRotation(Quaternion(0.1f,0.1f,0.1f));

    /// Set first person camera Node
    Camera* cameraObject = Existence->cameraNode_->GetComponent<Camera>();
    cameraObject->SetOrthographic(0);
    cameraObject->SetZoom(1);
    cameraObject->SetNearClip(0.0f);
    cameraObject->SetUseClipping(false);

    /// if touch enabled
    if(Existence->touchenabled_)
    {
        Existence->cameraNode_->SetPosition(Vector3(-1.0f,.5f,-1.0f));
        cameraObject->SetOrthographic(true);
        cameraObject->SetZoom(5);
    }

    /// Change viewport
    Existence->viewport =  new Viewport(context_, Existence->scene_, cameraObject);
    renderer_->SetViewport(0, Existence->viewport);

    Existence->effectRenderPath = Existence->viewport->GetRenderPath() -> Clone();

    Existence->effectRenderPath->Append(cache_->GetResource<XMLFile>("PostProcess/Bloom.xml"));
    Existence->effectRenderPath->Append(cache_->GetResource<XMLFile>("PostProcess/FXAA3.xml"));

    /// Make the bloom mixing parameter more pronounced
    Existence->effectRenderPath->SetShaderParameter("BloomMix", Vector2(Existence->GameConfig->VideoBloomParam1,
            Existence->GameConfig->VideoBloomParam2));
    Existence->effectRenderPath->SetEnabled("Bloom", false);
    Existence->effectRenderPath->SetEnabled("FXAA3", false);

    Existence -> viewport->SetRenderPath(Existence->effectRenderPath);

    /// Get rendering window size as floats
    float Width = (float)graphics_->GetWidth();
    float Height = (float)graphics_->GetHeight();

    if(Existence->touchenabled_==true)
    {

        Sprite* BackgroundSprite = new Sprite(context_);
        BackgroundSprite->SetName("BackgroundSprite");

        /// Get letter box image
        Texture2D* texture = cache_ ->GetResource<Texture2D>("Resources/Textures2D/GenericBackground.png");

        /// Set letter box properties
        BackgroundSprite->SetTexture(texture); // Set texture
        BackgroundSprite->SetSize(Width,Height);
        BackgroundSprite->SetAlignment(HA_CENTER, VA_CENTER);
        BackgroundSprite->SetBlendMode(BLEND_ALPHA);

        /// Create letter box image to UIElement
        UIElement * BackgroundUIElement = new UIElement(context_);
        BackgroundUIElement->AddChild(BackgroundSprite);

        /// Add letter box UIElement to ui
        uiRoot_->AddChild(BackgroundUIElement);
    }

    Existence->loadHUDFile("Resources/UI/MainTopBarWindow.xml",0,0);

    Sprite * MainTopBarBackground = (Sprite *) Existence->uiRoot_->GetChild("MainTopBarBackground",true);


    /// if UI load set its width to match the screen
    if(MainTopBarBackground)
    {
        MainTopBarBackground->SetScale(1.0f+((Width-1280)/1280),1);
    }

    GameModeAddUIElements();



    /// load hud
    Existence->loadUIXML(UIMINIQUICKMENU,0,0,0);

    LoadUIXML(UIGAME_HUDCLAUDIUS,0,0);

    /// Attach a listen
    SubscribeToEvent(G_MODE_CHANGE, HANDLER(GameEconomicGameClientStateGameMode, GameModeSendEventHandler));

    /// Get StarbaseNode
    Node * StarbaseNode = Existence->scene_ ->GetChild("StarBaseNode",true);
    Starbase * StarbaseComponent = StarbaseNode->GetComponent<Starbase>();

    StarbaseComponent->SetOnline(true);



    return;
}

/// Blank function for now
void GameEconomicGameClientStateGameMode::OnUpdate(StringHash eventType, VariantMap& eventData)
{

    /// Does Nothing for now
    return;
}

/// Handle updates
void GameEconomicGameClientStateGameMode::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    Input* input = GetSubsystem<Input>();

    /// Move the camera, scale movement with time step
    OnMoveCamera(timeStep);

    /// Paint decal with the left mousebutton; cursor must be visible
    if (GetSubsystem<UI>()->GetCursor()->IsVisible() && input->GetMouseButtonPress(MOUSEB_LEFT))
    {
        GetTargetPressed();
    }

    /// Handle that update
    HandleUIUpdate(timeStep);

    /// Update hud
    return;
}

void GameEconomicGameClientStateGameMode::HandleUIUpdate(float timeStep)
{
    /// Get subsystems
    UI* ui_ = GetSubsystem<UI>();

    UIElement * RootUIElement = ui_->GetRoot();

    /// Variables
    float Oxygen;

    /// Get Oxygen levels
    if(Existence->ThisStarbase->ThisComponent)
    {
        Starbase * TempStarbase = (Starbase *)Existence->ThisStarbase->ThisComponent;

        Oxygen=TempStarbase->GetOxygen();

    }

    /// Get Oxygen Level
    if(Text * OxygenText=(Text *)RootUIElement->GetChild("OxygenText",true))
    {
        if(Oxygen>(2048*.75))
        {
            OxygenText->SetText("Nominal");
        }
    }

    return;
}
/// On any camera movement
void GameEconomicGameClientStateGameMode::OnMoveCamera(float timeStep)
{
    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    Input* input = GetSubsystem<Input>();

    static bool MoveCamera = false;
    /// Movement speed as world units per second
    float MOVE_SPEED=5.0f;

    /// Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.2f;

    /// Alt override for Second Life controls
    if(MoveCamera||input->GetKeyDown(KEY_ALT))
    {
        if(Existence->touchenabled_==false)
        {
            /// Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
            IntVector2 mouseMove = input->GetMouseMove();
            Existence->yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
            Existence->pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
            Existence->pitch_ = Clamp(Existence->pitch_, -180.0f, 180.0f);

            /// Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
            Existence->cameraNode_->SetRotation(Quaternion(Existence->pitch_, Existence->yaw_, 0.0f));
        }
    }

    /// Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    /// Use the Translate() function (default local space) to move relative to the node's orientation.
    if (input->GetKeyDown('W'))
    {
        Existence->cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    }
    else if (input->GetKeyDown('S'))
    {
        Existence->cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    }
    else if (input->GetKeyDown('A'))
    {
        Existence->cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    }
    else if (input->GetKeyDown('D'))
    {
        Existence->cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
    }
    /// added controls for up and down movement
    else if (input->GetKeyDown('Q'))
    {
        Existence->cameraNode_->Translate(Vector3::UP * MOVE_SPEED * timeStep);
    }
    else if (input->GetKeyDown('E'))
    {
        Existence->cameraNode_->Translate(Vector3::DOWN * MOVE_SPEED * timeStep);
    }
    /// added controls for up and down movement
    else if (input->GetKeyDown('1'))
    {

        MOVE_SPEED=5.0f;
    }
    else if (input->GetKeyDown('2'))
    {
        MOVE_SPEED=15.0f;
    }
    else if (input->GetKeyDown('3'))
    {
        MOVE_SPEED=10.0f;
    }
    else if (input->GetKeyDown('4'))
    {
        MOVE_SPEED=20.0f;
    }
    else if (input->GetKeyDown('\\'))
    {
        if(MoveCamera)
        {
            MoveCamera=false;
        }
        else
        {
            MoveCamera=true;
        }

    }
    return;
}

/// load the game mode user interface
void GameEconomicGameClientStateGameMode::LoadGameModeUI(void)
{

}

/// Handle post updates
void GameEconomicGameClientStateGameMode::HandlerPostUpdates(StringHash eventType, VariantMap& eventData)
{

}

/// check if key was pressed
void GameEconomicGameClientStateGameMode::GetTargetPressed(void)
{
    // Data Needed
    Vector3 hitPos;
    Node* hitNode;
    IntVector2 hitMousePosition;
    /// blank Node for now
    hitNode = NULL;

    /// Raycast function
    if (Raycast(250.0f, hitPos, hitNode,hitMousePosition)==true)
    {
        /// Debug
        //cout << "Debug : " << hitNode -> GetName().CString() << endl;
        cout <<  hitMousePosition.ToString().CString() << endl;

        /// Get object and position
        if(InteractObject * interactable = hitNode->GetComponent<InteractObject>())
        {
            /// setup event data
            VariantMap eventData;

            /// Send Load Event
            eventData[InteractEvent::P_NODE] = hitNode;
            eventData[InteractEvent::P_HITPOSITION] = hitPos;
            eventData[InteractEvent::P_MOUSEPOSITION] = hitMousePosition;
            eventData[InteractEvent::P_OBJ] = this;

            /// Send event
            SendEvent(INTERACTEVENT, eventData);
        }

    }

    return;
}

/// test raycasts
bool GameEconomicGameClientStateGameMode::Raycast(float maxDistance, Vector3& hitPos, Node*& hitNode, IntVector2& hitMousePosition)
{

    /// Get subsystems
    UI* ui = GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    /// first get the mouse position
    hitMousePosition = ui->GetCursorPosition();


    IntVector2 pos=hitMousePosition;

    /// Check the cursor is visible and there is no UI element in front of the cursor
    if (!ui->GetCursor()->IsVisible() || ui->GetElementAt(pos, true))
        return false;

    /// Get the camera position
    Node * cameraNode_ = Existence -> scene_ -> GetChild("Camera",true);
    Camera* camera = cameraNode_->GetComponent<Camera>();

    /// If node doesn't exist
    if(cameraNode_==NULL)
    {
        return false;
    }

    /// Get the pointer position
    Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());

    /// Pick only geometry objects, not eg. zones or #include "../../../Urho3D/Scene/LogicComponent.h"s, only get the first (closest) hit
    PODVector<RayQueryResult> results;
    RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY, DEFAULT_VIEWMASK &~ (1 << 0));
    Existence->scene_->GetComponent<Octree>()->RaycastSingle(query);

    /// If a result then return result
    if (results.Size())
    {
        RayQueryResult& result = results[0];
        hitPos = result.position_;
        /// Get drawable
        Drawable * drawable = result.drawable_;

        hitNode  = drawable->GetNode();

        return true;
    }

    return false;
}

/// Handle updates
void GameEconomicGameClientStateGameMode::InteractListener(StringHash eventType, VariantMap& eventData)
{
    /// Get subsystems
    UI* ui_ = GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    /// Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

    ///  Reaname all UIRoot to UIroot

    UIElement * UIRoot_ = ui_ -> GetRoot();

    /// intercept state event
    Node * hitNode=  dynamic_cast<Node *>(eventData[InteractEvent::P_NODE].GetPtr());
    Vector3 hitPosition = eventData[InteractEvent::P_HITPOSITION].GetVector3();
    IntVector2 hitMousePosition = eventData[InteractEvent::P_MOUSEPOSITION].GetIntVector2();
    Node * sender=  dynamic_cast<Node *>(eventData[InteractEvent::P_OBJ].GetPtr());

    /// Load interact window
    LoadUIXML(UIGAME_UIINTERACT,0,0);

    /// get thw window
    Window * TestingDisplayBriefWindow = (Window *) UIRoot_->GetChild("TestingDisplayBriefWindow", true);

    /// Make sure the window does not overlap
    if(hitMousePosition.x_>width-TestingDisplayBriefWindow->GetWidth())
    {
        hitMousePosition.x_= width-TestingDisplayBriefWindow->GetWidth();
    }

    if(hitMousePosition.y_>height-TestingDisplayBriefWindow->GetHeight())
    {
        hitMousePosition.y_= height-TestingDisplayBriefWindow->GetHeight();
    }

    /// Set of the window
    TestingDisplayBriefWindow->SetPosition(hitMousePosition.x_,hitMousePosition.y_);

    /// Get ResourceCOmponent
    ResourceNodeComponent * NodeResources = hitNode->GetComponent<ResourceNodeComponent>();

    /// IF resource node fined
    if(NodeResources)
    {
        /// Get the text window
        Text * TestingText = (Text *) TestingDisplayBriefWindow->GetChild("TestingText", true);

        /// Get Resource Type
        if(TestingText)
        {
            ResourceComponentType ResourceComponentTYpeInfo=NodeResources->GetResourceComponentType();

            switch (ResourceComponentTYpeInfo)
            {
                /// Change text to show replication printer
            case RCType_ReplicationPrinter:
                TestingText->SetText(String("Replication Printer Interact"));
                break;
                ///  Change title to say refrigeration unit
            case RCType_RefrigerationUnit:
                TestingText->SetText(String("Refrigeration Unit Interact"));
                break;
            case RCType_PowerSource:
                TestingText->SetText(String("Power Source Unit Interact"));
                break;
            case RCType_Drone:
                TestingText->SetText(String("Drone Interact"));
                break;
            default:
                break;
            }
        }

        /// Check if listview and exist
        ListView * ResourceListView = (ListView *) TestingDisplayBriefWindow->GetChild("ResourcesListView", true);
        ListView * StorageListView = (ListView *) TestingDisplayBriefWindow->GetChild("StorageListView", true);

        /// Get resources and add
        if(ResourceListView&&StorageListView)
        {
            ResourceListView->RemoveAllItems();
            StorageListView->RemoveAllItems();

            for(unsigned int i=0; i<NodeResources->TotalNodeResources(); i++)
            {
                ResourceNodeInformation ResourceFromNode= NodeResources->GetNodeResource(i);

                /// Create new Text
                Text * newItem = new Text(context_);

                newItem->SetEditable(false);
                newItem->SetEnabled(true);

                newItem->SetText(ResourceFromNode.Resource.ResourceName);

                ResourceListView->AddItem(newItem);

                newItem->SetStyleAuto();
            }
        }


    }



    return;
}


void GameEconomicGameClientStateGameMode::ServerResponseHandler(StringHash eventType, VariantMap& eventData)
{

}

void GameEconomicGameClientStateGameMode::GameModeSendEventHandler(StringHash eventType, VariantMap& eventData)
{
    /// Take the frame time step, which is stored as a float
    unsigned int cmdType = eventData[GameModeChange::P_CMD].GetInt();
    String cmdArg = eventData[GameModeChange::P_ARG].GetString();

    /// if terminate
    if(cmdType==GAMEMODE_EVENT_TERMINATE)
    {
        /// Unsubcribe from all events
        UnsubscribeFromAllEvents();

        /// This is temoporarily the necessary code
        Existence->EraseUI();

        /// clear viables
        delete Existence->ThisStarbase;
        delete Existence->ThisPlayer;

        /// create new
        Existence->ThisStarbase = new StarbaseInformation();
        Existence->ThisPlayer = new PlayerObject();

        /// Create a event
        VariantMap gamestatechange;
        gamestatechange[GameState::P_CMD] = GAME_STATE_MAINMENU;

        cout << "Debug: Attempt to send a state change" << endl;

        this->SendEvent(G_STATES_CHANGE,gamestatechange);

    }
}

void GameEconomicGameClientStateGameMode::GameModeAddUIElements(void)
{
/// Get all Revelant resources
    ResourceCache* cache_ = GetSubsystem<ResourceCache>();
    Renderer* renderer_ = GetSubsystem<Renderer>();
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI* ui_ = GetSubsystem<UI>();
    FileSystem * filesystem_ = GetSubsystem<FileSystem>();

    UIElement * uiRoot_ = ui_ -> GetRoot();

    UIElement*MainTopBarMenuUIElement =uiRoot_->GetChild("MainTopBarMenuUIElement", true);

    float Width = graphics_->GetWidth();

    Button* StarbaseButton = new Button(context_);
    StarbaseButton->SetName("StarbaseButton");
    StarbaseButton->SetFixedSize(90,16);
    StarbaseButton->SetOpacity(.7);

    Button* CommunicationsButton = new Button(context_);
    CommunicationsButton->SetName("CommunicationsButton");
    CommunicationsButton->SetFixedSize(128,16);
    CommunicationsButton->SetOpacity(.7);

    Button* ActivitiesButton = new Button(context_);
    ActivitiesButton->SetName("ActivitiesButton");
    ActivitiesButton->SetFixedSize(90,16);
    ActivitiesButton->SetOpacity(.7);

    /// Text area
    Text* StarbaseText=new Text(context_);
    Text* GalaxyText=new Text(context_);
    Text* ActivityText=new Text(context_);
    Text* CommunicationText=new Text(context_);

    StarbaseText->SetName("StarbaseText");
    StarbaseText->SetText("| STARBASE |");
    GalaxyText->SetName("GalaxyText");
    GalaxyText->SetText("| GALAXY |");
    ActivityText->SetName("ActivityText");
    ActivityText->SetText("| ACTIVITY |");
    CommunicationText->SetName("CommunicationText");
    CommunicationText->SetText("| COMMUNICATION |");

    /// addlones
    MainTopBarMenuUIElement ->AddChild(StarbaseButton);
    MainTopBarMenuUIElement ->AddChild(StarbaseText);
    MainTopBarMenuUIElement ->AddChild(GalaxyText);
    MainTopBarMenuUIElement ->AddChild(ActivitiesButton);
    MainTopBarMenuUIElement ->AddChild(ActivityText);
    MainTopBarMenuUIElement ->AddChild(CommunicationsButton);
    MainTopBarMenuUIElement ->AddChild(CommunicationText);

    StarbaseText->SetPosition(Width/2+336-8,2);
    StarbaseButton->SetPosition(Width/2+336+2-8,2);

    GalaxyText->SetPosition(Width/2+426-8,2);

    ActivityText->SetPosition(Width/2+496-8,2);
    ActivitiesButton->SetPosition(Width/2+498-8,2);

    CommunicationText->SetPosition(Width/2+587-8,2);
    CommunicationsButton->SetPosition(Width/2+591-8,2);

    /// Apply styles
    StarbaseText->SetStyleAuto();
    StarbaseButton->SetStyleAuto();
    GalaxyText->SetStyleAuto();
    ActivityText->SetStyleAuto();
    ActivitiesButton->SetStyleAuto();
    CommunicationText->SetStyleAuto();
    CommunicationsButton->SetStyleAuto();

    /// If button is relesed
    SubscribeToEvent(StarbaseButton, E_RELEASED, HANDLER(GameEconomicGameClientStateGameMode, HandleTopMenuPressed));
    SubscribeToEvent(CommunicationsButton, E_RELEASED, HANDLER(GameEconomicGameClientStateGameMode, HandleTopMenuPressed));
    SubscribeToEvent(ActivitiesButton, E_RELEASED, HANDLER(GameEconomicGameClientStateGameMode, HandleTopMenuPressed));

    return;
}

/// Load a HUD file in a XML format in the file system - Prevent double loading
bool GameEconomicGameClientStateGameMode::LoadUIXML(int windowtype, const int positionx=0, const int positiony=0)
{
    /// Get resources
    ResourceCache * cache = GetSubsystem<ResourceCache>();
    FileSystem * filesystem = GetSubsystem<FileSystem>();
    UI* ui_ = GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    /// Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

    /// get current root
    UIElement * RootUIElement = ui_->GetRoot();
    UIElement * GameModeUIElement= RootUIElement->GetChild("GameUI",true);
    UIElement * HUDFileElement= new UIElement(context_);

    /// if no GameUI return false
    if(GameModeUIElement==NULL)
    {
        return 0;
    }

    /// Filename Hud
    String filenameHUD;

    /// chose based on menu type
    if(windowtype==UIGAME_UISTARBASEDISPLAYBRIEF)
    {
        /// If window exist
        UIElement * StarbaseDisplayBriefUIElement= GameModeUIElement->GetChild("StarbaseDisplayBriefUIElement",true);

        /// if the window exist exit
        if(StarbaseDisplayBriefUIElement)
        {
            StarbaseDisplayBriefUIElement->SetFocus(true);

            return 0;
        }

        /// Append the file
        filenameHUD.Append("Resources/UI/GameStarbaseDisplayBrief.xml");
    }


    /// chose based on menu type
    if(windowtype==UIGAME_UICOMMUNICATIONS)
    {
        /// If window exist
        UIElement * CommunicationsUIElement= GameModeUIElement->GetChild("CommunicationsLogsUIElement",true);

        /// if the window exist exit
        if(CommunicationsUIElement)
        {
            CommunicationsUIElement->SetFocus(true);

            return 0;
        }

        /// Append the file
        filenameHUD.Append("Resources/UI/CommunicationsLogs.xml");
    }


    /// chose based on menu type
    if(windowtype==UIGAME_UIINTERACT)
    {
        /// If window exist
        UIElement * StarbaseDisplayBriefUIElement= GameModeUIElement->GetChild("TestingUIElement",true);

        /// if the window exist exit
        if(StarbaseDisplayBriefUIElement)
        {
            StarbaseDisplayBriefUIElement->SetFocus(true);

            return 0;
        }

        /// Append the file
        filenameHUD.Append("Resources/UI/Testing.xml");
    }

    if(windowtype==UIGAME_HUDCLAUDIUS)
    {
        /// If window exist
        UIElement * ReadOutsHudUIElement= GameModeUIElement->GetChild("ReadOutsHudUIElement",true);

        /// if the window exist exit
        if(ReadOutsHudUIElement)
        {
            return 0;
        }

        /// Append the file
        filenameHUD.Append("Resources/UI/HudClaudius.xml");
    }


    if(windowtype==UIGAME_UIACTIVITIES)
    {
        /// If window exist
        UIElement * ReadOutsHudUIElement= GameModeUIElement->GetChild("ActivitiesHolderUIElement",true);

        /// if the window exist exit
        if(ReadOutsHudUIElement)
        {
            return 0;
        }

        /// Append the file
        filenameHUD.Append("Resources/UI/Activity.xml");
    }


    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    /// Configure resources
    XMLElement hudElement;

    /// Load Resource
    XMLFile* hudFile= cache->GetResource<XMLFile>(filenameHUD);

    /// if filename can't be find exit
    if(hudFile==NULL)
    {
        return 0;
    }

    /// Get root element XML
    hudElement =  hudFile->GetRoot();

    /// Add a min top bar
    HUDFileElement-> LoadXML(hudElement, style);

    /// Add a uielement for the bar
    GameModeUIElement -> AddChild(HUDFileElement);

    /// Position the window
    HUDFileElement -> SetPosition(positionx,positiony);

    /// SetStyleAUto
    HUDFileElement->SetStyleAuto();

    /// Set Parameters
    HUDFileElement->SetSize(1440,900);
    HUDFileElement->SetFocusMode(FM_FOCUSABLE_DEFOCUSABLE);

    /// chose based on menu type
    if(windowtype==UIGAME_UISTARBASEDISPLAYBRIEF)
    {
        Button * StarbaseDisplayBriefDiagnosticButton = (Button *) HUDFileElement -> GetChild("StarbaseDisplayBriefDiagnosticButton",true);

        SubscribeToEvent(StarbaseDisplayBriefDiagnosticButton,E_RELEASED, HANDLER(GameEconomicGameClientStateGameMode, HandleUIStarbaseBriefButtonPressed));

        /// Get StarbaseNode
        Node * StarbaseNode = Existence->scene_ ->GetChild("StarBaseNode",true);
        Starbase * StarbaseComponent = StarbaseNode->GetComponent<Starbase>();

        double TotalPower=StarbaseComponent->GetTotalPower();
        double UsedPower=StarbaseComponent->GetUsedPower();

        /// Get oxygen
        float Oxygen = StarbaseComponent->GetOxygen();

        /// Get Oxygen Level
        if(Text * OxygenText=(Text *)HUDFileElement->GetChild("OxygenBriefText",true))
        {
            if(Oxygen>(2048.0f*0.75f))
            {
                OxygenText->SetText("Nominal");
            }
        }


    }

    /// Set Position
    if(windowtype==UIGAME_HUDCLAUDIUS)
    {
        HUDFileElement -> SetPosition(0,64);
    }

    /// Get the child and assign a close pressed
    Button * closebutton = (Button *) HUDFileElement -> GetChild("closeButton",true);

    /// If the close button exist
    if(closebutton)
    {
        /// Assign close function to the button
        SubscribeToEvent(closebutton, E_RELEASED, HANDLER(GameEconomicGameClientStateGameMode, HandleUIWindowClosed));
    }


    return true;
}

/// Handler for top menu pressed
void GameEconomicGameClientStateGameMode::HandleTopMenuPressed(StringHash eventType, VariantMap& eventData)
{
    /// Get needed resources
    Renderer* renderer = GetSubsystem<Renderer>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui_ = GetSubsystem<UI>();
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    Graphics* graphics = GetSubsystem<Graphics>();

    /// Get rendering window size as floats
    float Width = (float)graphics->GetWidth();
    float Height = (float)graphics->GetHeight();

    /// get the button that was clicked
    Button* clicked = static_cast<Button*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());

    String clickedButtonString(clicked->GetName());

    /// Get the UI Root element
    UIElement * UIRoot = ui_->GetRoot();

    /// If exit was clicked
    if (clickedButtonString.Contains("StarbaseButton")==true)
    {
        /// Load UI
        LoadUIXML(UIGAME_UISTARBASEDISPLAYBRIEF,0,0);

        /// Get the Window
        Window * BriefWindow = (Window *) UIRoot->GetChild("StarbaseDisplayBriefWindow",true);

        /// If window exist
        BriefWindow->SetPosition((Width/2)-(BriefWindow->GetWidth()/2),(Height/2)-(BriefWindow->GetHeight()/2));

        /// Update Title
        /// Get StarbaseNode
        Node * StarbaseNode = Existence->scene_ ->GetChild("StarBaseNode",true);
        Starbase * StarbaseComponent = StarbaseNode->GetComponent<Starbase>();

        Text * StarbaseNameText = (Text *) BriefWindow->GetChild("StarbaseName",true);
        StarbaseNameText->SetText(Existence->ThisStarbase->Name);

        /// Get poewr
        Text * StarbaseTotalPower = (Text *) BriefWindow->GetChild("StarbaseTotalPower",true);
        Text * StarbaseUsedPower = (Text *) BriefWindow->GetChild("StarbaseUsedPower",true);

        double TotalPower = StarbaseComponent->GetTotalPower();
        double UsedPower = StarbaseComponent->GetUsedPower();

        String TotalPowerString = String("Total Power :")+String(TotalPower);
        String UsedPowerString = String("Used Power :")+String(TotalPower-UsedPower);

        StarbaseTotalPower->SetText(TotalPowerString);
        StarbaseUsedPower->SetText(UsedPowerString);

    }

    /// If exit was clicked
    if (clickedButtonString.Contains("CommunicationsButton")==true)
    {
        /// Load UI
        LoadUIXML(UIGAME_UICOMMUNICATIONS,0,0);

        /// Get the Window
        Window * CommunicationLogsWindow = (Window *) UIRoot->GetChild("CommunicationsLogsWindow",true);

        CommunicationLogsWindow->SetPosition((Width/2)-(CommunicationLogsWindow->GetWidth()/2),(Height/2)-(CommunicationLogsWindow->GetHeight()/2));

        /// Populate
        TempLogs = new Vector<CommunicationLog>();

        ListView * CommunicationsLogListView = (ListView *) UIRoot->GetChild("CommunicationsLogListView",true);

        Existence->LoadCommunicationLogs(LogFormat_Personal, TempLogs);

        /// loop
        if(TempLogs->Size()>0&&CommunicationsLogListView)
        {
            /// loop through the size
            for(unsigned int i=0; i<TempLogs->Size(); i++)
            {
                Text * NewLogItem = new Text(context_);

                NewLogItem->SetText(TempLogs->At(i).Title);
                NewLogItem->SetName(String(i));

                /// AddLogItem
                CommunicationsLogListView->AddItem(NewLogItem);
                /// Set Style
                NewLogItem->SetStyleAuto();


                /// Add each selection color
                NewLogItem->SetSelectionColor (Color(0.0f,0.0f,0.5f));
                NewLogItem->SetHoverColor (Color(0.0f,0.0f,1.0f));


                /// Set Enabled
                NewLogItem->SetEnabled(true);
                NewLogItem->SetEditable(false);
                NewLogItem->SetVisible(true);
            }

            /// Set windows settings
            CommunicationsLogListView->SetStyleAuto();
            CommunicationsLogListView->SetMultiselect(false);
            CommunicationsLogListView->SetSelection(0);

            SubscribeToEvent(CommunicationsLogListView, E_ITEMDOUBLECLICKED,HANDLER(GameEconomicGameClientStateGameMode,HandleCommunicationsLogView));
        }
    }


    /// If exit was clicked
    if (clickedButtonString.Contains("ActivitiesButton")==true)
    {
        /// Load UI
        LoadUIXML(UIGAME_UIACTIVITIES,0,0);

        /// Get the Window
        Window * ActivitiesWindow = (Window *) UIRoot->GetChild("ActivitiesWindow",true);

        ActivitiesWindow->SetPosition((Width/2)-(ActivitiesWindow->GetWidth()/2),(Height/2)-(ActivitiesWindow->GetHeight()/2));


    }

    return;
}


void GameEconomicGameClientStateGameMode::HandleCommunicationsLogView(StringHash eventType, VariantMap& eventData)
{

    /// Resource
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    UI * ui_ = GetSubsystem<UI>();

    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get rendering window size as floats
    UIElement* CommunicationsLogsUIElement = (UIElement *) UIRoot ->GetChild("CommunicationsLogsUIElement",true);

    /// Get needed info
    Text* selectedItem = (Text*)(eventData[ItemDoubleClicked::P_ELEMENT].GetPtr());

    unsigned int selection= strtoul(selectedItem->GetName().CString(),NULL,0);

    Text * CommunicationsLogText = (Text *)CommunicationsLogsUIElement->GetChild("CommunicationsLogText", true);

    if(CommunicationsLogText)
    {
        CommunicationsLogText->SetText(TempLogs->At(selection).Text);
    }

    return;
}

/// Handler for window created
void GameEconomicGameClientStateGameMode::HandleUIWindowClosed(StringHash eventType, VariantMap& eventData)
{

    /// Get roott
    UI* ui_ = GetSubsystem<UI>();

    /// Get the UI Root element
    UIElement * RootUIElement= ui_->GetRoot();

    /// Get control that was clicked
    UIElement* clicked = static_cast<UIElement*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());

    /// Get the parent
    UIElement* selfparent = clicked -> GetParent();

    /// Disable and hide
    selfparent -> SetDeepEnabled(false);
    selfparent -> GetParent() -> Remove();

    return;
}



/// Handler for window created
void GameEconomicGameClientStateGameMode::HandleUIStarbaseBriefButtonPressed(StringHash eventType, VariantMap& eventData)
{

/// Get needed resources
    Renderer* renderer = GetSubsystem<Renderer>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui_ = GetSubsystem<UI>();
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    Graphics* graphics = GetSubsystem<Graphics>();

    /// Get rendering window size as floats
    float Width = (float)graphics->GetWidth();
    float Height = (float)graphics->GetHeight();

    /// get the button that was clicked
    Button* clicked = static_cast<Button*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());

    String clickedButtonString(clicked->GetName());

    /// Get the UI Root element
    UIElement * UIRoot = ui_->GetRoot();

    /// If exit was clicked
    if (clickedButtonString.Contains("StarbaseDisplayBriefDiagnosticButton")==true)
    {
        /// Get StarbaseNode
        Node * StarbaseNode = Existence->scene_ ->GetChild("StarBaseNode",true);
        Starbase * StarbaseComponent = StarbaseNode->GetComponent<Starbase>();

        ListView * StarbaseDisplayBriefAllNodesListView = (ListView *)UIRoot ->GetChild("StarbaseDisplayBriefAllNodesListView", true);

        /// Get the Window
        Window * BriefWindow = (Window *) UIRoot->GetChild("StarbaseDisplayBriefWindow",true);

        /// Get poewr
        Text * StarbaseTotalPower = (Text *) BriefWindow->GetChild("StarbaseTotalPower",true);
        Text * StarbaseUsedPower = (Text *) BriefWindow->GetChild("StarbaseUsedPower",true);

        double TotalPower = StarbaseComponent->GetTotalPower();
        double UsedPower = StarbaseComponent->GetUsedPower();

        String TotalPowerString = String("Total Power :")+String(TotalPower);
        String UsedPowerString = String("Used Power :")+String(TotalPower-UsedPower);

        StarbaseTotalPower->SetText(TotalPowerString);
        StarbaseUsedPower->SetText(UsedPowerString);


        unsigned int numberNodes = StarbaseComponent->GetBaseNodes();

        /// Get oxygen
        float Oxygen = StarbaseComponent->GetOxygen();

        /// Get Oxygen Level
        if(Text * OxygenText=(Text *)UIRoot->GetChild("OxygenBriefText",true))
        {
            if(Oxygen>(2048.0f*0.75f))
            {
                OxygenText->SetText("Nominal");
            }
        }


        /// Show
        if(numberNodes>0)
        {
            StarbaseDisplayBriefAllNodesListView->RemoveAllItems();

            /// Add new items
            for(unsigned int i=0; i<numberNodes; i++)
            {
                /// Create new Text
                Text * newItem = new Text(context_);

                newItem->SetEditable(false);
                newItem->SetEnabled(true);

                StarbaseNodeInformation  SceneNode = StarbaseComponent->GetBaseNode(i);

                /// settext
                String CreateString;

                CreateString.Append(SceneNode.StarbaseNode->GetName());
                CreateString.Append(" <");

                /// OPen the component to get the power
                PowerComponent * StarbaseNodePowerComponent = SceneNode.StarbaseNode->GetComponent<PowerComponent>();

                double CurrentPower = StarbaseNodePowerComponent->GetPower();

                switch (SceneNode.StarbaseNodeType)
                {
                case RCType_Component:
                {
                    CreateString.Append("Component");
                }
                break;
                case RCType_Forcefield:
                {
                    CreateString.Append("Forcefield");

                    /// Copy to string
                    CreateString.Append(" Power:");
                    CreateString.Append(String(CurrentPower));
                }
                break;
                case RCType_Light:
                {
                    CreateString.Append("Light");

                    /// If a lightnode exist
                    if(Node * LightNode = SceneNode.StarbaseNode -> GetChild("Generic_Light", true))
                    {

                        /// Get the component
                        Light * SceneNodeLight = LightNode->GetComponent<Light>();

                        /// If component exist
                        if(SceneNodeLight)
                        {
                            /// Get Brightness
                            float LightBrightness = SceneNodeLight->GetBrightness();
                            CreateString.Append(" Power:");
                            CreateString.Append(String(CurrentPower));

                            /// Copy to string
                            CreateString.Append(" Brightness:");
                            CreateString.Append(String(LightBrightness));
                        }
                    }
                }
                break;
                case RCType_PeriodicElement:
                {
                    CreateString.Append("PeriodicElement");
                }
                break;
                case RCType_PeriodicUnidentified:
                {
                    CreateString.Append("PeriodicUnidentifield");
                }
                break;
                case RCType_RawMaterial:
                {
                    CreateString.Append("RawMaterial");
                }
                break;

                case RCType_Structural:
                {
                    CreateString.Append("Structural");
                }
                break;

                case RCType_Tool:
                {
                    CreateString.Append("Tool");
                }
                break;
                case RCType_ReplicationPrinter:
                {
                    CreateString.Append("ReplicationPrinter");
                    /// Copy to string
                    CreateString.Append(" Power:");
                    CreateString.Append(String(CurrentPower));
                }
                break;
                case RCType_RefrigerationUnit:
                {
                    CreateString.Append("RefrigerationUnit");
                    /// Copy to string
                    CreateString.Append(" Power:");
                    CreateString.Append(String(CurrentPower));
                }
                break;
                case RCType_PowerSource:
                {
                    CreateString.Append("PowerSource");
                    /// Copy to string
                    CreateString.Append(" Power:");
                    CreateString.Append(String(CurrentPower));
                }
                break;
                case RCType_NotApplicable:
                {
                    CreateString.Append("N/A");
                }
                break;

                case RCType_None:

                default:
                {
                    CreateString.Append("None");
                }
                break;

                }
                CreateString.Append(">");

                newItem->SetText(CreateString);
                newItem->SetName(String(i));

                /// Add each selection color
                newItem->SetSelectionColor (Color(0.0f,0.0f,0.5f));
                newItem->SetHoverColor (Color(0.0f,0.0f,1.0f));

                /// add to items
                StarbaseDisplayBriefAllNodesListView->AddItem(newItem);

                newItem->SetStyleAuto();
            }
        }



    }




    return;
}



