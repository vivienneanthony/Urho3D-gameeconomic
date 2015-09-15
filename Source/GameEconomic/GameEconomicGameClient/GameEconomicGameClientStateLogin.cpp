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
#include "../GameEconomicComponents/Accounts.h"
#include "../Networking.h"

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
#include "../ServerResponse.h"

#include "../../Urho3D/Engine/DebugHud.h"

#define DEFAULTSIZE 4096

using namespace std;
using namespace Urho3D;

class GameEconomicGameClient;

/// State Login Constructor
GameEconomicGameClientStateLogin::GameEconomicGameClientStateLogin(Context * context)
    :GameEconomicGameClientStateSingleton(context)
    ,Existence(NULL)
{
    /// Debug
    cout << "Debug: State Login Constructor" << endl;

    /// Get component
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Set aApplication
    Existence = gamestatehandlercomponent_->GetApplication();

    /// Set UI State
    gamestatehandlercomponent_->SetUIState(UI_LOGININTERFACE);

    /// Debug
    cout << "Debug: State Login Constructor Test Value " << Existence->testvalue << endl;

    return;
}

/// State Login Desconstructor
GameEconomicGameClientStateLogin::~GameEconomicGameClientStateLogin()
{
    /// Debug
    cout << "Debug: State Login Deconstructor" << endl;

    return;
}

/// State Login Enter
void GameEconomicGameClientStateLogin::Enter()
{

    /// Debug
    cout << "Debug: State Login Enter" << endl;

    /// Attach a listen
    SubscribeToEvent(N_SERVER_RESPONSE, HANDLER(GameEconomicGameClientStateLogin, ServerResponseHandler));

    /// Login function start when entered
    LoginScreenUI();

    ShowServerStatusUI();

    return;

}

/// State Login Exit
void GameEconomicGameClientStateLogin::Exit()
{
    ///UnsubscribeFromAllEvents();

    /// Debug
    cout << "Debug: State Login Exit" << endl;

    return;
}

/// State Login OnUpdate
void GameEconomicGameClientStateLogin::OnUpdate(StringHash eventType, VariantMap& eventData)
{
    /// Debug
    cout << "Debug: State Login OnUpdate" << endl;
}

void GameEconomicGameClientStateLogin::SetParameter(String parameter_)
{
    /// Do Nothing
    return;
}
/// State Login Screen UI
void GameEconomicGameClientStateLogin::LoginScreenUI(void)
{
    /// Debug
    cout << "Debug: State Login Create UI" << endl;

    /// Get all revelant resources
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics_= GetSubsystem<Graphics>();
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    /// Login screen - Create the Window and add it to the UI's root node
    Existence->window_= new Window(context_);
    Existence->uiRoot_->AddChild(Existence->window_);

    UIElement* usernameTextUIElement = new UIElement(context_);
    Text* usernameText = new Text(context_);
    LineEdit* usernameInput=new LineEdit(context_);
    UIElement* passwordTextUIElement = new UIElement(context_);
    Text* passwordText = new Text(context_);
    LineEdit* passwordInput=new LineEdit(context_);

    /// Set Window size and layout settings
    Existence->window_->SetMinSize(384, 192);
    Existence->window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    Existence->window_->SetAlignment(HA_CENTER, VA_CENTER);
    Existence->window_->SetName("LoginWindow");
    Existence->window_->SetMovable(false);
    Existence->window_->SetOpacity(.6);

    /// Create Window 'titlebar' container
    usernameTextUIElement ->SetMinSize(0,32);
    usernameTextUIElement ->SetVerticalAlignment(VA_TOP);
    usernameTextUIElement ->SetLayoutMode(LM_HORIZONTAL);

    usernameText->SetName("Login");
    usernameText->SetText("Username");

    /// Create Window 'titlebar' container
    passwordTextUIElement ->SetMinSize(0,32);
    passwordTextUIElement ->SetVerticalAlignment(VA_TOP);
    passwordTextUIElement ->SetLayoutMode(LM_HORIZONTAL);

    passwordText->SetName("Password");
    passwordText->SetText("Password");

    usernameInput->SetName("usernameInput");
    usernameInput->SetText("<Enter Email>");
    usernameInput->SetMaxLength(24);
    usernameInput->SetMinHeight(24);
    usernameInput->SetStyleAuto();

    passwordInput->SetName("passwordInput");
    passwordInput->SetText("<Enter Password>");
    passwordInput->SetMaxLength(24);
    passwordInput->SetMinHeight(24);
    passwordInput->SetStyleAuto();

    /// Add the controls to the title bar
    usernameTextUIElement->AddChild(usernameText);
    passwordTextUIElement->AddChild(passwordText);
    Existence->window_->AddChild(usernameTextUIElement);
    Existence->window_->AddChild(usernameInput);
    Existence->window_->AddChild(passwordTextUIElement);
    Existence->window_->AddChild(passwordInput);

    /// Declare buttons
    Button* loginButton = new Button(context_);
    Button* TerminateCommLink = new Button(context_);
    Text * TerminateCommLinkText = new Text(context_);

    /// Use login button
    loginButton->SetName("Login");
    loginButton->SetStyle("loginButton");

    TerminateCommLink->SetName("TerminateCommLink");

    /// Set parameters
    TerminateCommLink->SetFixedSize(220,18);
    TerminateCommLink->SetPosition((width/2)-(220/2), height-96);
    TerminateCommLink->SetOpacity(.2);

    TerminateCommLinkText->SetPosition(0,height-94);
    TerminateCommLinkText->	SetHorizontalAlignment(HA_CENTER);
    TerminateCommLinkText->SetOpacity(.8);
    TerminateCommLinkText->SetFont(String("Anonymous"),14);
    TerminateCommLinkText->SetText(String("TERMINATE SERVERLINK"));

    /// Change Opacity
    loginButton->SetOpacity(.9);

    /// Add login button
    Existence->window_->AddChild(loginButton);

    /// Add more text
    Existence->uiRoot_->AddChild(TerminateCommLink);
    Existence->uiRoot_->AddChild(TerminateCommLinkText);

    /// Apply styles
    Existence->window_->SetStyleAuto();
    usernameText->SetStyleAuto();
    passwordText->SetStyleAuto();
    TerminateCommLink->SetStyleAuto();
    TerminateCommLinkText->SetStyleAuto();

    /// Position login button in center
    loginButton->SetPosition((Existence->window_->GetWidth()/2)-(loginButton->GetWidth()/2),108);

    /// Attach handler based on new account - Temporary
    SubscribeToEvent(loginButton, E_RELEASED, HANDLER(GameEconomicGameClientStateLogin, LoginScreenUILoginHandleClosePressed));
    SubscribeToEvent(TerminateCommLink, E_RELEASED, HANDLER(GameEconomicGameClientStateLogin, TerminateCommLink));

    return;
}


