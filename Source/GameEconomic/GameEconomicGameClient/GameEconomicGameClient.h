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

#include "../GameEconomicAppHead.h"
#include "../GameEconomicComponents/Accounts.h"
#include "../GameEconomicComponents/Player.h"

#include "../GameEconomicComponents/GameStateEvents.h"
#include "../GameEconomicComponents/GameStateHandlerComponent.h"

#include "../Networking.h"
#include "../Platform.h"
#include "../Accounts.h"


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

    /// This is temoporarily the necessary code
    AccountInformation * ThisAccount;
    Vector<PlayerList> ThisAccountPlayerList;

    /// Server connection related
    bool ServerConnection;

    /// Debug Testing
    int testvalue;

    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;

    /// Network Valuables- Low footprint
    NetworkConfiguration NetConfig;
    NetworkMode NetOnline;
    NetworkStatus NetStats;
    AccountInformation NetUserAuthenticateInfo;

    /// Platform related
    PlatformOS CurrentPlatform;

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
private:
    void Singleton(void);
protected:
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
private:
protected:
    void SplashScreen(void);
    void SplashSetupScreenViewport(void);
    void SplashShowGameLogo(void);

    void SplashStatInit(void);
    void HandlerSplashUpdate(StringHash eventType, VariantMap& eventData);

    Timer SplashTimer;
    SharedPtr<GameEconomicGameClient> Existence;
    /// Open file as a Urho3d Datafile
    SharedPtr<File> dataFile;
    bool splashcompleted;


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

    void ShowServerStatusUI(void);
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


