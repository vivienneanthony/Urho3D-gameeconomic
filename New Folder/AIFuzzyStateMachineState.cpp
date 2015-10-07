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


#include "AIFuzzyStateMachineState.h"

#include "AIFuzzyStateMachine.h"
#include "AIFuzzyControl.h"


using namespace std;
using namespace Urho3D;


/// Constructor
AIFuzzyStateMachineState::AIFuzzyStateMachineState(Context * context)
    :LogicComponent(context)
    ,M_Type(CONTROLTYPE_NONE)
    ,M_Parent(NULL)
    ,M_ActivationLevel(0.0f)
{
    /// Forced set defaults
    M_Type=CONTROLTYPE_NONE;
    M_Parent=NULL;
    M_ActivationLevel=0.0f;

    return;

}


/// Entity Register Object
void AIFuzzyStateMachineState::RegisterObject(Context* context)
{
    context->RegisterFactory<AIFuzzyStateMachineState>();

    return;
}


/// Set
void AIFuzzyStateMachineState::Set(AIMachineTypes type = FUSH_MACHINE_NONE,
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

