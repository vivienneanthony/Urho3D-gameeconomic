#ifndef GAMEECONOMICAPP_H
#define GAMEECONOMICAPP_H

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
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/IO/File.h"
#include "../../../Urho3D/IO/FileSystem.h"
#include "../../../Urho3D/Resource/XMLFile.h"
#include "../../../Urho3D/Resource/XMLElement.h"

#include <Urho3D/Engine/Application.h>



namespace Urho3D
{

class Node;
class Scene;
class Sprite;

}

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

const float TOUCH_SENSITIVITY = 2.0f;

class GameEconomicServer;

/// GameEconomicApp class, as framework for all GameEconomicApps.
///    - Initialization of the Urho3D engine (in Application class)
///    - Modify engine parameters for windowed mode and to show the class name as title
///    - Create Urho3D logo at screen
///    - Set custom window title and icon
///    - Create Console and Debug HUD, and use F1 and F2 key to toggle them
///    - Toggle rendering options from the keys 1-8
///    - Take screenshot with key 9
///    - Handle Esc key down to hide Console or exit application
///    - Init touch input on mobile platform using screen joysticks (patched for each individual GameEconomicApp)
class GameEconomicApp : public Application
{
    // Enable type information.
    OBJECT(GameEconomicApp);

public:
    /// Construct.
    GameEconomicApp(Context* context);

    /// Setup before engine initialization. Modifies the engine parameters.
    inline virtual void Setup();
    /// Setup after engine initialization. Creates the logo, console & debug HUD.
    inline virtual void Start();
    /// Cleanup after the main loop. Called by Application.
    inline virtual void Stop();

protected:
    /// Return XML patch instructions for screen joystick layout for a specific GameEconomicApp app, if any.
    virtual String GetScreenJoystickPatchString() const
    {
        return String::EMPTY;
    }
    /// Initialize touch input on mobile platform.
    inline void InitTouchInput();
    /// Control logo visibility.
    inline void SetLogoVisible(bool enable);

    /// Logo sprite.
    SharedPtr<Sprite> logoSprite_;
    /// Scene.
    SharedPtr<Scene> scene_;
    /// Camera scene node.
    SharedPtr<Node> cameraNode_;
    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;
    /// Flag to indicate whether touch input has been enabled.
    bool touchEnabled_;

private:
    /// Create logo.
    inline void CreateLogo();
    /// Set custom window Title & Icon
    inline void SetWindowTitleAndIcon();
    /// Create console and debug HUD.
    inline void CreateConsoleAndDebugHud();
    /// Handle key down event to process key controls common to all GameEconomicApps.
    inline void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    /// Handle scene update event to control camera's pitch and yaw for all GameEconomicApps.
    inline void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle touch begin event to initialize touch input on desktop platform.
    inline void HandleTouchBegin(StringHash eventType, VariantMap& eventData);

    /// Screen joystick index for navigational controls (mobile platforms only).
    unsigned screenJoystickIndex_;
    /// Screen joystick index for settings (mobile platforms only).
    unsigned screenJoystickSettingsIndex_;
    /// Pause flag.
    bool paused_;
};

#include "GameEconomicAppHeadless.inl"

#endif
