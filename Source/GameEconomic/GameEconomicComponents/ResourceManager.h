#ifndef ResourceManager_H
#define ResourceManager_H

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

#include "../ResourceManager.h"
#include "ResourceComponent.h"

/// ResourceManager class
class ResourceManager : public LogicComponent
{
    OBJECT(ResourceManager);

public:
    /// public functions
    ResourceManager(Context *context);
    virtual ~ResourceManager();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void BuildBaseResources(void);
protected:

private:
    ResourceManagerInformation * ThisResourceManager;

    Vector<ResourceComponent *>  * ThisResources;
};

#endif // ResourceManager_H
