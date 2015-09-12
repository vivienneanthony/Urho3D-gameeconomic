#ifndef GAMEECONOMICGAMECLIENT_H
#define GAMEECONOMICGAMECLIENT_H

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

#define DISPLAYMESH_MUILTIPLECHARACTER 1
#define DISPLAYMESH_SINGLECHARACTER 2

#define UIBLANK 0
#define UIMAIN  1

#define CHARACTERMAINSCENE          0
#define CHARACTERUISCENE            1

#define CAMERAORIENTATIONROTATEYAW 1
#define CAMERAORIENTATIONROTATEPITCH 1


#include <vector>
#include <time.h>
#include <vector>
#include <time.h>

#include "../GameEconomicAppHead.h"
#include "../GameEconomicComponents/Accounts.h"
#include "../GameEconomicComponents/Player.h"

#include "../GameEconomicComponents/GameStateEvents.h"
#include "../GameEconomicComponents/GameStateHandlerComponent.h"

#include "../Networking.h"
#include "../Platform.h"
#include "../Accounts.h"
#include "../Starbase.h"
#include "../Configuration.h"


#include "../GameEconomicServer/GameEconomicServerDatabaseGeneral.h"
#include "../GameEconomicComponents/ResourceComponent.h"
#include "../GameEconomicComponents/ResourceManager.h"
#include "GameEconomicGameClientStatePlayer.h"
#include "GameEconomicGameClientUI.h"
#include "GameEconomicGameClientStateProgress.h"


string ConvertUIntToString(unsigned int val);

/// This first example, maintaining tradition, prints a "Hello World" message.
/// Furthermore it shows:
///     - Using the Sample / Application classes, which initialize the Urho3D engine and run the main loop
///     - Adding a Text element to the graphical user interface
///     - Subscribing to and handling of update events

using namespace Urho3D;
using namespace std;

class GameEconomicGameClient : public GameEconomicApp
{
    OBJECT(GameEconomicGameClient);

public:
    /// Mention friend classes
    friend class GameEconomicGameClientStateSingleton;
    friend class GameEconomicGameClientStateLogin;
    friend class GameEconomicGameClientStateSplash;
    friend class GameEconomicGameClientStateMainScreen;
    friend class GameEconomicGameClientStatePlayer;
    friend class GameEconomicGameClientStateProgress;
    friend class GameEconomicGameClientStateGameMode;

    /// Construct.
    GameEconomicGameClient(Context* context);
    virtual ~GameEconomicGameClient();

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.https://github.com/urho3d/Urho3D/tree/master/Source/Samples
    virtual String GetScreenJoystickPatchString() const
    {
        return
            "<patch>"
            "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Hat0']]\">"
            "        <attribute name=\"Is Visible\" value=\"false\" />"
            "    </add>"
            "</patch>";
    }

    /// Initialization functions
    void Init(Context * context);

    /// Exit function
    void Exit(void);

    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Events Keyboard
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    /// Input Handlers - Function keys
    void HandlerFunctionKeyDown(StringHash eventType, VariantMap& eventData);
    /// Input Handler - Input
    void HandleInput(const String& input);

    /// Post Updates
    void HandlePostUpdates(StringHash eventType, VariantMap& eventData);

    /// Saved UI Code Just in case
    void SetupScreenViewport(void);
    void SetupScreenViewportBlank(void);
    void SetupScreenUI(void);
    void AddLogoViewport(void);
    int CreateCursor(void);
    void Print(const String& output);
    void EraseUI(void);
    void EraseScene(void);
    void UpdateNetworkStatusUI(bool online);

    /// Console related functions
    void InitializeConsole(void);
    void HandleConsoleCommand(StringHash eventType, VariantMap& eventData);

    /// Temporary online
    bool IsClientConnected(void);
    bool ClientConnect(void);
    bool SetServerSettings(void);

    /// moved here
    AccountInformation * LoginGetPlayerAccountFromAuthorization(String ServerString);


    Vector<PlayerList> LoginGetAccountPlayersFromAuthorization(String ServerString);

    /// Networking
    void HandleNetworkMessage(StringHash eventType, Urho3D::VariantMap& eventData);
    void SendNetworkMessage(NetworkMessageTypes, String Message);
    bool LoadNetworkConfig(NetworkConfiguration &loadingnetwork);
    void HandlerServerConnected(StringHash eventType, Urho3D::VariantMap& eventData);
    void HandlerServerDisconnected(StringHash eventType, Urho3D::VariantMap& eventData);
    void HandlerServerConnectionFailed(StringHash eventType, Urho3D::VariantMap& eventData);
    void NetworkingOnUpdate(float timeStep);
    void ConnectHearbeat(NetworkConfiguration &tempnetwork);
    void ConnectLogin(NetworkConfiguration &tempnetwork);

