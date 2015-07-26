#ifndef ACTIVITY_H
#define ACTIVITY_H


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

#include "Resource.h"

using namespace std;
using namespace Urho3D;


/// Temporary activity
class Activity : public LogicComponent
{
    OBJECT (Activity);


public:
    Activity(Context *context);
    virtual ~Activity();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void Start();

    bool MiningMission(Vector<ResourceGame> &InputResourcelist);
protected:
private:
};



#endif // ACTIVITY_H