/// State Login Screen UI
void GameEconomicGameClientStateLogin::ShowServerStatusUI(void)
{
    /// Debug
    cout << "Debug: State Login Create UI" << endl;

    /// Get all revelant resources
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics_= GetSubsystem<Graphics>();

    UIElement* NetworkStatusUIElement = new UIElement(context_);
    Text* NetworkStatusUpdateText = new Text(context_);

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    /// Add login UIElement
    Existence->uiRoot_->AddChild(NetworkStatusUIElement);

    /// Create Window 'titlebar' container
    NetworkStatusUIElement->SetMinSize(0,32);
    NetworkStatusUIElement->SetLayoutMode(LM_FREE);
    NetworkStatusUIElement->SetPosition((float)0, (float)height-32);

    NetworkStatusUpdateText->SetName("ServerStatusUpdateText");
    NetworkStatusUpdateText->SetText("(awaiting)");
    NetworkStatusUpdateText->SetPosition(float(width/2)-32,(float)0);

    /// Add the controls to the title bar
    NetworkStatusUIElement->AddChild(NetworkStatusUpdateText);

    NetworkStatusUIElement ->SetStyleAuto();
    NetworkStatusUpdateText ->SetStyleAuto();

    return;
}

/// Handlers for login screen (Handler) - Login Account
void GameEconomicGameClientStateLogin::LoginScreenUILoginHandleClosePressed(StringHash eventType, VariantMap& eventData)
{
    /// Get all revelant resources
    UI* ui_ = GetSubsystem<UI>();
    GameStateHandlerComponent * gamestatehandlercomponent_ = GetSubsystem<GameStateHandlerComponent>();

    /// Get root element for the UI
    UIElement * RootUIElement = ui_->GetRoot();

    LineEdit* UserNameInput = (LineEdit*)ui_->GetRoot()->GetChild("usernameInput", true);
    LineEdit* PasswordInput = (LineEdit*)ui_->GetRoot()->GetChild("passwordInput", true);

    /// Get input from window
    String Username = UserNameInput->GetText();
    String Password = PasswordInput->GetText();

    /// copy so network can access the information
    Existence->NetUserAuthenticateInfo.Username = Username;
    Existence->NetUserAuthenticateInfo.Password = Password;

    /// If password or username is empty
    if(Username.Empty()||Password.Empty())
    {
        return;
    }

    /// if network is offline
    if(Existence->NetStats==NetworkOffline)
    {
        return;
    }

    /// Connect to server with the current network configuration
    Existence->ConnectLogin(Existence->NetConfig);

    return;
}

