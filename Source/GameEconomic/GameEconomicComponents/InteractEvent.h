#ifndef INTERACTEVENT_H
#define INTERACTEVENT_H


/// Key pressed.
EVENT(INTERACTEVENT, InteractEvent)
{
    PARAM(P_NODE, HitNode);                // States
    PARAM(P_HITPOSITION, HitPosition);
    PARAM(P_MOUSEPOSITION, MousePosition);
    PARAM(P_OBJ, Sender);                // class
}
///}

EVENT(INTERACTUIEVENT,InteractUIEvent)
{
    PARAM(P_NODE, InteractNode);         // hitnode
    PARAM(P_CMD,Cmd);
    PARAM(P_OBJ, Sender);                // class
    PARAM(P_MOUSEPOSITION, MousePosition);
}
#endif
