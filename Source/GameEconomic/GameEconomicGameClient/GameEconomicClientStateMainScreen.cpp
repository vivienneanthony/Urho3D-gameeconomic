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

#include "GameEconomicGameClient.h"
#include "GameEconomicGameClientMainScreen.h"
#include "../TranslationTable.h"
#include "../../Urho3D/Engine/DebugHud.h"
#include "../ServerResponse.h"


#define DEFAULTSIZE 4096

using namespace std;
using namespace Urho3D;

class GameEconomicGameClient;

/// Constructor Destror
GameEconomicGameClientStateMainScreen::GameEconomicGameClientStateMainScreen(Urho3D::Context* context)
    :GameEconomicGameClientStateSingleton (context)
    ,Existence(baseclass)
{

    /// Debug
    cout << "Debug: State Main Screen Constructor" << endl;

    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Set Application
    Existence = gamestatehandlercomponent_->GetApplication();

    ///CHange Stater4==
    gamestatehandlercomponent_->SetUIState(UI_CHARACTERSELECTIONINTERFACE);

    /// Subscribe to


    return;
}

GameEconomicGameClientStateMainScreen::~GameEconomicGameClientStateMainScreen()
{
    /// Debug
    cout << "Debug: State Main Screen Deconstructor" << endl;

    return;
}

void GameEconomicGameClientStateMainScreen::Enter()
{
    /// Debug
    cout << "Debug: State Main Screen Enter" << endl;

    /// Clear Everything
    Existence->EraseUI();

    /// Clear screen
    Existence->SetupScreenViewportBlank();

    /// enter
    SubscribeToEvent(N_SERVER_RESPONSE, HANDLER(GameEconomicGameClientStateMainScreen, ServerResponseHandler));

    /// Load the user interace
    MainScreen();

    /// Get connection
    Network* Network_ = GetSubsystem<Network>();
    Connection* ServerConnection = Network_->GetServerConnection();

    /// create request
    String ServerRequest;
    ServerRequest.Append("requeststarbase ");
    ServerRequest.Append(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).UniqueID);

    ///cout << ServerRequest.CString() << endl;

    VectorBuffer msg;
    msg.WriteString(ServerRequest);

    ServerConnection->SendMessage(NetMessageRequest,true,true,msg,0);

    return;
}

void GameEconomicGameClientStateMainScreen::Exit()
{
    /// Debug
    cout << "Debug: State Main Screen Exit" << endl;

    /// Get subystem
    UI* ui = GetSubsystem<UI>();

    /// Clear the UI
    ui->Clear();

    ///UnsubscribeFromAllEvents();

    return;
}

void GameEconomicGameClientStateMainScreen::OnUpdate(StringHash eventType, VariantMap& eventData)
{
    /// Debug
    cout << "Debug: State Main Screen OnUpdate" << endl;

    return;
}

void GameEconomicGameClientStateMainScreen::SetParameter(String parameter_)
{


    /// Do Nothing
    return;
}

