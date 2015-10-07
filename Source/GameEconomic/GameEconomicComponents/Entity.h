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

#include <iostream>


using namespace std;
using namespace Urho3D;

/// Entity class
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
    virtual void HandleNodeCollision(StringHash eventType, Urho3D::VariantMap& eventData);
    virtual void ObjectCollision(Node* otherObject, Urho3D::VariantMap& eventData);

    virtual void FixedUpdate(StringHash eventType, Urho3D::VariantMap& eventData);

    /// Direction time
    float dirChangeTime;

protected:
private:
};

#endif // ENTITY_H
