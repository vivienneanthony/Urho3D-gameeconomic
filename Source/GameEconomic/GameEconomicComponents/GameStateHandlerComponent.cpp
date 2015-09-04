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


#include "GameStateEvents.h"
#include "Accounts.h"

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

#include "../GameEconomicGameClient/GameEconomicGameClient.h"
#include "GameStateHandlerComponent.h"


#include "../../Urho3D/Engine/DebugHud.h"


using namespace Urho3D;

using namespace std;


/// Game State Handler Coponent
GameStateHandlerComponent::GameStateHandlerComponent(Context* context) :
    LogicComponent (context)
    ,GameStateHandlerApplication(NULL)
{

    /// Debug Info
    cout << "Debug: Game State Handler Component Constructor - context " << &context << endl;

    /// constructor
    /// Debug
    cout << "Debug: Game State Handler OnStateChange subscribetoevent" << endl;

    SubscribeToEvent(G_STATES_CHANGE, HANDLER(GameStateHandlerComponent, onStateChange));

}


/// Game State Component Deconstructor
GameStateHandlerComponent::~GameStateHandlerComponent()
{
    /// Debug Info
    cout << "Debug: Game State Handler Component Deconstructor " << endl;
}


/// Register Subsystem
void GameStateHandlerComponent::RegisterNewSubsystem(Urho3D::Context* context)
{
    context -> RegisterSubsystem(new GameStateHandlerComponent(context));

    /// Debug Info
    cout << "Debug: Game State Handler RegisterNewSystem " << &context << endl;

    return;
}

void GameStateHandlerComponent::RegisterGameStates(Context* context)
{
    /// .... all states here
    context->RegisterFactory<GameEconomicGameClientStateSingleton>();
    context->RegisterFactory<GameEconomicGameClientStateLogin>();
    context->RegisterFactory<GameEconomicGameClientStateSplash>();
    context->RegisterFactory<GameEconomicGameClientStateMainScreen>();
    context->RegisterFactory<GameEconomicGameClientStatePlayer>();
    context->RegisterFactory<GameEconomicGameClientStateProgress>();
    context->RegisterFactory<GameEconomicGameClientStateGameMode>();

    /// Debug Info
    cout << "Debug: Game State Handler RegisterGameStates " << &context << endl;

}


/// try to test the state
void GameStateHandlerComponent::SetApplication(SharedPtr <GameEconomicGameClient> temp)
{

    GameStateHandlerApplication = temp;

    return;
}


/// try to test the state
void GameStateHandlerComponent::Start(void)
{
    /// Start
    GameEconomicGameClientStateSingleton *  gameState = new GameEconomicGameClientStateSplash(context_);

    gameState->Enter();

    return;
}

/// Game Sate Handler Changer State
void GameStateHandlerComponent::createState(String newState, Urho3D::VariantMap& eventData)
{

    /// Debug Info
    cout << "Debug: Game State Handler Component createState" << endl;

    /// Create a blank pointer
    GameEconomicGameClientStateSingleton * newgameState=NULL;

    /// Switch
    if (String(GameEconomicGameClientStateLogin::GetTypeNameStatic())==newState)
    {
        /// change to that state
        cout << "Debug: Create Login Progress Called" << endl;;

        state=GameEconomicGameClientStateLogin::GetTypeNameStatic();

        /// change to that state
        GameEconomicGameClientStateSingleton *  newgameState = new GameEconomicGameClientStateLogin(context_);

        /// delete old state
        delete gameState;
        gameState=newgameState;
        gameState->Enter();
    }
    else if (String(GameEconomicGameClientStateSplash::GetTypeNameStatic())==newState)
    {
        /// change to that state
        cout << "Debug: Create Splash Called" << endl;

        state=GameEconomicGameClientStateSplash::GetTypeNameStatic();

        GameEconomicGameClientStateSingleton *  newgameState = new GameEconomicGameClientStateSplash(context_);

        /// delete old state
        delete  gameState;
        gameState=newgameState;
        gameState->Enter();

    }
    else if (String(GameEconomicGameClientStatePlayer::GetTypeNameStatic())==newState)
    {
        /// change to that state
        cout << "Debug: Create Splash Called" << endl;

        state=GameEconomicGameClientStatePlayer::GetTypeNameStatic();

        GameEconomicGameClientStateSingleton *  newgameState = new GameEconomicGameClientStatePlayer(context_);

        /// delete old state
        delete  gameState;
        gameState=newgameState;
        gameState->Enter();

    }
    else if (String(GameEconomicGameClientStateProgress::GetTypeNameStatic())==newState)
    {
        /// change to that state
        cout << "Debug: Create Splash Called" << endl;

        state=GameEconomicGameClientStateProgress::GetTypeNameStatic();

        GameEconomicGameClientStateSingleton *  newgameState = new GameEconomicGameClientStateProgress(context_);

        /// delete old state
        delete  gameState;
        gameState=newgameState;
        gameState->Enter();

    }
    else if (String(GameEconomicGameClientStateMainScreen::GetTypeNameStatic())==newState)
    {
        /// change to that state
        cout << "Debug: Create MainScreen Called" << endl;

        state=GameEconomicGameClientStateMainScreen::GetTypeNameStatic();

        GameEconomicGameClientStateSingleton *  newgameState = new GameEconomicGameClientStateMainScreen(context_);

        /// delete old state
        delete  gameState;


        String cmdArg = eventData[GameState::P_ARG].GetString();

        cout << cmdArg.CString() << endl;

        gameState=newgameState;
        gameState->Enter();

    }
    else if (String(GameEconomicGameClientStateGameMode::GetTypeNameStatic())==newState)
    {
        /// change to that state
        cout << "Debug: Create GameMode Called" << endl;

        state=GameEconomicGameClientStateGameMode::GetTypeNameStatic();

        GameEconomicGameClientStateSingleton *  newgameState = new GameEconomicGameClientStateGameMode(context_);

        /// delete old state
        delete  gameState;


        String cmdArg = eventData[GameState::P_ARG].GetString();

        cout << cmdArg.CString() << endl;

        gameState=newgameState;
        gameState->Enter();

    }

    return;
}

