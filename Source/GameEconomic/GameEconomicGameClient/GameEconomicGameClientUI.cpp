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
#include "../../../Urho3D/Graphics/Light.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Graphics/Renderer.h"
#include "../../../Urho3D/Graphics/Camera.h"
#include "../../../Urho3D/UI/Window.h"
#include "../../../Urho3D/UI/Button.h"
#include "../../../Urho3D/UI/LineEdit.h"
#include "../../../Urho3D/UI/UIElement.h"
#include "../../../Urho3D/Math/BoundingBox.h"
#include "../../../Urho3D/UI/UIEvents.h"
#include "../../../Urho3D/UI/View3D.h"
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

#include "../../Urho3D/Engine/DebugHud.h"

#include "GameEconomicGameClient.h"
#include "GameEconomicGameClientUI.h"
#include "GameEconomicGameClientStateGameMode.h"

#define DEFAULTSIZE 4096

using namespace std;
using namespace Urho3D;

/// Load a HUD file in a XML format in the file system
bool GameEconomicGameClient::loadHUDFile(const char * filename, const int positionx, const int positiony)
{
    /// Get resources
    ResourceCache * cache = GetSubsystem<ResourceCache>();
    FileSystem * filesystem = GetSubsystem<FileSystem>();
    UI* ui_ = GetSubsystem<UI>();

    /// get current root
    UIElement * RootUIElement = ui_->GetRoot();
    UIElement * HUDFileElement= new UIElement(context_);
    UIElement * playerInfoHudElement= new UIElement(context_);

    /// Force UIFile Load
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    /// Configure resources
    XMLElement hudElement;

    /// Configure string to Urho friendly
    String filenameHUD = String(filename);

    /// Load Resource
    if(XMLFile* hudFile= cache->GetResource<XMLFile>(filenameHUD))
    {

        /// Get root element XML
        hudElement =  hudFile->GetRoot();

        /// Add a min top bar
        HUDFileElement-> LoadXML(hudElement, style);

        /// Add a uielement for the bar
        RootUIElement -> AddChild(HUDFileElement);

        /// Position the window
        HUDFileElement -> SetPosition(positionx,positiony);

        HUDFileElement->SetStyleAuto();
    }

    return true;
}


/// Load a HUD file in a XML format in the file system

bool GameEconomicGameClient::loadUIXML(int windowtype, const int positionx, const int positiony, int selected)
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
    UIElement * HUDFileElement= new UIElement(context_);
    UIElement * playerInfoHudElement= new UIElement(context_);

    String filenameHUD;

    /// chose based on menu type
    /// chose based on menu type
    if(windowtype==UIMINIQUICKMENU)
    {
        filenameHUD.Append("Resources/UI/QuickMenu.xml");
    }

    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    /// Configure resources
    XMLElement hudElement;

    /// Load Resource
    XMLFile* hudFile= cache->GetResource<XMLFile>(filenameHUD);

    /// Get root element XML
    hudElement =  hudFile->GetRoot();

    /// Add a min top bar
    HUDFileElement-> LoadXML(hudElement, style);

    /// Add a uielement for the bar
    RootUIElement -> AddChild(HUDFileElement);

    /// Position the window
    HUDFileElement -> SetPosition(positionx,positiony);

    HUDFileElement->SetStyleAuto();

    HUDFileElement->BringToFront();

    /// Get the child and assign a close pressed
    Button * closebutton = (Button *) HUDFileElement -> GetChild("closeButton",true);

    /// If load is quickmenu on side assign buttons and move position  to far right
    if(windowtype==UIMINIQUICKMENU)
    {
        /// set position
        HUDFileElement -> SetPosition(width-18,100);

        /// get buttons assign a event
        Button * exitButton  = (Button *) HUDFileElement -> GetChild("ExitButton",true);

        exitButton->SetEnabled(true);

        SubscribeToEvent(exitButton, E_RELEASED, HANDLER(GameEconomicGameClient, QuickMenuPressed));
    }


    return true;
}


void GameEconomicGameClient::QuickMenuPressed(StringHash eventType, VariantMap& eventData)
{
    /// Get needed resources
    Renderer* renderer = GetSubsystem<Renderer>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui_ = GetSubsystem<UI>();
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// get the button that was clicked
    Button* clicked = static_cast<Button*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());

    String clickedButtonString(clicked->GetName());

    string clickedtext;
    unsigned int clickedbutton=0;

    stringstream ss(clickedButtonString.CString());
    ss >> clickedtext >> clickedbutton;

    /// If exit was clicked
    if (clickedtext=="ExitButton")
    {
        if(gamestatehandlercomponent_->GetDebugHudMode()==true)
        {
            gamestatehandlercomponent_->SetDebugHudMode(false);
            GetSubsystem<DebugHud>()->ToggleAll();
        }

        /// Create a event
        VariantMap gamemodechange;
        gamemodechange[GameModeChange::P_CMD] = GAMEMODE_EVENT_TERMINATE;

        cout << "Debug: Attempt to send a state change" << endl;

        this->SendEvent(G_MODE_CHANGE,gamemodechange);

        return;
    }


}