    /// UI related
    bool loadHUDFile(const char * filename, const int positionx, const int positiony);
    void QuickMenuPressed(StringHash eventType, VariantMap& eventData);
    bool loadUIXML(int windowtype, const int positionx, const int positiony, int selected);
    void loadUIXMLClosePressed(StringHash eventType, VariantMap& eventData);
    void HandlerConfigurationWindowButtonPressed(StringHash eventType, VariantMap& eventData);

    /// Configuration related
    void LoadConfiguration(Configuration &configuration);
    void SaveConfiguration(Configuration &configuration);
    void UpdateConfigurationWindow();

    /// Data
    Vector<AlienRaceInformation> LoadGetAlienRacesFromAuthorization(String ServerString);
    Vector<FactionInformation> LoadGetFactionsFromAuthorization(String ServerString);

    /// Get subsubsystems
    Renderer * GetRenderSubsystems(void) const;
    UI * GetUISubsystems(void) const;
    Graphics * GetGraphicsSubsystems(void) const;
    ResourceCache * GetResourceCacheSubsystems(void) const;

    Window * GetSharedWindow(void) const;

    /// Debug test string
    int GetTestString(void)
    {
        return testvalue;
    }

    /// Base class get
    SharedPtr <GameEconomicGameClient> GetApplicationPTR(void) const
    {
        return applicationPtr;
    }

    /// Save the application Pointer
    SharedPtr<GameEconomicGameClient> applicationPtr;

    /// Configuration
    Configuration * GameConfig;

protected:

    /// Scene Shared Pointer
    SharedPtr<Scene> scene_;
    SharedPtr<Scene> scenePlayerUI_;
    SharedPtr<Scene> sceneLoadingGameModeTransition_;

    /// Camera scene node.
    SharedPtr<Node> cameraNode_;
    SharedPtr<Node> cameraNodePlayerUI_;
    SharedPtr<Scene> sceneLoadingGameModeTransitionUI_;

    /// Urho3D window shared pointers
    SharedPtr<Window> window_;
    SharedPtr<Window> window2_;

    /// Urho3D UIelement root, viewport, and render path
    SharedPtr<UIElement> uiRoot_;

    /// Viewport Shared
    SharedPtr<Viewport> viewport;

    /// RenderPath shared
    SharedPtr<RenderPath> effectRenderPath;

    /// Urho3D Shared pointer for input
    SharedPtr<Input> input_;

    /// Server connection related
    bool ServerConnection;

    /// Debug Testing
    int testvalue;

    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;

    /// This is temoporarily the necessary code
    AccountInformation * ThisAccount;
    StarbaseInformation * ThisStarbase;
    PlayerObject * ThisPlayer;

    unsigned int CurrentPlayerFromList;
    Vector<PlayerList> ThisAccountPlayerList;

    /// Alien and Faction List
    Vector<FactionInformation> ThisFactionList;
    Vector<AlienRaceInformation> ThisAlienRaceList;

    /// Network Valuables- Low footprint
    NetworkConfiguration NetConfig;
    NetworkMode NetOnline;
    NetworkStatus NetStats;
    AccountInformation NetUserAuthenticateInfo;

    /// Platform related
    PlatformOS CurrentPlatform;

    /// toucheenabled_;
    bool touchenabled_;

    ResourceManager * ResourcesManager;

private:

};

/// Login State
class GameEconomicGameClientStateSingleton : public LogicComponent
{
    OBJECT(GameEconomicGameClientStateSingleton);
public:
    SharedPtr<GameEconomicGameClient> baseclass;
    GameEconomicGameClientStateSingleton(Context * context);
    virtual ~GameEconomicGameClientStateSingleton();
    virtual void Enter();
    virtual void Exit();
    virtual void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    virtual void SetParameter(String parameters_);
    virtual void ServerResponseHandler(StringHash eventType, VariantMap& eventData);
protected:
private:
    void Singleton(void);

};

/// Login State
class GameEconomicGameClientStateSplash : public GameEconomicGameClientStateSingleton
{
    OBJECT(GameEconomicGameClientStateSplash);
public:
    GameEconomicGameClientStateSplash(Context * context);
    virtual ~GameEconomicGameClientStateSplash();
    virtual void Enter();
    virtual void Exit();
    virtual void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    virtual void SetParameter(String parameters_);
    virtual void ServerResponseHandler(StringHash eventType, VariantMap& eventData);

protected:
    SharedPtr<GameEconomicGameClient> Existence;

