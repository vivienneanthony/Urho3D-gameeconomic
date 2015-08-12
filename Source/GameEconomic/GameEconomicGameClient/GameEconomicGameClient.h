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


string ConvertUIntToString(unsigned int val);

/// This first example, maintaining tradition, prints a "Hello World" message.
/// Furthermore it shows:
///     - Using the Sample / Application classes, which initialize the Urho3D engine and run the main loop
///     - Adding a Text element to the graphical user interface
///     - Subscribing to and handling of update events


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

    void Init(Context * context);

    /// Diaplay login screen
    void SetupScreenViewport(void);
    void SetupScreenUI(void);

    void Exit(void);
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Events Keyboard
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);

    void HandleInput(const String& input);
    void eraseScene(void);

    void AddLogoViewport(void);

    int CreateCursor(void);

    void MoveCamera(float timeStep);
    void Print(const String& output);

    void HandlePostUpdates(StringHash eventType, VariantMap& eventData);

    /// Render related functions
    int LoadCharacterMesh(int mode, String nodename, unsigned int alienrace, unsigned int gender);
    int loadplayerMesh(Node * playermeshNode, int alienrace, int gender,int mode);

    /// File related functions
    void LoadAccount(void);
//    void SaveAccount(accountinformation account);
    void SavePlayer(bool activeplayer);
    int LoadAccountPlayers(void);
    int LoadPlayer(int player) ;
    int LoadTemporaryPlayer(int player);
    int GenerateSceneLoadDifferential(const char *filename=NULL);
    int LoadEnvironmentSettings(const char *environment);

    /// Console related functions
    void InitializeConsole(void);
    void HandleConsoleCommand(StringHash eventType, VariantMap& eventData);
    void HandlerFunctionKeyDown(StringHash eventType, VariantMap& eventData);


    int ConsoleActionEnvironment(const char * lineinput);
    int ConsoleActionCamera(const char * lineinput);
    int ConsoleActionDebug(const char * lineinput);
    int ConsoleActionCharacter(const char * lineinput);
    int ConsoleActionRenderer(const char * lineinput);
    int ConsoleActionBuild(const char * lineinput);

    void EraseUI(void);
    void EraseScene(void);
    /// UI Related Functions
    void loadSceneUI(void);
    bool loadHUDFile(const char * filename, const int positionx, const int positiony);
    void loadUIXMLClosePressed(StringHash eventType, VariantMap& eventData);
    bool loadUIXML(int windowtype, const int positionx, const int positiony, int selected);
    void QuickMenuPressed(StringHash eventType, VariantMap& eventData);
    void UpdateUI(float timestep);
    void PlayerWindowUpdateUI(int selected);
    void PlayerWindowHandleDisplaySelection(StringHash eventType, VariantMap& eventData);
    int UpdateUISceneLoader(void);
    void UpdatePlayerInfoBar(void);
    void SceneLoaderHanderPress(StringHash eventType, VariantMap& eventData);
//    int GenerateSceneUpdateEnvironment(terrain_rule terrainrule);

    /// Temporary online
    bool IsClientConnected(void);
    bool ClientConnect(void);
    bool SetServerSettings(void);

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

    /// Existence Weak pointer for a single character
//    WeakPtr<Character> character_;

    /// Existence player structure class and variable declation for character/player related information
    //Player  TemporaryPlayer;
    //Player  * TemporaryAccountPlayerList;
    // unsigned int TemporaryAccountPlayerSelected;
    //  unsigned int TemporaryAccountPlayerListLimit;

    /// Existence class and variable declaration for alien race alliance information
    vector<string> aliensarray;
    vector<string> tempaliensarray;

    /// This is temoporarily the necessary code
    bool accountexist;

    /// Server connection related
    bool ServerConnection;

    /// Debug Testing
    int testvalue;

    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;



private:
    void SplashShowGameLogo(void);
    void SplashStatInit(void);
    void HandlerSplashUpdate(StringHash eventType, VariantMap& eventData);
    void SplashSetupScreenViewport(void);

    Timer SplashTimer;
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
private:
    void LoginScreen(void);
    void LoginScreenUI(void);
    void LoginScreenUINewAccountHandleClosePressed(StringHash eventType, VariantMap& eventData);
    void LoginScreenUILoginHandleClosePressed(StringHash eventType, VariantMap& eventData);

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


