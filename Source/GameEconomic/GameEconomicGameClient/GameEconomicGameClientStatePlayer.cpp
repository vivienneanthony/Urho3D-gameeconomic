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


#include "../GameEconomicComponents/GameStateHandlerComponent.h"
#include "../GameEconomicComponents/GameStateEvents.h"
#include "../GameEconomicComponents/Player.h"

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
#include "GameEconomicGameClientStatePlayer.h"

#include "../TranslationTable.h"
#include "../ServerResponse.h"

#include "../../Urho3D/Engine/DebugHud.h"


#define DEFAULTSIZE 4096

using namespace std;
using namespace Urho3D;

class GameEconomicGameClient;

/// State Player Constructor
GameEconomicGameClientStatePlayer::GameEconomicGameClientStatePlayer(Context* context):
    GameEconomicGameClientStateSingleton (context)
    ,AllowClose(false)
    ,NewPlayer(NULL)
    ,Existence(NULL)
{
    /// Debug
    cout << "Debug: State Player Constructor" << endl;

    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();
    /// Set aApplication
    Existence = gamestatehandlercomponent_->GetApplication();

    /// Set UI State
    gamestatehandlercomponent_->SetUIState(UI_CHARACTERCREATIONINTERFACE );

    /// Debug
    cout << "Debug: State Player Constructor Test Value " << Existence->testvalue << endl;

    return;
}

/// State Player Desctructor
GameEconomicGameClientStatePlayer::~GameEconomicGameClientStatePlayer()
{
    /// Debug
    cout << "Debug: State Player Deconstructor" << endl;

    return;
}

/// State Player Enter
void GameEconomicGameClientStatePlayer::Enter()
{
    /// Debug
    cout << "Debug: State Player Enter" << endl;

    NewPlayer = new PlayerObject();

    /// enter
    SubscribeToEvent(N_SERVER_RESPONSE, HANDLER(GameEconomicGameClientStatePlayer, ServerResponseHandler));

    /// create UI
    Player();

    return;
}

/// State Player Exit
void GameEconomicGameClientStatePlayer::Exit()
{
    /// delete temp player
    delete NewPlayer;

    /// Debug
    cout << "Debug: State Player Exit" << endl;

    return;
}

/// State Player On Update
void GameEconomicGameClientStatePlayer::OnUpdate(StringHash eventType, VariantMap& eventData)
{
    /// Debug
    cout << "Debug: State Player OnUpdate" << endl;

    return;
}

void GameEconomicGameClientStatePlayer::SetParameter(String parameter_)
{
    /// Do Nothing
    return;
}

