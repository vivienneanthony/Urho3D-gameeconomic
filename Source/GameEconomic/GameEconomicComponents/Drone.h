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

/// define Player information
#include <iostream>

#include "Resource.h"

using namespace std;
using namespace Urho3D;

#include "../Drone.h"
#include "Entity.h"
#include "AIController.h"

#define Drone_ENTITY           0



using namespace std;

class AIController;

class Drone :  public Entity
{

    OBJECT (Drone);

public:
    Drone(Context * context);
    virtual ~Drone();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    virtual void SetParameters(DroneInformation &DBDrone);

    virtual void Start(void);

    virtual void FixedUpdate(float timeStep);
    virtual void SetControls(const Controls& newControls);

    virtual void Initialize(void);

    /// Other functions
    virtual bool Hail(void);

    virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
    virtual void ObjectCollision(Node* otherObject, VariantMap& eventData);

    /// Movement controls. Assigned by the main program each frame.
    Controls controls;
    Controls prevControls;

    bool HittingFloor;
    bool HittingObject;

    /// Target spot and vector
    Vector3 TargetVector;
    Node * TargetNode;

    float rotationInertia;

private:
    /// Drone Information
    DroneInformation * ThisDrone;
    RigidBody * ThisBody;

    SharedPtr<AIController> controller;

};

#endif // Drone_H
