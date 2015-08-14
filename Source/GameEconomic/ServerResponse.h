#ifndef SERVERRESPONSEEVENT_H
#define SERVERRESPONSEEVENT_H

//namespace ExistenceClientStates
//{
/// P_CMD
enum ServerResponseTypes
{
    ServerResponse_None,
    ServerResponse_AccountAuthentication,
    ServerResponse_Disconnected,
    ServerResponse_SentAccountPlayers
};

/// Key pressed.
EVENT(N_SERVER_RESPONSE, ServerResponse)
{
    PARAM(P_CMD, cmdType);                // States
    PARAM(P_ARG, cmdArguments);
    PARAM(P_OBJ, sender);                // class
}
///}

#endif
