#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Engine/EngineEvents.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"


using namespace std;
using namespace Urho3D;

#include "AIFuzzyStateMachineStates.h"
#include "AIFuzzyStateMachineState.h"
#include "Entity.h"
#include "Drone.h"
#include "AIFuzzyControl.h"
#include "AIController.h"
#include "DroneAIController.h"
#include "AIFuzzyDroneStateIdle.h"

/// Constructor and Destructor
DroneAIController::DroneAIController(Context *context)
    :AIController(context)
    ,M_AIFuzzyControl(NULL)
    ,M_Machine(NULL)
    ,ThisDrone(NULL)
{

    /// Set the machine
    M_Machine = new AIFuzzyStateMachine(context);

    /// Update Mask
    SetUpdateEventMask(USE_FIXEDUPDATE);

    return;
}

/// Set Function
void DroneAIController::Set(AIMachineTypes machineType, AIFuzzyControl * aicontrols)
{
    /// Copy to local copy
    M_AIFuzzyControl = aicontrols;

    /// Set Machine type
    M_Machine->Set(machineType, aicontrols);


    return;
}

/// Add attached node
void DroneAIController::AddDrone(Drone * SetThisDrone)
{

    /// Attached node so controls can be seen
    M_AIFuzzyControl->SetAIDrone(SetThisDrone);
}


/// Adding states
void DroneAIController::AddStates(void)
{
    /// Add States
    M_Machine->AddState(new AIFuzzyDroneStateIdle(context_, (AIFuzzyControl *)M_AIFuzzyControl));

    /// Reset All
    M_Machine->Reset();

    return;
}


/// Destructor
DroneAIController::~DroneAIController()
{
    return;

}

/// Entity Register Object
void DroneAIController::RegisterObject(Context* context)
{
    context->RegisterFactory<DroneAIController>();

    return;
}


/// Drone Update
void DroneAIController::FixedUpdate(float timeStep)
{
    /// Update Perception
    Update(timeStep);

    return;
}


/// Drone Update
void DroneAIController::Update(float dt)
{
    /// Update Perception
    UpdatePerceptions(dt);

    /// update machine dt
    M_Machine->UpdateMachine(dt);

    return;
}

/// Update Perception
void DroneAIController::UpdatePerceptions(float dt)
{
    return;
}


void DroneAIController::HandleUpdate(StringHash eventType, VariantMap& eventData)
{

    using namespace Update;

    /// Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    Update(timeStep);
}