    void SplashScreen(void);
    void SplashSetupScreenViewport(void);
    void SplashShowGameLogo(void);

    void SplashStatInit(void);
    void HandlerSplashUpdate(StringHash eventType, VariantMap& eventData);

    Timer SplashTimer;

    /// Open file as a Urho3d Datafile
    SharedPtr<File> dataFile;
    bool splashcompleted;

private:
};

/// Login State
class GameEconomicGameClientStateLogin : public GameEconomicGameClientStateSingleton
{
    OBJECT(GameEconomicGameClientStateLogin);
public:
    GameEconomicGameClientStateLogin(Context * context);
    virtual ~GameEconomicGameClientStateLogin();
    virtual void Enter();
    virtual void Exit();
    virtual void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    virtual void SetParameter(String parameters_);
    virtual void ServerResponseHandler(StringHash eventType, VariantMap& eventData);
private:
    void LoginScreen(void);
    void LoginScreenUI(void);
    void SetupViewport(void);
    void LoginProgressUI(void);
    void LoginScreenUINewAccountHandleClosePressed(StringHash eventType, VariantMap& eventData);
    void LoginScreenUILoginHandleClosePressed(StringHash eventType, VariantMap& eventData);
    AccountInformation * LoginGetPlayerAccountFromAuthorization(String ServerString);
    Vector<PlayerList> LoginGetAccountPlayersFromAuthorization(String ServerString);
    void TerminateCommLink(StringHash eventType, VariantMap& eventData);
    void ShowServerStatusUI(void);
protected:
    SharedPtr<GameEconomicGameClient> Existence;


};


/// Main Screen State
class GameEconomicGameClientStateMainScreen: public GameEconomicGameClientStateSingleton
{
    OBJECT(GameEconomicGameClientStateMainScreen);

public:
    GameEconomicGameClientStateMainScreen(Context * context);
    virtual ~GameEconomicGameClientStateMainScreen();
    virtual void Enter();
    virtual void Exit();
    virtual void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    virtual void SetParameter(String parameters_);
    virtual void ServerResponseHandler(StringHash eventType, VariantMap& eventData);
private:
    void MainScreen(void);
    void MainScreenUI(void);
    void MainScreenViewport(void);
    void TerminateSkynet(void);
    void   UpdateMainMenuCharacterInfo(void);
    void ChangePlayerUI(void);
    void ChangePlayerUIHandleCloseButton(StringHash eventType, VariantMap& eventData);
    void ChangePlayerUIHandleSelection(StringHash eventType, VariantMap& eventData);

protected:
    SharedPtr<GameEconomicGameClient> Existence;
    void HandleMenuPressed(StringHash eventType, VariantMap& eventData);
};


/// Main Screen State
class GameEconomicGameClientStatePlayer: public GameEconomicGameClientStateSingleton
{
    OBJECT(GameEconomicGameClientStatePlayer);

public:
    GameEconomicGameClientStatePlayer(Context * context);
    virtual ~GameEconomicGameClientStatePlayer();
    virtual void Enter();
    virtual void Exit();
    virtual void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    virtual void SetParameter(String parameters_);
    virtual void ServerResponseHandler(StringHash eventType, VariantMap& eventData);
private:
    void Player(void);
    void CreatePlayerScreenUI(void);
    void CreatePlayerUISelectPressed(StringHash eventType, VariantMap& eventData);
    void CreatePlayerUITabButtonPressed(StringHash eventType, VariantMap& eventData);
    void CreatePlayerUICreateCharacterPressed(StringHash eventType, VariantMap& eventData);

    void CreatePlayerUpdateAlienRaceList(void);
    void CreatePlayerUpdateFactionsList(void);
    void CreatePlayerMainScreenViewport(void);
    void CreatePlayerUpdateMainMenuCharacterInfo(void);
    void CreatePlayerSetVisible(VisibleWindow TabWindow);
    void CreatePlayerHandleFactionTabButton(void);
    void CreatePlayerHandleProfileTabButton(void);
    void CreatePlayerHandleGenderTabButton(void);
    void CreatePlayerHandleRaceTabButton(void);
    void CreatePlayerHandlePersonalityTabButton(void);
    void CreatePlayerUpdatePersonalityList(void);
    void CreatePlayerUpdateGenderList(void);
    void CreatePlayerNameInputHandleChange(StringHash eventType, VariantMap& eventData);
    void CreatePlayerHandlePlayerCompleted(StringHash eventType, VariantMap& eventData);
    void CreatePlayerUIHandleListViews(StringHash eventType, VariantMap& eventData);

protected:
    SharedPtr<GameEconomicGameClient> Existence;

