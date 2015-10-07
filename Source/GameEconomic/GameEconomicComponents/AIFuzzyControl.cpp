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

#include "Entity.h"
#include "Drone.h"

#include "AIFuzzyStateMachineStates.h"
#include "AIFuzzyControl.h"

using namespace std;
using namespace Urho3D;


/// Constructor and Destructor
AIFuzzyControl::AIFuzzyControl(Context * context):LogicComponent(context),M_Entity(NULL),M_Logic(FuzzyLogic_Idle)
{
    M_Entity = NULL;
    M_Type = CONTROLTYPE_NONE;

    return;

}

/// Destructor
AIFuzzyControl::~AIFuzzyControl()
{
    return;
}

/// Register
void AIFuzzyControl::RegisterObject(Context * context)
{
    context->RegisterFactory<AIFuzzyControl>();
}

/// Update and Initialize
void AIFuzzyControl::FixedUpdate(float dt)
{
    return;
}

/// Initialize
void AIFuzzyControl::Initialize(void)
{
    SetUpdateEventMask(USE_FIXEDUPDATE);

    return;
}

/// Set Ship
void AIFuzzyControl::SetAIDrone(Drone* ThisEntity)
{
    M_Entity = ThisEntity;

    return;
}
