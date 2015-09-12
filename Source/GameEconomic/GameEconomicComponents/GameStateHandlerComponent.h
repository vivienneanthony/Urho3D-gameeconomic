#ifndef GAMESTATEHANDLERCOMPONENT_H
#define GAMESTATEHANDLERCOMPONENT_H

#define         UI_NONE                             0
#define         UI_LOGININTERFACE                   1
#define         UI_ACCOUNTCREATIONINTERFACE         2
#define         UI_CHARACTERCREATIONINTERFACE       3
#define         UI_CHARACTERSELECTIONINTERFACE      4
#define         UI_PROGRESSINTERFACE                5
#define         UI_GAMECONSOLE                      6
#define         UI_SPLASHSCREEN                     7

#define         STATE_NONE                          10
#define         STATE_MAIN                          11
#define         STATE_GAME                          12


#define         UI_CONSOLEOFF                     0
#define         UI_CONSOLEON                      1

#define         CAMERAMODE_DEFAULT                  0
#define         CAMERAMODE_FIRSTPERSON              1
#define         CAMERAMODE_FLY                      2

#include <Urho3D/Urho3D.h>

#include "../GameEconomicGameClient/GameEconomicGameClient.h"

using namespace std;
using namespace Urho3D;

class GameEconomicGameClient;
class GameEconomicGameClientStateSingleton;

class URHO3D_API  GameStateHandlerComponent : public LogicComponent
{

    OBJECT(GameStateHandlerComponent);
public:
    /// Constructor
    GameStateHandlerComponent(Context * context);
    /// Destruct.
    virtual  ~GameStateHandlerComponent();


    /// Register object factory and attributes.
    static void RegisterNewSubsystem(Context* context);
    static void RegisterGameStates(Context* context);

    void Start(void);

    /// App related
    void SetApplication(SharedPtr <GameEconomicGameClient> temp);
    void createState(String newState,Urho3D::VariantMap& eventData);
    void onStateChange( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    SharedPtr<GameEconomicGameClient> GetApplication(void);

    /// Just a way to get the current state
    String GetCurrentState(void);

    /// Function to access other states
    int GetConsoleState(void);
    int SetConsoleState(int flag);

    int GetUIState(void);
    int SetUIState(int flag);

    int GetCameraMode(void);
    int SetCameraMode(int flag);

    int GetDebugHudMode(void);
    int SetDebugHudMode(int flag);

private:
    /// Made the gameState a shared to keep in memory
    GameEconomicGameClientStateSingleton *  gameState;

    /// Made the application pointer shared to keep in memory
    SharedPtr<GameEconomicGameClient> GameStateHandlerApplication;

    /// Added flags
    int consolestate;
    int uistate;
    int cameramode;
    int debughud;

    String state;
};

#endif // GAMESTATEHANDLER_H