    PlayerObject * NewPlayer;

    bool AllowClose;
};

/// Login State
class GameEconomicGameClientStateProgress : public GameEconomicGameClientStateSingleton
{
    OBJECT(GameEconomicGameClientStateProgress);
public:
    SharedPtr<GameEconomicGameClient> baseclass;
    GameEconomicGameClientStateProgress(Context * context);
    virtual ~GameEconomicGameClientStateProgress();
    virtual void Enter();
    virtual void Exit();
    virtual void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    virtual void SetParameter(String parameters_);
    virtual void ServerResponseHandler(StringHash eventType, VariantMap& eventData);

private:
    PlayerObject * ConvertServerStringtoPlayer(String ServerString);
    StarbaseInformation * ConvertServerStringtoStarbase(String ServerString);
    void ProgressScreenViewport(void);
    void ProgressScreenUI(void);
    void Intro(void);

    void GenerateMapDataToGameMap(string MapData);
    void GenerateMapDataConvertIntoGameObject(MapDataType inputtype, string InputString);
    vector<string> Split(const string &s, char delim);
    void HandleContinuePressed(StringHash eventType, VariantMap& eventData);

    bool loadScene(void);

protected:
    SharedPtr<GameEconomicGameClient> Existence;

    SharedPtr<Scene> progressScene_;
    SharedPtr<UI> progressUI_;
    SharedPtr<RenderPath> progressrendererPath_;
    /// RenderPath shared
    SharedPtr<RenderPath> effectRenderPath;

    SharedPtr<Camera> progresscameraNode_;
    SharedPtr<Viewport> progressViewport_;
    SharedPtr<Window> progressWindow_;

    Timer ProgressTimer;
    String progressloadparameters_;

    int progressload_;
    int progressloadingstate;

    /// Open file as a Urho3d Datafile
    SharedPtr<File> dataFile;
    SharedPtr<File> ProgressDataFile;

    bool loadedtransition;
    WeakPtr<VariantMap> eventDataPtr;

    unsigned int progressloadmode_;
    unsigned int progressloadgenerated_;

    bool FirstTime;
    bool ContinueGame;

};

class GameEconomicGameClientStateGameMode: public GameEconomicGameClientStateSingleton
{
    OBJECT(GameEconomicGameClientStateGameMode);
public:
    GameEconomicGameClientStateGameMode(Context * context);
    virtual ~GameEconomicGameClientStateGameMode();
    virtual void Enter();
    virtual void Exit();
    virtual void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    virtual void SetParameter(String parameters_);
    virtual void ServerResponseHandler(StringHash eventType, VariantMap& eventData);

    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void OnMoveCamera(float timeStep);
    bool Raycast(float maxDistance, Vector3& hitPos, Node*& hitNode);
    void GetTargetPressed(void);

private:
    void GameMode(void);
    void LoadGameModeUI(void);
    void HandlerPostUpdates(StringHash eventType, VariantMap& eventData);
    void InteractListener(StringHash eventType, VariantMap& eventData);
    void GameModeSendEventHandler(StringHash eventType, VariantMap& eventData);
    void GameModeAddUIElements(void);

    bool LoadUIXML(int windowtype, const int positionx, const int positiony);
    void HandleTopMenuPressed(StringHash eventType, VariantMap& eventData);
    void HandleUIWindowClosed(StringHash eventType, VariantMap& eventData);
    void HandleUIStarbaseBriefButtonPressed(StringHash eventType, VariantMap& eventData);

protected:

    SharedPtr<GameEconomicGameClient> Existence;
};

/// Miscellanous functions
vector<string> split(const string& s, const string& delim, const bool keep_empty=true);
time_t ConvertStringToTime(const char * buff, time_t timeseed);
string GenerateName(char group, char subgroup);

string ConvertUIntToString(unsigned int val);
float cutoff(float inputvalue, float pointmid, float range,bool debug);
float StringToFloat(string buffer);
Vector3 NormalizedToWorld(Image *height, Terrain *terrain, Vector2 normalized);
typedef std::pair<float,float> range ;
bool intersects( range a, range b );
range make_range( float a, float b );

#endif




