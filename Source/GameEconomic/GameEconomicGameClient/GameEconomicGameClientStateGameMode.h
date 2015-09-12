#ifndef GAMEMODEEVENT_H
#define GAMEMODEEVENT_H

//namespace ExistenceClientStates
//{
/// P_CMD
enum GameModeEvent
{
    GAMEMODE_EVENT_TERMINATE
};

/// Key pressed.
EVENT(G_MODE_CHANGE, GameModeChange)
{
    PARAM(P_CMD, cmdType);                // States
    PARAM(P_ARG, cmdArguments);
    PARAM(P_OBJ, sender);                // class
}
///}

enum GameWindowTypes
{
    UIGAME_UINONE,
    UIGAME_UISTARBASEDISPLAYBRIEF
};

#endif
