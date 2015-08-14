#ifndef PLATFORM_H
#define PLATFORM_H


#include <Urho3D/Urho3D.h>

using namespace Urho3D;
using namespace std;

enum PlatformOS
{
    PlatformUnknown=0,
    PlatformWindows,
    PlatformLinux,
    PlatformMacOSX,
    PlatformAndroid,
    PlatformIOS,
    PlatformRaspberry
};

class OSTool
{
public:
    static PlatformOS GetOS(String InputString)
    {
        /// create a return value;
        PlatformOS returnOS = PlatformUnknown;

        /// Get List
        Vector<String> OSList;

        /// Push OS
        OSList.Push("Unknown");
        OSList.Push("Windows");
        OSList.Push("Linux");
        OSList.Push("Mac OS X");
        OSList.Push("Android");
        OSList.Push("iOS");
        OSList.Push("Raspberry Pi");

        /// Loop through the size
        for(unsigned int i =0; i<OSList.Size(); i++)
        {
            /// Scan List
            if(OSList.At(i) == InputString)
            {
                returnOS = (PlatformOS) i;
            }
        }

        return returnOS;

    };
};

#endif