/// State Player UI
void GameEconomicGameClientStatePlayer::Player(void)
{
    UI * ui_ = GetSubsystem<UI>();

    /// Set variables
    bool CurrentStateIsLogin=true;

    /// Erase the old UI
    Existence->EraseUI();

    /// Create a new viewport
    CreatePlayerMainScreenViewport();

    /// Load the user interace
    CreatePlayerScreenUI();

    /// set visible window to player
    CreatePlayerSetVisible(VisibleWindow_Profile);

    /// Get root
    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Create random name for the name
    NewPlayer->Firstname=String(GenerateName(0,0).c_str());
    NewPlayer->Middlename=String(GenerateName(0,0).c_str());
    NewPlayer->Lastname=String(GenerateName(0,0).c_str());

    Text * NewFirstnameOriginal = (Text *) UIRoot->GetChild("NewFirstnameOriginal",true);
    NewFirstnameOriginal->SetVisible(false);
    NewFirstnameOriginal->SetText(NewPlayer->Firstname);

    Text * NewMiddlenameOriginal = (Text *) UIRoot->GetChild("NewMiddlenameOriginal",true);
    NewMiddlenameOriginal->SetVisible(false);
    NewMiddlenameOriginal->SetText(NewPlayer->Middlename);

    Text * NewLastnameOriginal= (Text *) UIRoot->GetChild("NewLastnameOriginal",true);
    NewLastnameOriginal->SetVisible(false);
    NewLastnameOriginal->SetText(NewPlayer->Lastname);

    LineEdit * NewFirstnameInput = (LineEdit *) UIRoot->GetChild("NewFirstnameInput",true);
    NewFirstnameInput->SetText(NewPlayer->Firstname);

    LineEdit * NewMiddlenameInput = (LineEdit *) UIRoot->GetChild("NewMiddlenameInput",true);
    NewMiddlenameInput->SetText(NewPlayer->Middlename);

    LineEdit * NewLastnameInput= (LineEdit *) UIRoot->GetChild("NewLastnameInput",true);
    NewLastnameInput->SetText(NewPlayer->Lastname);

    NewFirstnameInput ->SetStyleAuto();

    /// Upate
    Button * ProfileTitleButton = (Button *) UIRoot->GetChild("ProfileTitleButton",true);
    Button * GenderTitleButton = (Button *) UIRoot->GetChild("GenderTitleButton",true);
    Button * PersonalityTitleButton = (Button *) UIRoot->GetChild("PersonalityTitleButton",true);
    Button * RaceTitleButton = (Button *) UIRoot->GetChild("RaceTitleButton",true);
    Button * FactionTitleButton = (Button *) UIRoot->GetChild("FactionTitleButton",true);

    Button * CreateCharacterButton = (Button *) UIRoot->GetChild("CreateCharacterButton",true);

    ///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();

    /// updatelist in background
    CreatePlayerUpdateFactionsList();
    CreatePlayerUpdateAlienRaceList();

    /// listviews
    ListView * PersonalityListView = (ListView *) UIRoot->GetChild("NewPersonalityListView", true);
    ListView * GenderListView = (ListView *) UIRoot->GetChild("NewGenderListView", true);
    ListView * AlienRaceListView = (ListView *) UIRoot->GetChild("NewRaceListView", true);

    SubscribeToEvent(PersonalityListView,E_ITEMDOUBLECLICKED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUIHandleListViews));
    SubscribeToEvent(GenderListView,E_ITEMDOUBLECLICKED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUIHandleListViews));
    SubscribeToEvent(AlienRaceListView,E_ITEMDOUBLECLICKED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUIHandleListViews));

    /// get all tabs and assign the same handler
    SubscribeToEvent(ProfileTitleButton,E_RELEASED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUITabButtonPressed));
    SubscribeToEvent(GenderTitleButton,E_RELEASED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUITabButtonPressed));
    SubscribeToEvent(PersonalityTitleButton,E_RELEASED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUITabButtonPressed));
    SubscribeToEvent(FactionTitleButton,E_RELEASED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUITabButtonPressed));
    SubscribeToEvent(RaceTitleButton,E_RELEASED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerUITabButtonPressed));

    /// handle nameinput change
    SubscribeToEvent(NewFirstnameInput,E_TEXTFINISHED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerNameInputHandleChange));
    SubscribeToEvent(NewMiddlenameInput,E_TEXTFINISHED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerNameInputHandleChange));
    SubscribeToEvent(NewLastnameInput,E_TEXTFINISHED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerNameInputHandleChange));

    SubscribeToEvent(CreateCharacterButton,E_RELEASED,HANDLER(GameEconomicGameClientStatePlayer,CreatePlayerHandlePlayerCompleted));

    return;

}

/// Create a Player UI
void GameEconomicGameClientStatePlayer::CreatePlayerScreenUI(void)
{
    /// Get Needed SubSystems
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI * ui_ = GetSubsystem<UI>();

    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    unsigned int yposition=(height/2)-(600/2);

    Existence->loadHUDFile("Resources/UI/CreateNewCharacter.xml",32,yposition);

    unsigned int xposition=96+800;

    Existence->loadHUDFile("Resources/UI/CreateNewCharacterViewport.xml",xposition,yposition+128);

    /// Resize second window

    /// Get Listview
    Window * CreatePlayerViewportWindow = (Window *) UIRoot->GetChild("CreatePlayerViewportWindow", true);

    /// Resize and force to match - no matter the resolution
    CreatePlayerViewportWindow->SetMinSize(width-xposition, 600-128);
    CreatePlayerViewportWindow->SetMaxSize(width-xposition, 600-128);
    CreatePlayerViewportWindow->SetSize(width-xposition, 600-128);

    return;
}

/// Create Accooount UI  Handler
void GameEconomicGameClientStatePlayer::CreatePlayerUISelectPressed(StringHash eventType, VariantMap& eventData)
{

    return;
}

