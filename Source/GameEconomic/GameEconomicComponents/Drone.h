#ifndef Drone_H
#define Drone_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Physics/RigidBody.h"
#include "../../../Urho3D/Physics/CollisionShape.h"
#include "../../../Urho3D/Input/Controls.h"

#include <iostream>

#include "DroneAIController.h"

#include "Entity.h"
#include "../Drone.h"

#include "AIFuzzyStateMachineStates.h"
#include "AIFuzzyStateMachine.h"
#include "AIFuzzyControl.h"


using namespace std;
using namespace Urho3D;

/// Drone class
class Drone : public Entity
{
    OBJECT(Drone);

public:
    Drone(Context * context);
    virtual  ~Drone();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    virtual void SetParameters(DroneInformation &DBDrone);

    virtual void Start(void);

    /// Fixed Update
    virtual void SetControls(const Controls& newControls);

    virtual void Initialize(void);

    /// Other functions
    virtual bool Hail(void);

    virtual void HandleNodeCollision(StringHash eventType, Urho3D::VariantMap& eventData);
    virtual void ObjectCollision(Node* otherObject, Urho3D::VariantMap& eventData);
    virtual void SetNode(Node * SetNode);
    virtual void FixedUpdate(float timeStep);

    /// Movement controls. Assigned by the main program each frame.
    Controls controls;
    Controls prevControls;

    /// Hitting Floor
    bool HittingFloor;
    bool HittingObject;
    bool IsColliding;
    Node * IsCollidingNode;

    /// Target spot and vector
    Vector3 TargetVector;
    Vector3 ApplyImpulseTargetVector;

    /// Root Positions
    Vector3 RootVector;
    Quaternion RootRotation;

    /// Information
    float rotationInertia;

protected:
    /// Fuzzy Control
    SharedPtr<AIFuzzyControl> M_AIFuzzyControl;
    SharedPtr<DroneAIController> DroneController;
    SharedPtr<Node> ThisNode;

private:
    /// Drone Information
    DroneInformation * ThisDrone;
    RigidBody * ThisBody;

};

#endif // Drone_H
