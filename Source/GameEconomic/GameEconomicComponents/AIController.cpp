#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/IO/Deserializer.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Graphics/ParticleEffect.h>


#include "Entity.h"
#include "Drone.h"
#include "AIController.h"
#include "ResourceNodeComponent.h"

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;

/// Constructor
AIController::AIController(Context* context)
    : LogicComponent(context)
{
    colliding=0;

    return;
}

/// Destructor
AIController::~AIController()
{
    return;
}

/// star
void AIController::Start()
{
///    SubscribeToEvent(node_, E_NODECOLLISION, HANDLER(AIController, HandleNodeCollision));

    return;
}

/// Register
void AIController::RegisterObject(Context* context)
{
    context->RegisterFactory<AIController>();

    return;
}


/// star
void AIController::ResetAI(void)
{

    return;
}


/*
/// Object Collision
void AIController::ObjectCollision(Urho3D::Node* otherObject, Urho3D::VariantMap& eventData)
{
    return;
}


/// Drone control
void AIController::ControlDrone(Drone* ownDrone, Urho3D::Node* ownNode, float timeStep)
{
    /// return;
    /*  /// if ownnull and node is node return
      if (!ownDrone || !ownNode)
      {
          return;
      }

      /// If no target, walk idly
      ownDrone->controls.Set(CTRL_ALL, false);

      ownDrone->controls.Set(CTRL_UP, true); /// Set forward direction

      /// Hitting floor
      if(ownDrone->HittingFloor==true)
      {
          ownDrone->controls.Set(CTRL_ELEVATE,true);
      }

      /// Get TimeStep
      ownDrone->dirChangeTime -= timeStep;

      /// if time below change time
      if (ownDrone->dirChangeTime <= 0.0f)
      {
          ownDrone->dirChangeTime = 1.0f + Random(2.0f);

          /// choose a random direction
          float directionangle =  0.2f * (Random(600.0f) - 300.0f);

          ownDrone->rotationInertia= directionangle;

          /// Set left and right based on inertia
          if(directionangle<0.0f)
          {
              ownDrone->controls.Set(CTRL_LEFT,true);
          }
          else
          {
              ownDrone->controls.Set(CTRL_RIGHT,true);
          }
      }

    return;
}






*/
