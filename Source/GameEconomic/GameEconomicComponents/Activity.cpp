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


struct ActivityInformation
{
    // Idx

};

Activity::Activity(Context * context)
    :LogicComponent(context)
{
    //ctor
}

Activity::~Activity()
{
    //dtor
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

/// random minin missoin
bool Activity::MiningMission(Vector<ResourceGame> &InputResourcelist)
{
    /// do a Activity
    unsigned int planettype=0;

    unsigned int material1type= rand()%ResourceName->size();
    unsigned int material2type = rand()%ResourceName->size();
    unsigned int material1quant = rand()%1000;
    unsigned int material2quant = rand()%1000;

    InputResourcelist.Push( {material1type, material1quant});
    InputResourcelist.Push( {material2type, material2quant});

    return true;
}
