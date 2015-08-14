#ifndef NETWORKING_H
#define NETWORKING_H



enum NetworkMessageTypes
{
    NetMessageNone=30,
    NetMessageAuthenticateAcknowledge,
    NetMessageAuthenticateSend,
    NetMessageAuthenticatedApproved,
    NetMessageAuthenticatedDenied,
    NetMessageRequest=40,
    NetMessageRequestApprovedGetAccountPlayers,
    NetMessageHeartbeatRequest=90,
    NetMessageHeartbeatAcknowledge,
    NetMessageAdminClientSend=100,
    NetMessageAdminClientSendAcknowledge
};

enum NetworkClientTypes
{
    Unknown=0,
    Unauthenticated,
    Authenticated=100,
    AdminInterface=200,
    RequestHearbeat=900
};

enum NetworkMode
{
    NetDisconnected=0,
    NetConnected

};

enum NetworkStatus
{
    NetworkOffline=0,
    NetworkOnline
};


struct NetworkConfiguration
{
    /// basic configuration
    String hostserver;
    unsigned int hostport;
    NetworkClientTypes hostidentity;
};


EVENT(NETWORKCLIENTIDENTITY, NetworkClientIdentity)
{
    PARAM(NETWORK_CLIENTYPE, clientType);
    PARAM(NETWORK_CLIENTARRIVAL, clientArrival);
}


#endif