/// Handlers for login screen (Handler) -New Account
void GameEconomicGameClientStateLogin::LoginScreenUINewAccountHandleClosePressed(StringHash eventType, VariantMap& eventData)
{

    return;
}


/// Handlers for login screen (Handler) -New Account
void GameEconomicGameClientStateLogin::TerminateCommLink(StringHash eventType, VariantMap& eventData)
{
    exit(0);

    return;
}


/// Handler for server response
void GameEconomicGameClientStateLogin::ServerResponseHandler(StringHash eventType, VariantMap& eventData)
{
    /// Get connection
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();


    /// Take the frame time step, which is stored as a float
    unsigned int cmdType = eventData[ServerResponse::P_CMD].GetInt();
    String cmdArg = eventData[ServerResponse::P_ARG].GetString();

    /// if Account Authentication
    if(cmdType==ServerResponse_AccountAuthentication)
    {
        /// If unauthorized
        if(cmdArg==String("|0|0")||cmdArg==String("Unauthorized")||cmdArg==String("error"))
        {
            /// Clear the UI
            Existence->EraseUI();

            SetupViewport();

            /// Reset everything
            serverConnection->Disconnect();

            Existence->NetOnline=NetDisconnected;

            /// restart
            Enter();

            return;
        }
        else
        {
            /// Clear Existence
            delete Existence -> ThisAccount;
            Existence->ThisAccountPlayerList.Clear();

            /// allocate new
            Existence -> ThisAccount = new AccountInformation();
            Existence -> CurrentPlayerFromList = 0;

            /// convert input string to player info

            if(cmdArg!=String("|0|0")||!cmdArg.Empty())
            {
                Existence -> ThisAccount = LoginGetPlayerAccountFromAuthorization(cmdArg);


                /// create request
                String ServerRequest;
                ServerRequest.Append("requestplayers ");
                ServerRequest.Append(Existence->ThisAccount->UniqueID);

                VectorBuffer msg;
                msg.WriteString(ServerRequest);

                serverConnection->SendMessage(NetMessageRequest,true,true,msg,0);
            }
            else
            {
                Existence -> ThisAccount = LoginGetPlayerAccountFromAuthorization(cmdArg);


                /// create request
                String ServerRequest;
                ServerRequest.Append("requestfactions");

                VectorBuffer msg;
                msg.WriteString(ServerRequest);

                serverConnection->SendMessage(NetMessageRequest,true,true,msg,0);

            }
        }

    }

    /// if on players received
    if(cmdType==ServerResponse_SentAccountPlayers)
    {

        Existence->ThisAccountPlayerList.Empty();
        Existence->ThisAccountPlayerList = LoginGetAccountPlayersFromAuthorization(cmdArg);


        /// Send request for alienraces
        String ServerRequest;
        ServerRequest = String("requestfactions");


        VectorBuffer msg1;
        msg1.WriteString(ServerRequest);

        serverConnection->SendMessage(NetMessageRequest,true,true,msg1,0);
    }

    /// if on players received
    if(cmdType==ServerResponse_SentFactions)
    {
        String ServerRequest;
        /// Send request for alienraces
        ServerRequest = String("requestalienraces");

        VectorBuffer msg1;
        msg1.WriteString(ServerRequest);

        serverConnection->SendMessage(NetMessageRequest,true,true,msg1,0);
    }

    /// if on players received
    if(cmdType==ServerResponse_SentAlienRaces)
    {
        /// Verify size
        if(Existence->ThisAccountPlayerList.Size()==0)
        {
            /// Create a event
            VariantMap gamestatechange;
            gamestatechange[GameState::P_CMD] = GAME_STATE_PLAYERCREATE;
            gamestatechange[GameState::P_ARG] = String("ALLOWCLOSE_NO");

            cout << "Debug: Attempt to send a state change" << endl;

            this->SendEvent(G_STATES_CHANGE,gamestatechange);

        }
        else
        {

            /// Create a event
            VariantMap gamestatechange;
            gamestatechange[GameState::P_CMD] = GAME_STATE_MAINMENU;

            cout << "Debug: Attempt to send a state change" << endl;

            this->SendEvent(G_STATES_CHANGE,gamestatechange);
        }
    }
}