/// Main screen
void GameEconomicGameClientStateMainScreen::MainScreen(void)
{

    /// Get Needed SubSystems
    UI* ui = GetSubsystem<UI>();

    /// Setup interface
    MainScreenViewport();

    /// Create new UI
    MainScreenUI();

    /// Get needed info
    UIElement * uiroot_ = ui->GetRoot();

    ListView* MainMenuListView = (ListView*)uiroot_->GetChild("MainMenuListView", true);

    /// Set flags for listview
    MainMenuListView->SetHighlightMode(HM_ALWAYS);
    MainMenuListView->SetSelectOnClickEnd(false);
    MainMenuListView->SetClearSelectionOnDefocus(false);
    MainMenuListView->SetMultiselect (false);

/// loop through choices
    for(unsigned int i=0; i<4; i++)
    {
        /// Get attributes information and generate the menu
        Text * ItemAdd = new Text(context_);

        /// Create value
        ItemAdd->SetEditable(true);
        ItemAdd->SetEnabled(true);


        /// Setujp menu
        if(i==MainMenu_EnterStarbase)
        {
            /// Enter Starbase
            String UseText("Enter Starbase");
            String UseName("EnterStarbase");

            /// Use Text
            ItemAdd->SetName(UseName);
            ItemAdd->SetText(UseText);
        }
        else if (i==MainMenu_ChangePlayer)
        {
            /// Enter Starbase
            String UseText("Change Player");
            String UseName("ChangePlayer");

            /// Use Text
            ItemAdd->SetName(UseName);
            ItemAdd->SetText(UseText);
        }
        else if (i==MainMenu_CreatePlayer)
        {
            /// Enter Starbase
            String UseText("Create Player");
            String UseName("CreatePlayer");

            /// Use Text
            ItemAdd->SetName(UseName);
            ItemAdd->SetText(UseText);
        }
        else if(i==MainMenu_TerminateHangar)
        {
            /// Enter Starbase
            String UseText("Terminate Hangar");
            String UseName("TerminateHangar");

            /// Use Text
            ItemAdd->SetName(UseName);
            ItemAdd->SetText(UseText);
        }

        /// Add each selection color
        ItemAdd->SetSelectionColor (Color(0.0f,0.0f,0.5f));
        ItemAdd->SetHoverColor (Color(0.0f,0.0f,1.0f));

        /// Add item
        MainMenuListView -> AddItem(ItemAdd);

        ItemAdd->SetStyleAuto();
    }

    /// get items
    unsigned int MainMenuListViewItems=MainMenuListView->GetNumItems();

    UpdateMainMenuCharacterInfo();

    Text* StatusText = (Text*)uiroot_->GetChild("StatusText", true);

    /// set size
    StatusText->SetFixedSize(287,40);
    StatusText->SetMinSize(287,40);
    StatusText->SetMaxSize(287,40);
    StatusText->SetPosition(8,300);
    StatusText->SetEnabled(true);
    StatusText->SetVisible(true);
    StatusText->SetWordwrap(true);

    StatusText->SetStyleAuto();

    /// Attempt to attach to the list view
    SubscribeToEvent(MainMenuListView,E_ITEMCLICKED,HANDLER(GameEconomicGameClientStateMainScreen,HandleMenuPressed));

    return;
}

/// Main screen user interface function
void GameEconomicGameClientStateMainScreen::MainScreenUI(void)
{

    /// Get Needed SubSystems
    Graphics* graphics_ = GetSubsystem<Graphics>();

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    unsigned int yposition=(height/2)-(400/2);

    Existence->loadHUDFile("Resources/UI/MainMenu.xml",32,yposition);

    return;
}

void GameEconomicGameClientStateMainScreen::ServerResponseHandler(StringHash eventType, VariantMap& eventData)
{

    /// Get Needed SubSystems
    UI* ui = GetSubsystem<UI>();

    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    /// Get the root
    UIElement * uiRoot_ = ui->GetRoot();

    /// Take the frame time step, which is stored as a float
    unsigned int cmdType = eventData[ServerResponse::P_CMD].GetInt();
    String cmdArg = eventData[ServerResponse::P_ARG].GetString();

    /// if Account Authentication
    if(cmdType==ServerResponse_SentStarbase)
    {
        /// Get status text
        Text * StatusText = (Text *) uiRoot_->GetChild("StatusText",true);

        /// If unauthorized
        if(cmdArg==String("|0|0"))
        {
            /// If StatusText exist and starbase not loaded
            StatusText->SetText(String("Error. You are stationed at the homeworld. Enter Starbase to start."));


        }
        else
        {
            /// If StatusText exist and starbase loaded
            StatusText->SetText(String("Starbase loaded mission"));

        }
    }

    return;
}

