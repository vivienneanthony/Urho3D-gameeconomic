#ifndef EXISTENCEAPP_INLINE
#define EXISTENCEAPP_INLINE
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

#include "../../Urho3D/Engine/Application.h"
#include "../../Urho3D/Graphics/Camera.h"
#include "../../Urho3D/Engine/Console.h"
#include "../../Urho3D/UI/Cursor.h"
#include "../../Urho3D/Engine/DebugHud.h"
#include "../../Urho3D/Engine/Engine.h"
#include "../../Urho3D/IO/FileSystem.h"
#include "../../Urho3D/Input/Input.h"
#include "../../Urho3D/Input/InputEvents.h"
#include "../../Urho3D/Graphics/Renderer.h"
#include "../../Urho3D/Resource/ResourceCache.h"
#include "../../Urho3D/Graphics/Graphics.h"
#include "../../Urho3D/Scene/Scene.h"
#include "../../Urho3D/Scene/SceneEvents.h"
#include "../../Urho3D/UI/UI.h"
#include "../../Urho3D/UI/Sprite.h"
#include "../../Urho3D/Graphics/Texture2D.h"
#include "../../Urho3D/Core/Timer.h"
#include "../../Urho3D/Resource/XMLFile.h"
#include "GameEconomicGameClient/GameEconomicGameClient.h"

inline GameEconomicApp::GameEconomicApp(Context* context) :
    Application(context),
    screenJoystickIndex_(M_MAX_UNSIGNED),
    screenJoystickSettingsIndex_(M_MAX_UNSIGNED),
    paused_(false)
{
}

void GameEconomicApp::Setup()
{
    // Modify engine startup parameters
    engineParameters_["WindowTitle"] = GetTypeName();
    engineParameters_["LogName"]     = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
    engineParameters_["FullScreen"]  = true;
    engineParameters_["Headless"]    = false;

    GameEconomicGameClientPtr=NULL;
}

void GameEconomicApp::Start()
{

    // Set custom window Title & Icon
    SetWindowTitleAndIcon();

    // Create console and debug HUD
    CreateConsoleAndDebugHud();

    // Subscribe scene update event
    SubscribeToEvent(E_SCENEUPDATE, HANDLER(GameEconomicApp, HandleSceneUpdate));
}

void GameEconomicApp::SetWindowTitleAndIcon()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    Image* icon = cache->GetResource<Image>("Textures/UrhoIcon.png");
    graphics->SetWindowIcon(icon);
    graphics->SetWindowTitle("GameEconomicGameClient Alpha 0.001");
}

void GameEconomicApp::CreateConsoleAndDebugHud()
{
    // Get default style
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    // Create console
    Console* console = engine_->CreateConsole();
    console->SetDefaultStyle(xmlFile);
    console->GetBackground()->SetOpacity(0.8f);

    // Create debug HUD.
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(xmlFile);
}


void GameEconomicApp::HandleSceneUpdate(StringHash eventType, VariantMap& eventData)
{
    /*// Move the camera by touch, if the camera node is initialized by descendant GameEconomicApp class
    if (touchEnabled_ && cameraNode_)
    {
        Input* input = GetSubsystem<Input>();
        for (unsigned i = 0; i < input->GetNumTouches(); ++i)
        {
            TouchState* state = input->GetTouch(i);
            if (!state->touchedElement_)    // Touch on empty space
            {
                if (state->delta_.x_ ||state->delta_.y_)
                {
                    Camera* camera = cameraNode_->GetComponent<Camera>();
                    if (!camera)
                        return;

                    Graphics* graphics = GetSubsystem<Graphics>();
                    yaw_ += TOUCH_SENSITIVITY * camera->GetFov() / graphics->GetHeight() * state->delta_.x_;
                    pitch_ += TOUCH_SENSITIVITY * camera->GetFov() / graphics->GetHeight() * state->delta_.y_;

                    // Construct new orientation for the camera scene node from yaw and pitch; roll is fixed to zero
                    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
                }
                else
                {
                    // Move the cursor to the touch position
                    Cursor* cursor = GetSubsystem<UI>()->GetCursor();
                    if (cursor && cursor->IsVisible())
                        cursor->SetPosition(state->position_);
                }
            }
        }
    }
    */
}
#endif

