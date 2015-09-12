/// Urho Related Includes
#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// Created Resources
#include "ResourceComponent.h"

using namespace std;
using namespace Urho3D;

/** @brief ResourceComponent
  *
  * @todo: document this function
  */
ResourceComponent::ResourceComponent(Context *context):
    Object(context),
    ThisResourceComponent(NULL)
{
    /// Allocate a new ResourceComponent
    ThisResourceComponent = new ResourceComponentInformation();

    return;
}

/// Close the component - destructor
ResourceComponent::~ResourceComponent()
{
    /// Remove ResourceComponent from memory
    delete ThisResourceComponent;

    return;
}

/// Add ResourceComponent
bool ResourceComponent::AddProperties(ResourceComponentInformation ComponentInformation)
{
    /*
    /// If the name is empty then return false
    if (ComponentInformation.Name.Empty()||ComponentInformation.Name.Front()<'A'||ComponentInformation.Name.Front()>'Z')
    {
        return false;
    }

    /// If density is below 0
    if(ComponentInformation.Density<=0.0f);
    {
        return false;
    }

    /// If symbol name is empty then quit
    if (ComponentInformation.Symbol.Empty()||ComponentInformation.Symbol.Front()<'A'||ComponentInformation.Symbol.Front()>'Z')
    {
        return false;
    }

    /// If symbol name is empty then quit
    if (ComponentInformation.ComponentResource.Empty()||ComponentInformation.ComponentResource.Front()<'A'||ComponentInformation.ComponentResource.Front()>'Z')
    {
        return false;
    }
*/
    /// Component
    ThisResourceComponent->Name=ComponentInformation.Name;
    ThisResourceComponent->ComponentResource=ComponentInformation.ComponentResource;
    ThisResourceComponent->Symbol=ComponentInformation.Symbol;
    ThisResourceComponent->Density=ComponentInformation.Density;
    ThisResourceComponent->ComponentState=ComponentInformation.ComponentState;
    ThisResourceComponent->ComponentType=ComponentInformation.ComponentType;

    cout << ThisResourceComponent->Symbol.CString()<< endl;

    return true;
}

/// use this to return null if need be
ResourceComponentInformation * ResourceComponent::GetProperties(void)
{
    /// Create component information
    ResourceComponentInformation * ComponentInformation;

    /// Save Information
    ComponentInformation = new ResourceComponentInformation();

    /// Component
    ComponentInformation->Name=ThisResourceComponent->Name;
    ComponentInformation->ComponentResource=ThisResourceComponent->ComponentResource;
    ComponentInformation->Symbol=ThisResourceComponent->Symbol;
    ComponentInformation->Density=ThisResourceComponent->Density;
    ComponentInformation->ComponentState=ThisResourceComponent->ComponentState;
    ComponentInformation->ComponentType=ThisResourceComponent->ComponentType;

    return ComponentInformation;
}

/// Entity Register Object
void ResourceComponent::RegisterObject(Context* context)
{
    /// Register Factory
    context->RegisterFactory<ResourceComponent>();

    return;
}