/// Setup viewport
void GameEconomicGameClientStateMainScreen::MainScreenViewport(void)
{
    /// Get Needed SubSystems
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    /// Get the root
    UIElement * uiRoot_ = ui->GetRoot();

    Sprite* BackgroundBoxSprite = new Sprite(context_);
    BackgroundBoxSprite->SetName("BackgroundBoxSprite");

    /// Get letter box image
    Texture2D* texture = cache ->GetResource<Texture2D>("Resources/Textures/BrushedMetal.jpg");

    /// Set letter box properties
    BackgroundBoxSprite->SetTexture(texture); // Set texture
    BackgroundBoxSprite->SetSize(width,height);
    BackgroundBoxSprite->SetAlignment(HA_CENTER, VA_CENTER);

    /// Create letter box image to UIElement
    UIElement * BackgroundBoxUIElement = new UIElement(context_);
    BackgroundBoxUIElement->AddChild(BackgroundBoxSprite);

    /// Add letter box UIElement to ui
    uiRoot_->AddChild(BackgroundBoxUIElement);

    /// Set style of UIElements
    BackgroundBoxUIElement->SetOpacity(.8);

    BackgroundBoxSprite->SetStyleAuto();
    BackgroundBoxUIElement->SetStyleAuto();


    Sprite* LetterBoxSprite = new Sprite(context_);
    LetterBoxSprite->SetName("LetterBoxSprite");

    /// Get letter box image
    Texture2D* texture2 = cache ->GetResource<Texture2D>("Resources/Textures/LetterBox.png");

    /// Set letter box properties
    LetterBoxSprite->SetTexture(texture2); // Set texture
    LetterBoxSprite->SetSize(width,height);
    LetterBoxSprite->SetAlignment(HA_CENTER, VA_CENTER);

    /// Create letter box image to UIElement
    UIElement * LetterBoxUIElement = new UIElement(context_);
    LetterBoxUIElement->AddChild(LetterBoxSprite);

    /// Add letter box UIElement to ui
    uiRoot_->AddChild(LetterBoxUIElement);

    /// Set style of UIElements
    LetterBoxUIElement->SetOpacity(.4);

    LetterBoxSprite->SetStyleAuto();
    LetterBoxUIElement->SetStyleAuto();

    return;
}

/// Main screen user interface handle close pressed
void GameEconomicGameClientStateMainScreen::HandleMenuPressed(StringHash eventType, VariantMap& eventData)
{
    /// Resource
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    UI * ui_ = GetSubsystem<UI>();

    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get rendering window size as floats
    UIElement* CharacterPlayerLoaderUIElement = (UIElement *) UIRoot ->GetChild("CharacterPlayerLoaderUIElement",true);


    /// Get needed info
    Text* MainMenuListView = (Text*)(eventData[ItemClicked::P_ELEMENT].GetPtr());
    unsigned int selection= eventData[ItemClicked::P_SELECTION].GetInt();

    /// if terminate
    switch(selection)
    {
    case MainMenu_EnterStarbase:
    {
        /// Unsubscribe
        MainMenuListView->UnsubscribeFromAllEvents();

        /// Create a event
        VariantMap gamestatechange;
        gamestatechange[GameState::P_CMD] = GAME_STATE_PROGRESS;

        SendEvent(G_STATES_CHANGE,gamestatechange);
    }
    break;
    case MainMenu_TerminateHangar:
    {
        TerminateSkynet();
    }
    break;
    case MainMenu_CreatePlayer:
    {
        /// Unsubscribe
        MainMenuListView->UnsubscribeFromAllEvents();

        /// Create a event
        VariantMap gamestatechange;
        gamestatechange[GameState::P_CMD] = GAME_STATE_PLAYERCREATE;
        gamestatechange[GameState::P_ARG] = String("ALLOWCLOSE_YES");

        SendEvent(G_STATES_CHANGE,gamestatechange);
    }
    break;
    case MainMenu_ChangePlayer:
    {
        if(CharacterPlayerLoaderUIElement==NULL)
        {
            ChangePlayerUI();
        }
    }
    break;
    }

    return;
}

/// Exit main also disconnect
void GameEconomicGameClientStateMainScreen::TerminateSkynet(void)
{
    /// Get connection
    Network* Network_ = GetSubsystem<Network>();
    Connection* ServerConnection_ = Network_->GetServerConnection();

    /// If server connection
    if(ServerConnection_)
    {
        ServerConnection_->Disconnect();
    }

    /// Start shutdown
    exit(0);
}