/// onSteChangeHandler
void GameStateHandlerComponent::onStateChange( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData )
{

    /// Debug Info
    cout << "Debug: Game State Handler Component onStateChange called" << endl;

    /// intercept state event
    GameStates newState=  static_cast<GameStates>(eventData[GameState::P_CMD].GetInt());

    switch (newState)
    {
    case  GAME_STATE_LOGIN: //called from intro GameIntroSample
        /// exit out
        if(gameState!=NULL)
        {
            cout<< "gameState->Exit();" << endl;

            gameState->Exit();
        };
        /// create a new state
        createState(GameEconomicGameClientStateLogin::GetTypeNameStatic(),eventData);
        break;

    case  GAME_STATE_SPLASH: //called from intro GameIntroSample
        /// exit out
        if(gameState!=NULL)
        {
            gameState->Exit();
        };
        /// create a new state
        createState(GameEconomicGameClientStateSplash::GetTypeNameStatic(),eventData);
        break;
    case  GAME_STATE_PLAYERCREATE: //called from intro GameIntroSample
        /// exit out
        if(gameState!=NULL)
        {
            gameState->Exit();
        };
        /// create a new state
        createState(GameEconomicGameClientStatePlayer::GetTypeNameStatic(),eventData);
        break;

    case  GAME_STATE_MAINMENU: //called from intro GameIntroSample

        /// exit out
        if(gameState!=NULL)
        {
            gameState->Exit();
        };
        /// create a new state
        createState(GameEconomicGameClientStateMainScreen::GetTypeNameStatic(),eventData);
        break;

    case  GAME_STATE_PROGRESS: //called from intro GameIntroSample

        /// exit out
        if(gameState!=NULL)
        {
            gameState->Exit();
        };
        /// create a new state
        createState(GameEconomicGameClientStateProgress::GetTypeNameStatic(),eventData);
        break;
    case  GAME_STATE_GAMEMODE: //called from intro GameIntroSample

        /// exit out
        if(gameState!=NULL)
        {
            gameState->Exit();
        };
        /// create a new state
        createState(GameEconomicGameClientStateGameMode::GetTypeNameStatic(),eventData);
        break;
    default:
        cout << "Debug: Unkown State " << newState;
        break;
    }

    return;
}

/// save state
String GameStateHandlerComponent::GetCurrentState(void)
{
    return state;
}


/// Functions for states
int GameStateHandlerComponent::GetConsoleState(void)
{
    int flag;

    flag = consolestate;

    return flag;
}


int GameStateHandlerComponent::SetConsoleState(int flag)
{

    consolestate=flag;

    return 1;
}

int GameStateHandlerComponent::GetUIState(void)
{
    int flag;
    flag=uistate;

    return flag;
}

int GameStateHandlerComponent::SetUIState(int flag)
{

    uistate = flag;

    return 1;
}

int GameStateHandlerComponent::GetCameraMode(void)
{
    int flag;

    flag = cameramode;

    return flag;;
}

int GameStateHandlerComponent::SetCameraMode(int flag)
{

    cameramode = flag;

    return 1;
}

int GameStateHandlerComponent::GetDebugHudMode(void)
{
    int flag;

    flag = debughud;

    return flag;;
}

int GameStateHandlerComponent::SetDebugHudMode(int flag)
{

    debughud = flag;

    return 1;
}

/// try to test the state
SharedPtr<GameEconomicGameClient> GameStateHandlerComponent::GetApplication(void)
{
    return GameStateHandlerApplication;
}

