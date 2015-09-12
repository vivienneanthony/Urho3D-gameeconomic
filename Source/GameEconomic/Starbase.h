#ifndef STARBASE_H_INCLUDE
#define STARBASE_H_INCLUDE

#include <Urho3D/Urho3D.h>

#include <iostream>
#include <ctime>

using namespace std;
using namespace Urho3D;


struct StarbaseInformation
{
    /// name information
    String Name;
    String MapData;
    String Owner;
    String UniqueID;

    /// logiccomponet
    LogicComponent * ThisComponent;
};


#endif