/// Update mainscreen
void GameEconomicGameClientStateMainScreen::UpdateMainMenuCharacterInfo(void)
{
    /// Get Needed SubSystems
    ResourceCache* cache_ = GetSubsystem<ResourceCache>();
    Renderer* renderer_ = GetSubsystem<Renderer>();
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI* ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * uiroot_ = ui_ -> GetRoot();

    /// Get Character Window
    UIElement * CharacterWindow_ = uiroot_ -> GetChild("CharacterWindow",true);

    /// Find elements and insert
    Text * CharacterFirstnameElement_ = (Text *) CharacterWindow_->GetChild("CharacterFirstnameText",true);
    Text * CharacterMiddlenameElement_ = (Text *) CharacterWindow_->GetChild("CharacterMiddlenameText",true);
    Text * CharacterLastnameElement_ = (Text *) CharacterWindow_->GetChild("CharacterLastnameText",true);
    Text * CharacterRaceElement_ = (Text *) CharacterWindow_->GetChild("CharacterRaceText",true);
    Text * CharacterFactionElement_ = (Text *) CharacterWindow_->GetChild("CharacterFactionText",true);
    Text * CharacterGenderElement_ = (Text *) CharacterWindow_->GetChild("CharacterGenderText",true);
    Text * CharacterPersonalityElement_ = (Text *) CharacterWindow_->GetChild("CharacterPersonalityText",true);

    CharacterFirstnameElement_->SetText(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).Firstname);
    CharacterMiddlenameElement_->SetText(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).Middlename);
    CharacterLastnameElement_->SetText(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).Lastname);

    /// match player alien race to configuration
    String PlayerAlienRace = TranslationTable::GetAlienRaceInt(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).AlienRace);
    String PlayerAlienRaceName;
    String PlayerFactionName;
    String PlayerAlienRaceAlliance;

    /// loop through each alien race
    for(unsigned int i=0; i < Existence->ThisAlienRaceList.Size(); i++)
    {
        if(PlayerAlienRace==Existence->ThisAlienRaceList.At(i).UniqueID)
        {
            PlayerAlienRaceName = Existence->ThisAlienRaceList.At(i).Name;

            PlayerAlienRaceAlliance = Existence->ThisAlienRaceList.At(i).AlignedFaction;
            break;
        }
    }

    /// if alliance set;
    if(!PlayerAlienRaceAlliance.Empty())
    {
        /// loop through each alien race
        for(unsigned int i=0; i < Existence->ThisFactionList.Size(); i++)
        {
            if(PlayerAlienRaceAlliance==Existence->ThisFactionList.At(i).UniqueID)
            {
                PlayerFactionName  = Existence->ThisFactionList.At(i).Name;
            }
        }
    }

    /// Update race and faction
    CharacterRaceElement_->SetText(PlayerFactionName);
    CharacterFactionElement_->SetText(PlayerAlienRaceName);

    /// Change
    switch(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).Gender)
    {
    case 0:
        CharacterGenderElement_->SetText(String("Male"));
        break;
    case 1:
        CharacterGenderElement_->SetText(String("Female"));
        break;
    case 2:
        CharacterGenderElement_->SetText(String("Bigender"));
        break;
    }

    /// huh
    CharacterPersonalityElement_->SetText(TranslationTable::GetPersonalityInt(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).PersonalityTrait));

    return;
}

