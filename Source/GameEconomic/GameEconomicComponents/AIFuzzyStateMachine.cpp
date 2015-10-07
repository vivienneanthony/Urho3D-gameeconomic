#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include "AIFuzzyStateMachineStates.h"
#include "AIFuzzyStateMachineState.h"
#include "AIFuzzyStateMachine.h"
#include "AIFuzzyControl.h"

#include <iostream>
#include <vector>

using namespace Urho3D;
using namespace std;

/// Constructor
AIFuzzyStateMachine::AIFuzzyStateMachine(Context * context)
    :AIFuzzyStateMachineState(context)
{
    /// Update Mask
    SetUpdateEventMask(USE_FIXEDUPDATE);

    /// Subscribe
    ///SubscribeToEvent(E_UPDATE,HANDLER(AIFuzzyStateMachine,HandleUpdate));

    return;
}

/// Set machine type and AI Fuzzy
void AIFuzzyStateMachine::Set(AIMachineTypes type= FUSH_MACHINE_NONE, AIFuzzyControl* parent=NULL)
{
    M_Type=type;
    M_Parent=parent;

    return;
}

/// Entity Register Object
void AIFuzzyStateMachine::RegisterObject(Context* context)
{
    context->RegisterFactory<AIFuzzyStateMachine>();

    return;
}

/// Urho3D Specific Fixes Update
void AIFuzzyStateMachine::FixedUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    /// Urho specific update
    Update(timeStep);

    return;
}

/// AI Fuzzy Update
void AIFuzzyStateMachine::Update(float dt)
{
    /// Run update machine
    UpdateMachine(dt);
}

/// Update Machine
void AIFuzzyStateMachine::UpdateMachine(float dt)
{
    ///don't do anything if you have no states
    if(M_States.size() == 0)
    {
        cout << "no states exist"<< endl;
        return;
    }


    ///check for activations, and then update
    M_ActivatedStates.clear();
    std::vector<AIFuzzyStateMachineState*> nonActiveStates;
    for(unsigned int i =0; i<M_States.size(); i++)
    {
        if(M_States[i]->CalculateActivation() > 0)
            M_ActivatedStates.push_back(M_States[i]);
        else
            nonActiveStates.push_back(M_States[i]);
    }

    ///Exit all non active states for cleanup
    if(nonActiveStates.size() != 0)
    {
        for(unsigned int i =0; i<nonActiveStates.size(); i++)
            nonActiveStates[i]->Exit();
    }

    ///Update all activated states
    if(M_ActivatedStates.size() != 0)
    {
        for(unsigned int i =0; i<M_ActivatedStates.size(); i++)
            M_ActivatedStates[i]->Update(dt);
    }
}

/// Add a new state - push on back
void AIFuzzyStateMachine::AddState(AIFuzzyStateMachineState* state)
{
    M_States.push_back(state);
}

/// IsAStateActive
bool AIFuzzyStateMachine::IsActive(AIFuzzyStateMachineState *state)
{
    if(M_ActivatedStates.size() != 0)
    {
        for(unsigned int i =0; i<M_ActivatedStates.size(); i++)
            if(M_ActivatedStates[i] == state)
                return true;
    }
    return false;
}

/// Reset All sates
void AIFuzzyStateMachine::Reset()
{
    ///exit and init all the states
    for(unsigned int i =0; i<M_States.size(); i++)
    {
        M_States[i]->Exit();
        M_States[i]->Initialize();
    }
}

/// Handle Update
void  AIFuzzyStateMachine::HandleUpdate(StringHash eventType, VariantMap& eventData)
{

    using namespace Update;

    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    /// Urho specific update
    Update(timeStep);
}
