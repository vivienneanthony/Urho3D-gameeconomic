#ifndef NETWORKING_H
#define NETWORKING_H


enum NetworkMessageTypes
{
    NetMessageNone,
    NetMessageAuthenticateSend,
    NetMessageAuthenticateAcknowledge,
    NetMessageAdminClientSend=999,
    NetMessageAdminClientSendAcknowledge
};

enum NetworkClientTypes
{
    Unknown=0,
    Unauthenticated,
    Authenticated=100,
    AdminInterface=200
};

EVENT(NETWORKCLIENTIDENTITY, NetworkClientIdentity)
{
    PARAM(NETWORK_CLIENTYPE, clientType);
    PARAM(NETWORK_CLIENTARRIVAL, clientArrival);
}


#endif
