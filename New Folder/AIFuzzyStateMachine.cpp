#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include "AIFuzzyStateMachine.h"
#include "AIFuzzyStateMachineState.h"
#include "AIFuzzyControl.h"

#include <iostream>
#include <vector>


using namespace Urho3D;
using namespace std;

AIFuzzyStateMachine::AIFuzzyStateMachine(Context * context)
    :AIFuzzyStateMachineState(context)
{
    /// Update Mask
    SetUpdateEventMask(USE_FIXEDUPDATE);

    //ctor
}


/// Set
void AIFuzzyStateMachine::Set(AIMachineTypes type= FUSH_MACHINE_NONE,
                              AIFuzzyControl* parent=NULL)
{
    M_Type=type;
    M_Parent=parent;

    return;
}

AIFuzzyStateMachine::~AIFuzzyStateMachine()
{
    return;
}

/// Entity Register Object
void AIFuzzyStateMachine::RegisterObject(Context* context)
{
    context->RegisterFactory<AIFuzzyStateMachine>();

    return;
}


void AIFuzzyStateMachine::Update(float dt)
{
    /// Run update machine
    UpdateMachine(dt);
}

//---------------------------------------------------------
void AIFuzzyStateMachine::UpdateMachine(float dt)
{
    //don't do anything if you have no states
    if(M_States.size() == 0)
    {
        cout << "no states exist"<< endl;
        return;
    }


    //check for activations, and then update
    M_ActivatedStates.clear();
    std::vector<AIFuzzyStateMachineState*> nonActiveStates;
    for(unsigned int i =0; i<M_States.size(); i++)
    {
        if(M_States[i]->CalculateActivation() > 0)
            M_ActivatedStates.push_back(M_States[i]);
        else
            nonActiveStates.push_back(M_States[i]);
    }

    //Exit all non active states for cleanup
    if(nonActiveStates.size() != 0)
    {
        for(unsigned int i =0; i<nonActiveStates.size(); i++)
            nonActiveStates[i]->Exit();
    }

    //Update all activated states
    if(M_ActivatedStates.size() != 0)
    {
        for(unsigned int i =0; i<M_ActivatedStates.size(); i++)
            M_ActivatedStates[i]->Update(dt);
    }
}

//---------------------------------------------------------
void AIFuzzyStateMachine::AddState(AIFuzzyStateMachineState* state)
{
    M_States.push_back(state);
}

//---------------------------------------------------------
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

//---------------------------------------------------------
void AIFuzzyStateMachine::Reset()
{
    //exit and init all the states
    for(unsigned int i =0; i<M_States.size(); i++)
    {
        M_States[i]->Exit();
        M_States[i]->Initialize();
    }
}
