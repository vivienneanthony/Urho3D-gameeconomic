#include "Entity.h"

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// define Player information
#include <iostream>

using namespace std;
using namespace Urho3D;

/// Entity destructor
Entity::Entity(Context *context)
    :LogicComponent(context)
{
    return;
}

/// Entity destructor
Entity::~Entity()
{
    return;
}

/// Entity Register Object
void Entity::RegisterObject(Context* context)
{
    context->RegisterFactory<Entity>();

    return;
}

/// Entity Start
void Entity::Start(void)
{
    return;
}

/// Entity Handle Collision
void Entity::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    return;
}

/// ENtity Object Collision
void Entity::ObjectCollision(Node* otherObject, VariantMap& eventData)
{
    return;
}
