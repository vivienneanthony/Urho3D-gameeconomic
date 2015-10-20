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
    UIGAME_UISTARBASEDISPLAYBRIEF,
    UIGAME_UIINTERACT,
    UIGAME_UICOMMUNICATIONS,
    UIGAME_UIACTIVITIES,
    UIGAME_UIACTIVITYCREATE,
    UIGAME_UIREFRIGERATIONSTORAGE,
    UIGAME_HUDCLAUDIUS,
    UIGAME_UIINTERACTDRONE,
    UIGAME_UIINTERACTFORCEFIELD,
    UIGAME_UIINTERACTLIGHT,
    UIGAME_UIINTERACTPOWERSOURCE,
    UIGAME_UIINTERACTREFRIGERATIONUNIT,
    UIGAME_UIINTERACTREPLICATIONPRINTER,
    UIGAME_UIINTERACTSTORAGECONTAINER,

};

/// enums

#define InteractOptions_ReplicationPrinter  3
#define InteractOptions_RefrigerationUnit  1
#define InteractOptions_Light  1
#define InteractOptions_Drone  2


static const char * ReplicationPrinterInteractText[] =
{
    "Toggle Power ...","Replication Job ...","Printer Job ..."
};


static const char *ReplicationPrinterInteractOptions[] =
{
    "TogglePower","ReplicationJob","PrinterJob"
};


static const char * RefrigerationUnitInteractText[] =
{
    "Toggle Power ..."
};


static const char *RefrigerationUnitInteractOptions[] =
{
    "TogglePower"
};


static const char * LightInteractText[] =
{
    "Toggle Power ..."
};


static const char *LightInteractOptions[] =
{
    "TogglePower"
};


static const char * DroneInteractText[] =
{
    "Toggle Power ...","Assign Exploration Mission..."
};


static const char *DroneInteractOptions[] =
{
    "TogglePower","ExplorationMission"
};

#endif

