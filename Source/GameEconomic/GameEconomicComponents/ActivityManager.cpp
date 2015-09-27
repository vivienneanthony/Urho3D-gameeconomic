#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include "Activity.h"
#include "ActivityManager.h"
#include "Resource.h"

using namespace std;
using namespace Urho3D;


struct ActivityManagerInformation
{
    // Idx

};

ActivityManager::ActivityManager(Context * context)
    :LogicComponent(context)
    ,RootActivities(NULL)
    ,RootActivity(NULL)
{
    /// Clear any vector
    RootActivities = new Vector<Activity *>();


    RootActivity = new ActivityInformation();

    /// Subscribe to event
    SubscribeToEvent(E_UPDATE,HANDLER(ActivityManager,HandleUpdate));


}

ActivityManager::~ActivityManager()
{
    delete RootActivities;
    delete RootActivity;
}


/// Entity Register Object
void ActivityManager::RegisterObject(Context* context)
{
    context->RegisterFactory<ActivityManager>();

    return;
}

/// Start Function
void ActivityManager::Start(void )
{
    return;
}

void ActivityManager::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    unsigned int TotalActivities=0;

    for(unsigned int i=0; i<RootActivities->Size(); i++)
    {
        TotalActivities+=RootActivities->At(i)->GetTotalSize();
    }

    //cout << TotalActivities << endl;
    return;
}