/// Change character player UI
void GameEconomicGameClientStateMainScreen::ChangePlayerUI(void)
{

    /// Get Needed SubSystems
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI * ui_ = GetSubsystem<UI>();

    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    unsigned int yposition=(height/2)-(600/2);

    /// Load xml file
    Existence->loadHUDFile("Resources/UI/CharacterPlayersLoader.xml",833,yposition);

    /// Get Listview
    ListView * CharacterPlayerListView = (ListView *) UIRoot ->GetChild("CharacterPlayerLoaderPlayersWindowListView",true);

    /// loop through each one and add
    for(unsigned int i=0; i<Existence->ThisAccountPlayerList.Size(); i++)
    {
        Text * newItem = new Text(context_);

        /// add to items
        CharacterPlayerListView ->AddItem(newItem);

        String PlayerName;

        PlayerName.Append(Existence->ThisAccountPlayerList.At(i).Firstname);
        PlayerName.Append(" ");
        PlayerName.Append(Existence->ThisAccountPlayerList.At(i).Middlename);
        PlayerName.Append(" ");
        PlayerName.Append(Existence->ThisAccountPlayerList.At(i).Lastname);

        /// settext
        newItem ->SetText(PlayerName);
        newItem ->SetName(String(i));

        /// Add each selection color
        newItem->SetSelectionColor (Color(0.0f,0.0f,0.5f));
        newItem->SetHoverColor (Color(0.0f,0.0f,1.0f));

        /// Set style auto
        newItem->SetStyleAuto();
    }

    /// Attempt to attach to the list view
    Button * CharacterPlayerLoaderWindowCloseButton = (Button *) UIRoot ->GetChild("CharacterPlayerLoaderWindowCloseButton",true);

    SubscribeToEvent(CharacterPlayerLoaderWindowCloseButton, E_RELEASED,HANDLER(GameEconomicGameClientStateMainScreen,ChangePlayerUIHandleCloseButton));
    SubscribeToEvent(CharacterPlayerListView, E_ITEMDOUBLECLICKED,HANDLER(GameEconomicGameClientStateMainScreen,ChangePlayerUIHandleSelection));

    return;
}

/// UIHandleCloseButton
void GameEconomicGameClientStateMainScreen::ChangePlayerUIHandleCloseButton(StringHash eventType, VariantMap& eventData)
{
    /// UI element
    UI * ui_ = GetSubsystem<UI>();

    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get rendering window size as floats
    UIElement* CharacterPlayerLoaderUIElement = (UIElement *) UIRoot ->GetChild("CharacterPlayerLoaderUIElement",true);

    /// Remove
    CharacterPlayerLoaderUIElement->Remove();




    return;
}

/// UIHandleCloseButton
void GameEconomicGameClientStateMainScreen::ChangePlayerUIHandleSelection(StringHash eventType, VariantMap& eventData)
{
    /// Resource
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    UI * ui_ = GetSubsystem<UI>();

    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get rendering window size as floats
    UIElement* CharacterPlayerLoaderUIElement = (UIElement *) UIRoot ->GetChild("CharacterPlayerLoaderUIElement",true);

    /// Get needed info
    Text* MainMenuListView = (Text*)(eventData[ItemDoubleClicked::P_ELEMENT].GetPtr());
    unsigned int selection= eventData[ItemDoubleClicked::P_SELECTION].GetInt();

    Existence-> CurrentPlayerFromList = selection;

    /// Get connection
    Network* Network_ = GetSubsystem<Network>();
    Connection* ServerConnection = Network_->GetServerConnection();

    /// create request
    String ServerRequest;
    ServerRequest.Append("requeststarbase ");
    ServerRequest.Append(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).UniqueID);

    ///cout << ServerRequest.CString() << endl;
    VectorBuffer msg;
    msg.WriteString(ServerRequest);

    ServerConnection->SendMessage(NetMessageRequest,true,true,msg,0);

/// create request
    String ServerRequest2;
    ServerRequest2.Append("requestplayerdetail ");
    ServerRequest2.Append(Existence->ThisAccountPlayerList.At(Existence->CurrentPlayerFromList).UniqueID);

    cout << ServerRequest2.CString() << endl;

    VectorBuffer msg2;
    msg2.WriteString(ServerRequest2);

    ServerConnection->SendMessage(NetMessageRequest,true,true,msg2,0);


    /// Remove
    CharacterPlayerLoaderUIElement->Remove();

    /// Update the character information screen
    UpdateMainMenuCharacterInfo();





    return;
}