/// Progress window UI
void GameEconomicGameClientStateLogin::LoginProgressUI(void)
{
    /// Get Needed SubSystems
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui_ = GetSubsystem<UI>();

    /// Get root
    UIElement * progressuiRoot_ = ui_-> GetRoot();

    /// Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

    /// Create the Window and add it to the UI's root node
    Window * progressWindow_= new Window(context_);

    UIElement* titleBar = new UIElement(context_);
    Text* windowTitle = new Text(context_);
    Text* progressText=new Text(context_);
    Button* continueButton = new Button(context_);

    /// Set Window size and layout settings
    progressWindow_->SetMinSize(384, 192);
    progressWindow_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    progressWindow_->SetAlignment(HA_CENTER, VA_CENTER);
    progressWindow_->SetName("LoginWindow");
    progressWindow_->SetMovable(false);
    progressWindow_->SetOpacity(.6);

    /// Create Window 'titlebar' container
    titleBar->SetMinSize(0,32);
    titleBar->SetVerticalAlignment(VA_TOP);
    titleBar->SetLayoutMode(LM_HORIZONTAL);

    windowTitle->SetName("Login");
    windowTitle->SetText("Accessing Server");

    progressText->SetName("progressText");
    progressText->SetText("Loading ghost characters states... ");

    /// addlones
    continueButton->SetName("Login");
    continueButton->SetStyle("continueButton");

    /// Add the controls to the title bar
    titleBar->AddChild(windowTitle);
    progressWindow_->AddChild(titleBar);
    progressWindow_->AddChild(progressText);
    progressWindow_->AddChild(continueButton);

    progressuiRoot_ -> AddChild(progressWindow_);

    /// Apply styles
    progressWindow_->SetStyleAuto();
    progressText->SetStyleAuto();
    windowTitle->SetStyleAuto();

    return;
}

/// Progress window UI
void GameEconomicGameClientStateLogin::SetupViewport(void)
{

    /// Get Needed SubSystems
    ResourceCache* cache_ = GetSubsystem<ResourceCache>();
    Renderer* renderer_ = GetSubsystem<Renderer>();
    Graphics* graphics_ = GetSubsystem<Graphics>();
    UI* ui_ = GetSubsystem<UI>();

    /// Change UI
    UIElement * uiRoot_ = ui_->GetRoot();

    BorderImage * Splash = (BorderImage * ) uiRoot_ -> GetChild("Splash", true);

    if(Splash!=NULL)
    {
        /// Remove splash
        Splash -> Remove();
    }

    /// Get rendering window size as floats

    /// Get rendering window size as floats
    float width = (float)graphics_->GetWidth();
    float height = (float)graphics_->GetHeight();

    /// Create LetterBox Sprite
    Sprite* LetterBoxSprite = new Sprite(context_);
    LetterBoxSprite->SetName("LetterBoxSprite");

    /// Get letter box image
    Texture2D* texture = cache_ ->GetResource<Texture2D>("Resources/Textures/LetterBox.png");

    /// Set letter box properties
    LetterBoxSprite->SetTexture(texture); // Set texture
    LetterBoxSprite->SetSize(width,height);
    LetterBoxSprite->SetAlignment(HA_CENTER, VA_CENTER);

    /// Create letter box image to UIElement
    UIElement * LetterBoxUIElement = new UIElement(context_);
    LetterBoxUIElement->AddChild(LetterBoxSprite);

    /// Add letter box UIElement to ui
    uiRoot_->AddChild(LetterBoxUIElement);

    /// Set style of UIElements
    LetterBoxUIElement->SetOpacity(.8);

    LetterBoxSprite->SetStyleAuto();
    LetterBoxUIElement->SetStyleAuto();

    /// Create HangarsSymbolSmall Sprite
    Sprite* HangarsSymbolSmallSprite = new Sprite(context_);
    HangarsSymbolSmallSprite->SetName("HangarsSymbolSmallSprite");

    /// Get letter box image
    Texture2D* HangarsSymbolTexture = cache_ ->GetResource<Texture2D>("Resources/Textures/HangarsSymbolSmall.png");

    /// Set letter box properties
    HangarsSymbolSmallSprite->SetTexture(HangarsSymbolTexture); // Set texture
    HangarsSymbolSmallSprite->SetSize(HangarsSymbolTexture->GetWidth()/2,HangarsSymbolTexture->GetHeight()/2);
    HangarsSymbolSmallSprite->SetAlignment(HA_LEFT, VA_TOP);

    /// Create letter box image to UIElement
    UIElement * HangarsSymbolSmallUIElement = new UIElement(context_);
    HangarsSymbolSmallUIElement->AddChild(HangarsSymbolSmallSprite);

    /// Add letter box UIElement to ui
    uiRoot_->AddChild(HangarsSymbolSmallUIElement);

    /// Set style of UIElements
    HangarsSymbolSmallUIElement->SetOpacity(.8);
    HangarsSymbolSmallUIElement->SetPosition((width/2)-((HangarsSymbolTexture->GetWidth()/2)/2),250);

    HangarsSymbolSmallSprite->SetStyleAuto();
    HangarsSymbolSmallUIElement->SetStyleAuto();

    /// Load fonts
    Font * Mionta = cache_ ->GetResource<Font>("Resources/Fonts/mionta.ttf");
    Font * Neuton = cache_ ->GetResource<Font>("Resources/Fonts/Neuton-SC-Light.ttf");

    /// Create logo text
    Text * LogoText = new Text(context_);
    LogoText -> SetTextAlignment(HA_CENTER);
    LogoText -> SetFont(Mionta,32);
    LogoText -> SetText("HANGARS");
    LogoText -> SetColor(Color(.9f,.9f,.9f));

    /// Create LetterBox UI Element
    UIElement * LogoTextUIElement = new UIElement(context_);

    LogoTextUIElement->AddChild(LogoText);

    /// Add to UI
    uiRoot_->AddChild(LogoTextUIElement);

    ///TitleText->SetStyleAuto();
    LogoTextUIElement->SetStyleAuto();

    /// Move text to a position
    LogoTextUIElement->SetPosition((width/2)-240,200);

    return;
}