/// Tab Button pressed
void GameEconomicGameClientStatePlayer::CreatePlayerUITabButtonPressed(StringHash eventType, VariantMap& eventData)
{
    /// Resource
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Get needed info
    Button* Clicked = (Button *)(eventData[Released::P_ELEMENT].GetPtr());

    if(Clicked->GetName()==String("FactionTitleButton"))
    {
        CreatePlayerHandleFactionTabButton();
    }
    else if(Clicked->GetName()==String("PersonalityTitleButton"))
    {
        CreatePlayerHandlePersonalityTabButton();
    }
    if(Clicked->GetName()==String("ProfileTitleButton"))
    {
        CreatePlayerHandleProfileTabButton();
    }
    if(Clicked->GetName()==String("RaceTitleButton"))
    {
        CreatePlayerHandleRaceTabButton();

    }
    if(Clicked->GetName()==String("GenderTitleButton"))
    {
        CreatePlayerHandleGenderTabButton();
    }



    return;
}

/// Tab Button pressed
void GameEconomicGameClientStatePlayer::CreatePlayerUICreateCharacterPressed(StringHash eventType, VariantMap& eventData)
{

    return;
}

void GameEconomicGameClientStatePlayer::ServerResponseHandler(StringHash eventType, VariantMap& eventData)
{
    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    /// Take the frame time step, which is stored as a float
    unsigned int cmdType = eventData[ServerResponse::P_CMD].GetInt();
    String cmdArg = eventData[ServerResponse::P_ARG].GetString();

    /// if Account Authentication
    if(cmdType==ServerResponse_CreatePlayerResponse)
    {
        /// If unauthorized
        if(cmdArg==String("error"))
        {
            /// do nothing for now
        }
        else
        {
            /// Get request for new players
            String ServerRequest;
            ServerRequest.Append("requestplayers ");
            ServerRequest.Append(Existence->ThisAccount->UniqueID);

            VectorBuffer msg;
            msg.WriteString(ServerRequest);

            serverConnection->SendMessage(NetMessageRequest,true,true,msg,0);
        }
    }

    /// If the server sent new list of players everything is good switch to main screen
    if(cmdType==ServerResponse_SentAccountPlayers)
    {
        Existence->ThisAccountPlayerList.Empty();
        Existence->ThisAccountPlayerList = Existence->LoginGetAccountPlayersFromAuthorization(cmdArg);

        UnsubscribeFromAllEvents();

        /// Create a event to switch to the main screen
        VariantMap gamestatechange;
        gamestatechange[GameState::P_CMD] = GAME_STATE_MAINMENU;

        cout << "Debug: Attempt to send a state change" << endl;

        SendEvent(G_STATES_CHANGE,gamestatechange);
    }
    return;
}

/// Change Factions
void GameEconomicGameClientStatePlayer::CreatePlayerUpdateFactionsList(void)
{
    /// Get subsystem
    UI * ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get Listview
    ListView * FactionsListView = (ListView *) UIRoot->GetChild("NewFactionListView", true);

    /// if the two primary areas aren't visible then stop
    if(FactionsListView==NULL)
    {
        return;
    }

    /// Remove items from list
    FactionsListView->RemoveAllItems();

    ///cout << Existence->ThisFactionList.Size();

    /// Add new items
    for(unsigned int i=0; i<Existence->ThisFactionList.Size(); i++)
    {
        /// Create new Text
        Text * newItem = new Text(context_);

        newItem->SetEditable(true);
        newItem->SetEnabled(true);

        /// settext
        newItem->SetText(Existence->ThisFactionList.At(i).Name);
        newItem->SetName(Existence->ThisFactionList.At(i).UniqueID);

        /// Add each selection color
        newItem->SetSelectionColor (Color(0.0f,0.0f,0.5f));
        newItem->SetHoverColor (Color(0.0f,0.0f,1.0f));

        /// add to items
        FactionsListView->AddItem(newItem);

        newItem->SetStyleAuto();
    }



    /// force parameters
    FactionsListView->SetSelection(0);
    FactionsListView->SetMultiselect(false);
    FactionsListView->SetHighlightMode(HM_ALWAYS);
    FactionsListView->SetSelectOnClickEnd(false);
    FactionsListView->SetClearSelectionOnDefocus(false);

    return;
}

