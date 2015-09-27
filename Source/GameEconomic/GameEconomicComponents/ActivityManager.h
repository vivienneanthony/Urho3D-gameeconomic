#ifndef ActivityManager_H
#define ActivityManager_H


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

#include "../Activity.h"
#include "Resource.h"

using namespace std;
using namespace Urho3D;


/// Temporary ActivityManager
class ActivityManager : public LogicComponent
{
    OBJECT (ActivityManager);

public:
    ActivityManager(Context *context);
    virtual ~ActivityManager();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void Start();

   void HandleUpdate(StringHash eventType, VariantMap& eventData);

protected:
private:
    Vector<Activity *> * RootActivities;

    ActivityInformation * RootActivity;
};



#endif // ActivityManager_H
