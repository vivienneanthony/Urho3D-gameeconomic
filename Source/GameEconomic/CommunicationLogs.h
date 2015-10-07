#ifndef COMMUNICATIONLOGS_H
#define COMMUNICATIONLOGS_H


enum LogFormatType
{
    LogFormat_Personal
};

struct CommunicationLog
{
    unsigned long int Creation;
    String Title;
    String Text;
};

#endif
