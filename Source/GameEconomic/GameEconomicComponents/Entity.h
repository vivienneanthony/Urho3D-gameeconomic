#ifndef ENTITY_H
#define ENTITY_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Input/Controls.h"

/// define Player information
#include <iostream>

#include "Resource.h"

using namespace std;
using namespace Urho3D;

class Entity : public LogicComponent
{
    OBJECT(Entity);

public:
    Entity(Context *context);
    virtual ~Entity();

     /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    virtual void Start(void);

    /// Collision handler
    virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
    virtual void ObjectCollision(Node* otherObject, VariantMap& eventData);

    float dirChangeTime;

protected:

private:


};

#endif // ENTITY_H
