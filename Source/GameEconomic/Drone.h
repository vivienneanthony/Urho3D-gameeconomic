#ifndef Drone_Information_H
#define Drone_Information_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

enum DroneTypeClass
{
    Drone100Beta,
    Drone200Alpha
};

struct DroneInformation
{
    DroneTypeClass DroneType;
    String Name;
    unsigned int AlienRace;
    bool AlienAllianceAligned;

};

#endif
