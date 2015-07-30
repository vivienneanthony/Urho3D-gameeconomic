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
#include "Accounts.h"


using namespace std;
using namespace Urho3D;

/** @brief Accounts
  *
  * @todo: document this function
  */
Accounts::Accounts(Context *context):
    LogicComponent(context)
{
}

Accounts::~Accounts()
{
    //dtor
}

/// Entity Register Object
void Accounts::RegisterObject(Context* context)
{
    context->RegisterFactory<Accounts>();

    return;
}


void Accounts::Start(void)
{

    return;
}


