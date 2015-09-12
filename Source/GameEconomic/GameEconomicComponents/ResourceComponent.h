#ifndef ResourceComponent_H
#define ResourceComponent_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include <iostream>

using namespace std;
using namespace Urho3D;

#include "../ResourceComponent.h"

/// ResourceComponent class
class ResourceComponent : public Object
{
    OBJECT(ResourceComponent);
public:
    /// public functions
    ResourceComponent(Context *context);
    virtual ~ResourceComponent();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    bool AddProperties(ResourceComponentInformation ComponentInformation);
    ResourceComponentInformation * GetProperties(void);

protected:
private:
    ResourceComponentInformation * ThisResourceComponent;
};

#endif // ResourceComponent_H
