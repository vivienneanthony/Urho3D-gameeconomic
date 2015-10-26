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
#include "../../../Urho3D/Scene/Scene.h"

#define CLOSEDMARKET    0
#define OPENMARKET      1
#define BLACKMARKET     2

#include <iostream>
#include <vector>

#include "Entity.h"
#include "Drone.h"
#include "AIFuzzyControl.h"
#include "AIFuzzyStateMachine.h"
#include "AIFuzzyStateMachineState.h"
#include "DroneAIController.h"
#include "ResourceNodeComponent.h"

using namespace std;
using namespace Urho3D;


///Drone Constructor
Drone::Drone(Context * context):
    Entity(context)
    ,ThisDrone(NULL)
    ,ThisBody(NULL)
    ,ThisNode(NULL)
    ,IsColliding(false)
    ,IsCollidingNode(NULL)
    ,M_AIFuzzyControl(NULL)
    ,DroneController(NULL)
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
    /// Delete
    delete M_AIFuzzyControl;
    delete ThisDrone;

    return;
}

/// Entity Register Object
void Drone::RegisterObject(Context* context)
{
    /// Register Drone
    context->RegisterFactory<Drone>();

    return;
}

/// Start Drone or reset
void Drone::Start(void)
{
    /// Do nothing
    return;
}

/// Set Node
void Drone::SetNode(Node * SetNode)
{
    /// Set node
    ThisNode = SetNode;

    return;
}

/// Initialize
void Drone::Initialize(void)
{
    /// If no node do nothing
    if(!ThisNode)
    {
        return;
    }

    /// New Drone AI Controller
    DroneController =  ThisNode->CreateComponent<DroneAIController>();

    /// Add AIFuzzyControl
    M_AIFuzzyControl = ThisNode->CreateComponent<AIFuzzyControl>();

    /// Set Fuzzy Control
    DroneController->Set(FUSH_MACHINE_DRONE100,M_AIFuzzyControl);

    /// Add dron
    DroneController->AddDrone(ThisNode->GetComponent<Drone>());

    /// Add States
    DroneController->AddStates();

    /// Choose
    SubscribeToEvent(node_, E_NODECOLLISION, HANDLER(Drone, HandleNodeCollision));

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

    return;
}

/// Set Controls
void Drone::SetControls(const Controls& newControls)
{
    /// Set Controls
    controls = newControls;

    return;
}

/// Drone Fix Update
void Drone::FixedUpdate(float timeStep)
{
    /// Do nothing

    ///ApplyImpulseTargetVector.Normalize();
    if(ThisNode&&ThisBody)
    {
        if(IsColliding)
        {
            /// Get the current angular velocity on collision
            Vector3 ThisAngularVelocity = ThisBody->GetAngularVelocity();
            Vector3 ThisLinearVelocity = ThisBody->GetLinearVelocity();

            /// Simply reverse the velocity for now
            ThisBody->SetAngularVelocity(-ThisAngularVelocity);
            ThisBody->SetLinearVelocity(-ThisLinearVelocity);

            /// Set false for Idle can move on
            IsColliding=false;
            IsCollidingNode=NULL;
        }

        /// Apply Impulse
        Vector3 BaseImpulse=Vector3::ZERO;

        ///  Position is lower then 1.5
        if(ThisNode->GetWorldPosition().y_<=2.0f)
        {
            BaseImpulse+=Vector3(0.0f,0.2f,0.0f);
        }

        /// If not colliding add more impulse
        if(!IsColliding)
        {
            BaseImpulse+=ApplyImpulseTargetVector;
        }

        /// Apply impulse
        ThisBody->ApplyImpulse(BaseImpulse);

    }

    Vector3 Target(Vector3(3.0f,3.0f,3.0f).Normalized());

    Vector3 Start = ThisBody->GetPosition().Normalized();

    Quaternion rot;
    rot.FromRotationTo(Start,Target);

    ThisBody->SetRotation(rot);

    return;
}

/// Hail Drone
bool Drone::Hail(void)
{
    /// Do nothing
    return false;
}

/// Handle Collision
void Drone::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    /// Node Collision
    using namespace NodeCollision;

    /// Get Collision Node
    Node* otherNode = (Node*)eventData[P_OTHERNODE].GetPtr();

    /// Set Collision
    IsColliding=true;
    IsCollidingNode=NULL;

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
    /// Is colliding last collision;
    IsCollidingNode = otherObject;

    return;
}


Quaternion Drone::getRotationQuat(const Vector3& from, const Vector3& dest, const Vector3& fallbackAxis = Vector3::ZERO)
{
    Quaternion result;

    Vector3 H = from+dest;
    H = H.Normalized();

    result.w_ = from.DotProduct(H);
    result.x_ = from.y_*H.z_ - from.z_*H.y_;
    result.y_ = from.z_*H.x_ - from.x_*H.z_;
    result.z_ = from.x_*H.y_ - from.y_*H.x_;

    return result;
}
