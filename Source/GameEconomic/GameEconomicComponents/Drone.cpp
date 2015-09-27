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
#include "../../../Urho3D/Physics/PhysicsEvents.h"
#include "../../../Urho3D/Input/Controls.h"
#include "../../../Urho3D/Scene/Node.h"


#define CLOSEDMARKET    0
#define OPENMARKET      1
#define BLACKMARKET     2

#include <iostream>
#include <vector>


#include "Entity.h"
#include "Drone.h"
#include "ResourceNodeComponent.h"

using namespace std;
using namespace Urho3D;

///Drone Constructor
Drone::Drone(Context * context):
    Entity(context)
    ,ThisDrone(NULL)
    ,ThisBody(NULL)
    ,TargetNode(NULL)
    ,TargetVector(Vector3(0.0f,0.0f,0.0f))
{
    /// Create a new Drone
    ThisDrone = new DroneInformation();

    /// Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);

}

/// Destructor
Drone::~Drone()
{
    //dtor
}

/// Entity Register Object
void Drone::RegisterObject(Context* context)
{
    context->RegisterFactory<Drone>();

    return;
}

/// Start Drone or reset
void Drone::Start(void)
{

    return;
}

void Drone::Initialize(void)
{
    return;
}

/// Set Parameters for loading resource
void Drone::SetParameters(DroneInformation &DBDrone)
{
    /// Copy values to drone
    ThisDrone->Name = DBDrone.Name;
    ThisDrone->DroneType = DBDrone.DroneType;
    ThisDrone->AlienRace = DBDrone.AlienRace;
    ThisDrone->AlienAllianceAligned = DBDrone.AlienAllianceAligned;

    /// Set node global so it's not looked up twice
    ThisBody = this->GetNode()->GetComponent<RigidBody>();

    /// Set node global so it's not looked up twice
    //ThisBody = this->GetNode()->GetComponent<ResourceNodeComponent>();

    return;
}

/// Set Controls
void Drone::SetControls(const Controls& newControls)
{
    controls = newControls;
}

/// Drone Fix Update
void Drone::FixedUpdate(float timeStep)
{
    /// Force the physics rotation
    Quaternion rotationq= ThisBody->GetRotation();

    /// ControlDrone
    controller->ControlDrone(this, this->GetNode(), timeStep);

    /// move force
    Vector3 moveForce = Vector3::ZERO;

    Quaternion newrotation;

    /// If a rigid body is assigned;
    if(ThisBody)
    {
        /// apply left and right and foward and backwards
        if (controls.IsDown(CTRL_UP | CTRL_DOWN))
        {
            if (controls.IsDown(CTRL_UP))
                moveForce += rotationq * Vector3(0, 0, 1);
            if (controls.IsDown(CTRL_DOWN))
                moveForce += rotationq * Vector3(0, 0, -1);

            /// Normalize so that diagonal strafing isn't faster
            moveForce.Normalize();
            moveForce *= .02;
            ThisBody->ApplyImpulse(moveForce);
        }

        /// turn right or left based on rotationq
        if (controls.IsDown(CTRL_RIGHT)||controls.IsDown(CTRL_LEFT))
        {
            if (controls.IsDown(CTRL_LEFT))
            {
                newrotation = rotationq * Quaternion(-(rotationInertia), Vector3(0.0f, 1.0f, 0.0f)); // Pitch
            }

            if (controls.IsDown(CTRL_RIGHT))
            {
                newrotation = rotationq * Quaternion(rotationInertia, Vector3(0.0f, 1.0f, 0.0f)); // Pitch
            }

            ThisBody->SetRotation(newrotation);
        }

        /// Apply vertical force
        if(controls.IsDown(CTRL_ELEVATE))
        {
            moveForce += moveForce * Vector3(0, 1, 0);

            /// Normalize so that diagonal strafing isn't faster
            moveForce.Normalize();
            moveForce *= .2;
            ThisBody->ApplyImpulse(moveForce);
        }
    }

       HittingObject=false;
}

/// Hail Drone
bool Drone::Hail(void)
{
    return false;
}

/// Handle Collision
void Drone::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollision;

    /// Get Collision Node
    Node* otherNode = (Node*)eventData[P_OTHERNODE].GetPtr();

    /// If there is a another node then it must be a object collision
    if (otherNode != NULL)
    {
        ObjectCollision(otherNode, eventData);
    }

    return;
}

/// Other drone Object Collision
void Drone::ObjectCollision(Node* otherObject, VariantMap& eventData)
{
    /// Get node info
    ResourceNodeComponent * OtherObjectResource = otherObject->GetComponent<ResourceNodeComponent>();

    /// Get resource node
    if(OtherObjectResource)
    {
        /// Get node information
        ResourceComponentType ResourceHit=OtherObjectResource->GetResourceComponentType();
        String ResourceName=OtherObjectResource->GetResourceComponentName();

        /// Set hitting floor to zero
        HittingFloor=false;
        HittingObject=false;

        /// Change type
        if(ResourceName==String("Generic_Type1"))
        {
            HittingFloor=true;
            cout << ResourceName.CString()<< endl;
        }
        else
        {
            HittingObject=true;
        }

    }

    return;
}

