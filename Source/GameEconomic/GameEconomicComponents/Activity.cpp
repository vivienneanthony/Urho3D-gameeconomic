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
#include "Resource.h"

using namespace std;
using namespace Urho3D;


Activity::Activity(Context * context)
    :LogicComponent(context)
    ,ThisActivities(NULL)
    ,ThisActivity(NULL)
{
    /// Clear any vector
    ThisActivities = new Vector<Activity *>(); //ctor
    ThisActivity = new ActivityInformation();
}

Activity::~Activity()
{
    /// Delete Activities from memory
    delete ThisActivities;
    delete ThisActivity;
}


/// Entity Register Object
void Activity::RegisterObject(Context* context)
{
    context->RegisterFactory<Activity>();

    return;
}

/// Start Function
void Activity::Start(void )
{
        //// Blank

        return;
}

/// Get total size of vectors
unsigned int Activity::GetTotalSize(void)
{
    /// set total acitivities to zero
    unsigned int TotalActivities = 0;

    /// Check vector for amount of activities
    for(unsigned int i =0;i<ThisActivities->Size();i++)
    {
        /// Get total size for each
        TotalActivities+=ThisActivities->At(i)->GetTotalSize();
    }

    /// Return total size
    return TotalActivities;
}