/// Change Factions
void GameEconomicGameClientStatePlayer::CreatePlayerUpdateAlienRaceList(void)
{
    /// Get subsystem
    UI * ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get Listview
    ListView * NewFactionListView = (ListView *) UIRoot->GetChild("NewFactionListView", true);
    ListView * NewRaceListView = (ListView *) UIRoot->GetChild("NewRaceListView", true);


    Text * SelectedFaction = (Text *) NewFactionListView -> GetSelectedItem();

    /// Get the name
    String RaceUniqueID = SelectedFaction->GetName();

    /// if the two primary areas aren't visible then stop
    if(NewFactionListView==NULL|| NewRaceListView==NULL|| SelectedFaction==NULL )
    {
        return;
    }


    /// Remove items from list
    NewRaceListView->RemoveAllItems();

    /// Add new items
    for(unsigned int i=0; i<Existence->ThisAlienRaceList.Size(); i++)
    {
        if(Existence->ThisAlienRaceList.At(i).AlignedFaction==RaceUniqueID)
        {
            /// Create new Text
            Text * newItem = new Text(context_);

            /// add to items
            NewRaceListView ->AddItem(newItem);

            /// settext
            newItem ->SetText(Existence->ThisAlienRaceList.At(i).Name);
            newItem ->SetName(String((unsigned int)TranslationTable::GetAlienRaceInt(Existence->ThisAlienRaceList.At(i).UniqueID)));

            ///cout << (unsigned int)TranslationTable::GetAlienRaceInt(Existence->ThisAlienRaceList.At(i).UniqueID) << endl;

            /// Add each selection color
            newItem->SetSelectionColor (Color(0.0f,0.0f,0.5f));
            newItem->SetHoverColor (Color(0.0f,0.0f,1.0f));

            newItem->SetStyleAuto();
        }
    }

    /// force parameters
    NewRaceListView->SetSelection(0);
    NewRaceListView->SetMultiselect(false);
    NewRaceListView->SetHighlightMode(HM_ALWAYS);
    NewRaceListView->SetSelectOnClickEnd(false);
    NewRaceListView->SetClearSelectionOnDefocus(false);

    return;
}


/// Setup viewport
void GameEconomicGameClientStatePlayer::CreatePlayerMainScreenViewport(void)
{
    /// Get Needed SubSystems
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

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


/// Update mainscreen
void GameEconomicGameClientStatePlayer::CreatePlayerUpdateMainMenuCharacterInfo(void)
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

    CharacterFirstnameElement_->SetText(NewPlayer->Firstname);
    CharacterMiddlenameElement_->SetText(NewPlayer->Middlename);
    CharacterLastnameElement_->SetText(NewPlayer->Lastname);

    /// match player alien race to configuration
    String PlayerAlienRace = TranslationTable::GetAlienRaceInt(NewPlayer->AlienRace);
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
    switch(NewPlayer->Gender)
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
    CharacterPersonalityElement_->SetText(TranslationTable::GetPersonalityInt(NewPlayer->PersonalityTrait));

    return;

}


/// GameEconomicStatePlayer
void GameEconomicGameClientStatePlayer::CreatePlayerSetVisible(VisibleWindow TabWindow)
{

    UI * ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get windows per race
    Window * PersonalityWindow = (Window *) UIRoot->GetChild("PersonalityWindow",true);
    Window * FactionWindow = (Window *) UIRoot->GetChild("FactionWindow",true);
    Window * GenderWindow = (Window *) UIRoot->GetChild("GenderWindow",true);
    Window * ProfileWindow = (Window *) UIRoot->GetChild("ProfileWindow",true);
    Window * RaceWindow = (Window *) UIRoot->GetChild("RaceWindow",true);

    /// turn off windows for now
    FactionWindow->SetVisible(false);
    ProfileWindow->SetVisible(false);
    RaceWindow->SetVisible(false);
    PersonalityWindow->SetVisible(false);
    GenderWindow->SetVisible(false);

    /// Set windows visibility
    switch(TabWindow)
    {
    case VisibleWindow_Faction:
        FactionWindow->SetVisible(true);
        break;
    case VisibleWindow_Profile:
        ProfileWindow->SetVisible(true);
        break;
    case VisibleWindow_Race:
        RaceWindow->SetVisible(true);
        break;
    case VisibleWindow_Personality:
        PersonalityWindow->SetVisible(true);
        break;
    case VisibleWindow_Gender:
        GenderWindow->SetVisible(true);
        break;
    }

    return;
}

