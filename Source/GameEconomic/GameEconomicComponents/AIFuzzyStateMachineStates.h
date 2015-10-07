#ifndef AIIFUZZYMACHINEINFORMATION_H
#define AIIFUZZYMACHINEINFORMATION_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Input/Controls.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace Urho3D;

/// States Type
enum AIStatesTypes
{
    FUSH_STATE_NONE,
    FUSH_STATE_IDLE
};


/// Set Machine Types
enum AIMachineTypes
{
    FUSH_MACHINE_NONE,
    FUSH_MACHINE_DRONE100,
    FUSH_MACHINE_DRONE200
};



#endif
