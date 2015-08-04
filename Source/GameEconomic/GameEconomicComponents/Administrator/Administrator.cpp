#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// header
#include "Administrators.h"

using namespace std;
using namespace Urho3D;

/** @brief Administrators
  *
  * @todo: document this function
  */
Administrators::Administrators(Context *context):
    LogicComponent(context)
{
}

Administrators::~Administrators()
{
    //dtor
}

/// Entity Register Object
void Administrators::RegisterObject(Context* context)
{
    context->RegisterFactory<Administrators>();

    return;
}


void Administrators::Start(void)
{

    return;
}