/// Handle FactionTabButtion
void GameEconomicGameClientStatePlayer::CreatePlayerHandleFactionTabButton(void)
{
    /// Change table
    CreatePlayerSetVisible(VisibleWindow_Faction);

    ///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();

    /// Update Faction List
    CreatePlayerUpdateFactionsList();

    return;
}

/// Handle FactionTabButtion
void GameEconomicGameClientStatePlayer::CreatePlayerHandleProfileTabButton(void)
{
    /// Change table
    CreatePlayerSetVisible(VisibleWindow_Profile);

    ///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();

    /// Get Needed SubSystems
    ResourceCache* cache_ = GetSubsystem<ResourceCache>();
    Renderer* renderer_ = GetSubsystem<Renderer>();
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI* ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * UIRoot = ui_ -> GetRoot();

    Text * NewFirstnameOriginal = (Text *) UIRoot->GetChild("NewFirstnameOriginal",true);
    NewFirstnameOriginal->SetText(NewPlayer->Firstname);

    Text * NewMiddlenameOriginal = (Text *) UIRoot->GetChild("NewMiddlenameOriginal",true);
    NewMiddlenameOriginal->SetText(NewPlayer->Middlename);

    Text * NewLastnameOriginal= (Text *) UIRoot->GetChild("NewLastnameOriginal",true);
    NewLastnameOriginal->SetText(NewPlayer->Lastname);

    return;
}

/// Handle FactionTabButtion
void GameEconomicGameClientStatePlayer::CreatePlayerHandleRaceTabButton(void)
{
    /// Change table
    CreatePlayerSetVisible(VisibleWindow_Race);

    ///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();

    /// Create or update faction list
    CreatePlayerUpdateAlienRaceList();

    return;
}



/// Handle FactionTabButtion
void GameEconomicGameClientStatePlayer::CreatePlayerHandleGenderTabButton(void)
{
    /// Change table
    CreatePlayerSetVisible(VisibleWindow_Gender);

    ///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();

    /// Create or update gender list
    CreatePlayerUpdateGenderList();

    return;
}


/// Handle FactionTabButtion
void GameEconomicGameClientStatePlayer::CreatePlayerHandlePersonalityTabButton(void)
{
    /// Change table
    CreatePlayerSetVisible(VisibleWindow_Personality);

///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();


    CreatePlayerUpdatePersonalityList();

    return;
}


/// Change Personality
void GameEconomicGameClientStatePlayer::CreatePlayerUpdatePersonalityList(void)
{
    /// Get subsystem
    UI * ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get Listview
    ListView * PersonalityListView = (ListView *) UIRoot->GetChild("NewPersonalityListView", true);

    /// if the two primary areas aren't visible then stop
    if(PersonalityListView==NULL)
    {
        return;
    }

    /// If number of items is three then it was rendered already
    if(PersonalityListView->GetNumItems()!=PERSONALITYLIMIT)
    {

        /// Remove items from list
        PersonalityListView->RemoveAllItems();

        /// Add new items
        for(unsigned int i=0; i<PERSONALITYLIMIT; i++)
        {
            /// Create new Text
            Text * newItem = new Text(context_);

            newItem->SetEditable(true);
            newItem->SetEnabled(true);

            /// settext

            newItem->SetText(TranslationTable::GetPersonalityInt(i));
            newItem->SetName(String(i));

            /// Add each selection color
            newItem->SetSelectionColor (Color(0.0f,0.0f,0.5f));
            newItem->SetHoverColor (Color(0.0f,0.0f,1.0f));

            /// add to items
            PersonalityListView->AddItem(newItem);

            newItem->SetStyleAuto();
        }


        /// force parameters
        PersonalityListView->SetSelection(0);
        PersonalityListView->SetMultiselect(false);
        PersonalityListView->SetHighlightMode(HM_ALWAYS);
        PersonalityListView->SetSelectOnClickEnd(false);
        PersonalityListView->SetClearSelectionOnDefocus(false);
    }


    return;
}


