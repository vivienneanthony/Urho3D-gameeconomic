#ifndef NETWORKING_H
#define NETWORKING_H


/// NetworkMessageTypes
enum NetworkMessageTypes
{
    NetMessageNone,
    NetMessageAuthenticateSend,
    NetMessageAuthenticateAcknowledge,
    NetMessageAdminClientSend=999,
    NetMessageAdminClientSendAcknowledge
};

#endif
