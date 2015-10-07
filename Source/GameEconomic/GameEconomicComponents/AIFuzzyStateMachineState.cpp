#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include <iostream>

#include "AIFuzzyStateMachineStates.h"
#include "AIFuzzyStateMachineState.h"

#include "AIFuzzyStateMachine.h"
#include "AIFuzzyControl.h"


using namespace std;
using namespace Urho3D;


/// Constructor
AIFuzzyStateMachineState::AIFuzzyStateMachineState(Context * context)
    :LogicComponent(context)
    ,M_Type(FUSH_STATE_NONE)
    ,M_Parent(NULL)
    ,M_ActivationLevel(0.0f)
{
    /// Forced set defaults
    M_Type=FUSH_STATE_NONE;
    M_Parent=NULL;
    M_ActivationLevel=0.0f;

    /// Update Mask
    SetUpdateEventMask(USE_FIXEDUPDATE);

    return;

}


/// Entity Register Object
void AIFuzzyStateMachineState::RegisterObject(Context* context)
{
    context->RegisterFactory<AIFuzzyStateMachineState>();

    return;
}

AIFuzzyStateMachineState::~AIFuzzyStateMachineState()
{

}

/// Set
void AIFuzzyStateMachineState::Set(AIStatesTypes type = FUSH_STATE_NONE,
                                   AIFuzzyControl* parent=NULL)
{
    M_Type=type;
    M_Parent=parent;
    M_ActivationLevel=0.0f;
}

void AIFuzzyStateMachineState::Enter(void)
{
    return;
}

void AIFuzzyStateMachineState::Exit(void)
{
    return;
}

void AIFuzzyStateMachineState::FixedUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    Update(timeStep);

    return;
}


void AIFuzzyStateMachineState::Update(float dt)
{
    return;
}

/// Initialize
void AIFuzzyStateMachineState::Initialize(void)
{
    M_ActivationLevel=0.0f;
};

/// Calculate Activation
float AIFuzzyStateMachineState::CalculateActivation()
{
    return M_ActivationLevel;
};

/// Check Lower Bound
unsigned int AIFuzzyStateMachineState::CheckLowerBound(float LBound = 0.0f)
{
    if(M_ActivationLevel <LBound)
        M_ActivationLevel=LBound;
};

/// Check Upperbound
unsigned int AIFuzzyStateMachineState::CheckUpperBound(float UBound = 0.0f)
{
    if(M_ActivationLevel >UBound)
        M_ActivationLevel=UBound;
};

/// Check Bounds
unsigned int AIFuzzyStateMachineState::CheckBound(float LB =0.0f, float UB=0.0f)
{
    CheckLowerBound(LB);
    CheckUpperBound(UB);
};

void AIFuzzyStateMachineState::HandleUpdate(StringHash eventType, VariantMap& eventData)
{

    using namespace Update;

    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    Update(timeStep);
}