/// Change Gender
void GameEconomicGameClientStatePlayer::CreatePlayerUpdateGenderList(void)
{
    /// Get subsystem
    UI * ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * UIRoot =  ui_ -> GetRoot();

    /// Get Listview
    ListView * GenderListView = (ListView *) UIRoot->GetChild("NewGenderListView", true);

    /// if the two primary areas aren't visible then stop
    if(GenderListView==NULL)
    {
        return;
    }

    /// If number of items is three then it was rendered already
    if(GenderListView->GetNumItems()!=3)
    {
        /// Remove items from list
        GenderListView->RemoveAllItems();

        Vector<String> Genders;

        Genders.Push(String("Male"));
        Genders.Push(String("Female"));
        Genders.Push(String("Multigender"));

        /// Add new items
        for(unsigned int i=0; i<Genders.Size(); i++)
        {
            /// Create new Text
            Text * newItem = new Text(context_);

            newItem->SetEditable(true);
            newItem->SetEnabled(true);

            /// settext
            newItem->SetText(Genders.At(i));
            newItem->SetName(String(i));

            /// Add each selection color
            newItem->SetSelectionColor (Color(0.0f,0.0f,0.5f));
            newItem->SetHoverColor (Color(0.0f,0.0f,1.0f));

            /// add to items
            GenderListView->AddItem(newItem);

            newItem->SetStyleAuto();
        }

        /// force parameters
        GenderListView->SetSelection(0);
        GenderListView->SetMultiselect(false);
        GenderListView->SetHighlightMode(HM_ALWAYS);
        GenderListView->SetSelectOnClickEnd(false);
        GenderListView->SetClearSelectionOnDefocus(false);
    }

    return;
}

/// Change Gender
void GameEconomicGameClientStatePlayer::CreatePlayerNameInputHandleChange(StringHash eventType, VariantMap& eventData)
{
    /// Get subsystem
    UI * ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * UIRoot =  ui_ -> GetRoot();


    /// Get needed info
    LineEdit* FieldChanged = (LineEdit *)(eventData[TextFinished::P_ELEMENT].GetPtr());
    String ChangedText = eventData[TextFinished::P_TEXT].GetString();

    bool hasNumber=false;

    /// if text is not empty - if a number do not allow
    if(!ChangedText.Empty())
    {
        for(unsigned char i='0'; i<'9'; i++)
            if(ChangedText.Contains(i))
            {
                hasNumber = true;
            }
    }

    /// if text is empty text name and return original - make so no db access breaking database
    if(ChangedText.Empty()||ChangedText.Contains('|')||ChangedText.Contains('"')||ChangedText.Contains('\'')||hasNumber)
    {
        if(FieldChanged->GetName()==String("NewFirstnameInput"))
        {
            Text * NewFirstnameOriginal = (Text *) UIRoot->GetChild("NewFirstnameOriginal", true);
            FieldChanged-> SetText(NewFirstnameOriginal->GetText());
        }
        if(FieldChanged->GetName()==String("NewMiddlenameInput"))
        {
            Text * NewMiddlenameOriginal = (Text *) UIRoot->GetChild("NewMiddlenameOriginal", true);
            FieldChanged-> SetText(NewMiddlenameOriginal->GetText());
        }
        if(FieldChanged->GetName()==String("NewLastnameInput"))
        {
            Text * NewLastnameOriginal = (Text *) UIRoot->GetChild("NewLastnameOriginal", true);
            FieldChanged-> SetText(NewLastnameOriginal->GetText());
        }

        return;
    }

    /// update original
    if(FieldChanged->GetName()==String("NewFirstnameInput"))
    {
        Text * NewFirstnameOriginal = (Text *) UIRoot->GetChild("NewFirstnameOriginal", true);
        NewFirstnameOriginal->SetText(ChangedText);
        NewPlayer->Firstname=ChangedText;
    }
    if(FieldChanged->GetName()==String("NewMiddlenameInput"))
    {
        Text * NewMiddlenameOriginal = (Text *) UIRoot->GetChild("NewMiddlenameOriginal", true);
        NewMiddlenameOriginal->SetText(ChangedText);
        NewPlayer->Middlename=ChangedText;
    }
    if(FieldChanged->GetName()==String("NewLastnameInput"))
    {
        Text * NewLastnameOriginal = (Text *) UIRoot->GetChild("NewLastnameOriginal", true);
        NewLastnameOriginal->SetText(ChangedText);
        NewPlayer->Lastname=ChangedText;
    }

    ///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();

    return;
}


