#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Scene/Node.h"

#include <iostream>

using namespace std;
using namespace Urho3D;


#include "AIFuzzyStateMachineStates.h"
#include "AIFuzzyStateMachineState.h"

#include "AIFuzzyStateMachine.h"
#include "AIFuzzyControl.h"

#include "AIFuzzyDroneStateIdle.h"

///Constructor
AIFuzzyDroneStateIdle::AIFuzzyDroneStateIdle(Context * context):
    AIFuzzyStateMachineState(context)
{
    /// Update Mask
    SetUpdateEventMask(USE_FIXEDUPDATE);

    return;
}

/// keep original idea
AIFuzzyDroneStateIdle::AIFuzzyDroneStateIdle(Context * context, AIFuzzyControl* SetParent):
    AIFuzzyStateMachineState(context)
    , changeFocusVectorTime(0.0f)
{
    /// Update Mask
    SetUpdateEventMask(USE_FIXEDUPDATE);

    /// copy variables
    M_Parent=SetParent;

    return;
}


/// Destructor
AIFuzzyDroneStateIdle::~AIFuzzyDroneStateIdle()
{
    return;
}

/// Set
void AIFuzzyDroneStateIdle::Set(AIStatesTypes SetState, AIFuzzyControl* SetParent)
{
    M_Parent = SetParent;
    M_Type = SetState;

    return;
}

void AIFuzzyDroneStateIdle::FixedUpdate(StringHash eventType, VariantMap& eventData)
{

    using namespace Update;

    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    Update(timeStep);
}

/// Change something
void AIFuzzyDroneStateIdle::Update(float dt)
{
    /// If Fuzzy is Idle
    if(M_Parent->M_Logic==FuzzyLogic_Idle)
    {
        /// Test if colliding
        if(M_Parent->M_Entity->IsColliding)
        {
          /*  /// Get WorldPosition
            Vector3 CollisionPosition = M_Parent->M_Entity->IsCollidingNode->GetWorldPosition();
            Vector3 CurrentPosition =  M_Parent->M_Entity->GetNode()->GetWorldPosition();

            /// Find difference in Vectors
            Vector3 PositionDifference = CollisionPosition-CurrentPosition;

            /// Set a difference Impulse
            M_Parent->M_Entity->ApplyImpulseTargetVector=-(PositionDifference*.3);

            /// set is colliding to false
            M_Parent->M_Entity-> IsCollidingNode = NULL;
            M_Parent->M_Entity-> IsColliding = false;*/
        }
        else if(changeFocusVector<=0.0f)
        {
            /// if no vector time then set a timeframe
            changeFocusVectorTime=Random(10.0f)+5.0f;
            changeFocusVector=changeFocusVectorTime;

            /// Get Position
            Vector3 RandomPosition(Random(8.0f)-4.0f, 0.0f, Random(8.0f)-4.0f);

            /// Save Positions
            //M_Parent->M_Entity->RootVector = M_Parent->M_Entity->GetNode()->GetWorldPosition();
            //M_Parent->M_Entity->RootRotation = M_Parent->M_Entity->GetNode()->GetWorldRotation();

            /// Save Random Position
            M_Parent->M_Entity->TargetVector = RandomPosition;
            M_Parent->M_Entity->ApplyImpulseTargetVector=(RandomPosition/changeFocusVectorTime)*.3;
        }
        else
        {
            /// Decrease focus time
            changeFocusVector-=dt;
        }
    }
    else
    {
        M_Parent->M_Entity->TargetVector=Vector3(Vector3::ZERO);
        M_Parent->M_Entity->ApplyImpulseTargetVector=Vector3(Vector3::ZERO);
    }
}

float AIFuzzyDroneStateIdle::CalculateActivation()
{

    /// Get activation level
    if(M_Parent->M_Logic==FuzzyLogic_Idle)
    {
        M_ActivationLevel = 1.0f;
    }
    else
    {
        M_ActivationLevel = 0.0f;
    }


    /// Check Bounds
    CheckBound(0.0f,1.0f);

    return M_ActivationLevel;
}


void AIFuzzyDroneStateIdle::Exit()
{
    return;
}

