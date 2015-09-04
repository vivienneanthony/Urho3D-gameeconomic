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

#include "../GameEconomicComponents/GameStateHandlerComponent.h"
#include "../GameEconomicComponents/GameStateEvents.h"
#include "../Player.h"
#include "../Starbase.h"

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
    //SubscribeToEvent(INTERACTEVENT, HANDLER(GameEconomicGameClientStateGameMode, InteractListener));

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
    Existence->effectRenderPath->SetShaderParameter("BloomMix", Vector2(0.9f, 0.6f));
    Existence->effectRenderPath->SetEnabled("Bloom", false);
    Existence->effectRenderPath->SetEnabled("FXAA3", false);

    Existence -> viewport->SetRenderPath(Existence->effectRenderPath);

    ///Existence->cameraNode_ = cameraNode_;

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


    /// Attach a listen
    SubscribeToEvent(G_MODE_CHANGE, HANDLER(GameEconomicGameClientStateGameMode, GameModeSendEventHandler));



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

    /// Get TargetPressed
    GetTargetPressed();

    return;
}

/// On any camera movement
void GameEconomicGameClientStateGameMode::OnMoveCamera(float timeStep)
{
    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    Input* input = GetSubsystem<Input>();

    /// Movement speed as world units per second
    float MOVE_SPEED=5.0f;

    /// Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.2f;

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
    /*/// Data Needed
    Vector3 hitPos;
    Node* hitNode;

    /// blank Node for now
    hitNode = NULL;

    /// Raycast function
    if (Raycast(250.0f, hitPos, hitNode)==true)
    {

        /// Debug
        cout << "Debug : " << hitNode -> GetName().CString() << endl;

        /*if(InteractObject * interactable = hitNode->GetComponent<InteractObject>())
        {

            /// setup event data
            VariantMap eventData;

            /// Send Load Event
            eventData[InteractEvent::P_CMD] = 1;
            eventData[InteractEvent::P_ARG] = hitPos.ToString();
            eventData[InteractEvent::P_OBJ] = hitNode -> GetName();

            /// Send event
            SendEvent(INTERACTEVENT, eventData);

        }*/



    return;
}

/// test raycasts
bool GameEconomicGameClientStateGameMode::Raycast(float maxDistance, Vector3& hitPos, Node*& hitNode)
{

    /// Get subsystems
    UI* ui = GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    IntVector2 pos = ui->GetCursorPosition();

    /// Check the cursor is visible and there is no UI element in front of the cursor
    if (!ui->GetCursor()->IsVisible() || ui->GetElementAt(pos, true))
        return false;

    /// Get the camera position
    Node * cameraNode_ = Existence -> scene_ -> GetChild("CameraFirstPerson",true);
    Camera* camera = cameraNode_->GetComponent<Camera>();

    /// If node doesn't exist
    if(cameraNode_==NULL)
    {
        return false;
    }

    /// Get the pointer position
    Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());

    /// Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
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

        cout << "it got here" << endl;
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

    /// Text area
    Text* StarbaseText=new Text(context_);
    Text* GalaxyText=new Text(context_);

    StarbaseText->SetName("StarbaseText");
    StarbaseText->SetText("| STARBASE |");

    GalaxyText->SetName("GalaxyText");
    GalaxyText->SetText("| GALAXY |");

    /// addlones
    /// Add the controls to the title bar
    MainTopBarMenuUIElement ->AddChild(StarbaseText);
    MainTopBarMenuUIElement->AddChild(GalaxyText);

    StarbaseText->SetPosition(Width/2+200,2);
    GalaxyText->SetPosition(Width/2+290,2);

    /// Apply styles
    StarbaseText->SetStyleAuto();
    GalaxyText->SetStyleAuto();
}