/// create character and send server code
void GameEconomicGameClientStatePlayer::CreatePlayerHandlePlayerCompleted(StringHash eventType, VariantMap& eventData)
{
    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    /// create request
    String ServerRequest;

    /// create player then attach uniqueid
    ServerRequest.Append("createplayer ");
    ServerRequest.Append(Existence->ThisAccount->UniqueID);

    /// add each element
    ServerRequest.Append(" ");
    ServerRequest.Append(NewPlayer->Firstname);
    ServerRequest.Append(" ");
    ServerRequest.Append(NewPlayer->Middlename);
    ServerRequest.Append(" ");
    ServerRequest.Append(NewPlayer->Lastname);
    ServerRequest.Append(" ");
    ServerRequest.Append(String(NewPlayer->Level));
    ServerRequest.Append(" ");
    ServerRequest.Append(String(NewPlayer->AlienRace));
    ServerRequest.Append(" ");
    ServerRequest.Append(String("1"));
    ServerRequest.Append(" ");
    ServerRequest.Append(String(NewPlayer->Gender));
    ServerRequest.Append(" ");
    ServerRequest.Append(String(NewPlayer->PersonalityTrait));

    ///cout << ServerRequest.CString()<<endl;

    /// set up buffer
    VectorBuffer msg;
    msg.WriteString(ServerRequest);

    serverConnection->SendMessage(NetMessageRequest,true,true,msg,0);
}

/// Generate a character name
string GenerateName(char group, char subgroup)
{
    /// Create a string
    string temporaryname;

    /// Create a base an stem limit and vector
    unsigned int namebasesize=12;
    vector<string> namebase(namebasesize);

    unsigned int namestemsize=12;
    vector<string> namestem(namestemsize);

    /// Create a base and stem array
    string namebases[]= {"A","B","C","D","L'l","X","Jes","Nex","R","N","L","M"};
    string namestems[]= {"on","el","ul","axob","uc","ath","'rth","elys","iel","loe","mma","oe"};

    /// Copy base
    for(unsigned int idx=0; idx<namebasesize; idx++)
    {
        namebase.at(idx)=namebases[idx];
    }

    /// Copy stem
    for(unsigned int idx=0; idx<namestemsize; idx++)
    {
        namestem.at(idx)=namestems[idx];
    }

    /// Select random base and stem
    unsigned int randombase= rand()%namebasesize;
    unsigned int randomstemsamount = rand()%2;
    unsigned int randomstem;

    /// Create a name
    string generatedname;

    generatedname.append(namebase.at(randombase));

    /// Always add 1 stem
    randomstemsamount+=1;

    /// Add Stems to the name
    for(unsigned int count=0; count<randomstemsamount; count++)
    {
        randomstem= rand()%namestemsize;
        generatedname.append(namestem.at(randomstem));
    }

    return generatedname;
}


/// UIHandleCloseButton
void GameEconomicGameClientStatePlayer::CreatePlayerUIHandleListViews(StringHash eventType, VariantMap& eventData)
{
    ListView* SelectedListView = (ListView*)(eventData[ItemDoubleClicked::P_ELEMENT].GetPtr());
    unsigned int selection= eventData[ItemDoubleClicked::P_SELECTION].GetInt();

    Text * SelectedTextItem = (Text *)SelectedListView->GetItem(selection);

    if(SelectedListView->GetName()==String("NewRaceListView"))
    {
        NewPlayer->AlienRace= atoi(SelectedTextItem->GetName().CString());
    }
    if(SelectedListView->GetName()==String("NewPersonalityListView"))
    {
        NewPlayer->PersonalityTrait= atoi(SelectedTextItem->GetName().CString());
    }
    if(SelectedListView->GetName()==String("NewGenderListView"))
    {
        NewPlayer->Gender =atoi(SelectedTextItem->GetName().CString());
    }

    ///Update the character info
    CreatePlayerUpdateMainMenuCharacterInfo();


    return;
}