/// Get account information
AccountInformation * GameEconomicGameClientStateLogin::LoginGetPlayerAccountFromAuthorization(String ServerString)
{
    /// Change convert
    String TempString=ServerString;
    Vector <String> ServerStringSplit;

    /// Create a pointer
    AccountInformation * ReturnAccountInformation  = new AccountInformation();

    /// Replace | to a space
    TempString.Replace ('|', ' ', false);

    /// Split the string
    ServerStringSplit = TempString.Split(' ');

    if(ServerStringSplit.At(1)==String("0"))
    {
        return NULL;
    }

    /// copy data
    ReturnAccountInformation->Username=ServerStringSplit.At(3);
    ReturnAccountInformation->Firstname=ServerStringSplit.At(4);
    ReturnAccountInformation->Middlename=ServerStringSplit.At(5);
    ReturnAccountInformation->Lastname=ServerStringSplit.At(6);
    ReturnAccountInformation->Email.Clear();
    ReturnAccountInformation->Password.Clear();
    ReturnAccountInformation->LastLogin=0;
    ReturnAccountInformation->UniqueID=ServerStringSplit.At(12);

    return ReturnAccountInformation;
}


/// Get players from authorization
Vector<PlayerList> GameEconomicGameClientStateLogin::LoginGetAccountPlayersFromAuthorization(String ServerString)
{

    /// Change convert
    String TempString=ServerString;
    Vector <String> ServerStringSplit;

    /// Replace | to a space
    TempString.Replace ('|', ' ', false);

    /// Split the string
    ServerStringSplit = TempString.Split(' ');

    /// Create a empty list for now
    Vector<PlayerList> temporaryList;

    /// return blank list
    if(ServerStringSplit.At(1)==String("0"))
    {
        return temporaryList;
    }

    /// Converstion here
    unsigned int NumberCols=atoi(ServerStringSplit.At(0).CString());
    unsigned int NumberRows=atoi(ServerStringSplit.At(1).CString());

    for(unsigned int i=0; i<NumberRows; i++)
    {
        unsigned int index=(i*NumberCols)+2;

        /// Create temporary Player
        PlayerList temporaryPlayer;

        /// Copy unique ID
        temporaryPlayer.Firstname = ServerStringSplit.At(index);
        temporaryPlayer.Middlename = ServerStringSplit.At(index+1);
        temporaryPlayer.Lastname = ServerStringSplit.At(index+2);
        temporaryPlayer.UniqueID = ServerStringSplit.At(index+3);
        temporaryPlayer.Level = atoi(ServerStringSplit.At(index+4).CString());
        temporaryPlayer.AlienRace = atoi(ServerStringSplit.At(index+5).CString());
        temporaryPlayer.AlienRaceAllianceAligned = atoi(ServerStringSplit.At(index+6).CString());
        temporaryPlayer.Gender = atoi(ServerStringSplit.At(index+7).CString());
        temporaryPlayer.PersonalityTrait = atoi(ServerStringSplit.At(index+8).CString());

        /// Append temporaryplayer
        temporaryList.Push(temporaryPlayer);
    }

    return temporaryList;
}
