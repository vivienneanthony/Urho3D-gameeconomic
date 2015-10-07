#ifndef DRONEAICONTROLLER_H
#define DRONEAICONTROLLER_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include "AIController.h"
#include "AIFuzzyControl.h"
///#include "AIFuzzyStateMachine.h"
#include "AIFuzzyStateMachineStates.h"

using namespace Urho3D;
using namespace std;


class AIFuzzyStateMachine;

/// Drone AIController class
class DroneAIController : public AIController
{
    OBJECT(DroneAIController);
public:
    /// Constructor and destructor
    DroneAIController(Context* context);
    virtual ~DroneAIController();

    /// Register the AIController
    static void RegisterObject(Context* context);

    /// Set AIControl
    virtual void Set(AIMachineTypes machineType, AIFuzzyControl * aicontrols);

    /// Add States
    virtual void AddStates(void);

    /// Update function - Urho FixedUpdate
    virtual void FixedUpdate(float dt);

    /// Update function - Specifc to the Manager
    virtual void Update(float dt);
    virtual void HandleUpdate(StringHash eventType, VariantMap& eventData);
    virtual void UpdatePerceptions(float dt);

    /// Add Drone
    virtual void AddDrone(Drone * SetThisDrone);

    /// Collision
    void HandleNodeCollision(StringHash eventType, Urho3D::VariantMap& eventData);
    void ObjectCollision(Node* otherObject, Urho3D::VariantMap& eventData);

    /// Not sure if it's being used yet
    SharedPtr<Drone> ThisDrone;

protected:
private:
    /// State machine pointer made it shared
    //AIFuzzyStateMachine * M_Machine;
    SharedPtr<AIFuzzyStateMachine> M_Machine;

    /// Shared Pointer to the AI Fuzzy Control
    SharedPtr<AIFuzzyControl> M_AIFuzzyControl;



};
#endif // DRONEAICONTROLLER_H
