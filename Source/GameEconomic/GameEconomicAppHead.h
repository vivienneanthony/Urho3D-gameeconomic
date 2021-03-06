#ifndef GAMEECONOMICAPPHEAD_H
#define GAMEECONOMICAPPHEAD_H

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

//#pragma once
#include <Urho3D/Urho3D.h>


#include "../../Urho3D/Engine/Application.h"
#include "GameEconomicComponents/GameStateHandlerComponent.h"


#include "GameEconomicGameClient/GameEconomicGameClient.h"


namespace Urho3D
{

class Node;
class Scene;
class Sprite;
}

/// fw declaration
class GameEconomicGameClient;
class GameEconomicGameClientStateSingleton;

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

const float TOUCH_SENSITIVITY = 2.0f;

/// Sample class, as framework for all SAMPLES.
///    - Initialization of the Urho3D engine (in Application class)
///    - Modify engine parameters for windowed mode and to show the class name as title
///    - Create Urho3D logo at screen
///    - Set custom window title and icon
///    - Create Console and Debug HUD, and use F1 and F2 key to toggle them
///    - Toggle rendering options from the keys 1-8
///    - Take screenshot with key 9
///    - Handle Esc key down to hide Console or exit application
///    - Init touch input on mobile platform using screen joysticks (patched for each individual sample)
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

    /// pointer
    GameEconomicGameClient * GameEconomicGameClientPtr;

protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const
    {
        return String::EMPTY;
    }
    /// Initialize touch input on mobile platform.
    void InitTouchInput();
    /// Control logo visibility.
    void SetLogoVisible(bool enable);


private:

    /// Set custom window Title & Icon
    inline void SetWindowTitleAndIcon();
    /// Create console and debug HUD.
    inline void CreateConsoleAndDebugHud();
    /// Handle scene update event to control camera's pitch and yaw for all SAMPLES.
    inline void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);

    /// Screen joystick index for navigational controls (mobile platforms only).
    unsigned screenJoystickIndex_;
    /// Screen joystick index for settings (mobile platforms only).
    unsigned screenJoystickSettingsIndex_;
    /// Pause flag.
    bool paused_;


};

#include "GameEconomicAppHead.inl"

#endif





