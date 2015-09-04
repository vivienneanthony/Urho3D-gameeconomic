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
#include "ResourceManager.h"

using namespace std;
using namespace Urho3D;

/** @brief ResourceManager
  *
  * @todo: document this function
  */
ResourceManager::ResourceManager(Context *context):
    LogicComponent(context),
    ThisResourceManager(NULL)
{
    /// Allocate a new ResourceManager
    ThisResourceManager = new ResourceManagerInformation();

    /// Clear any vector
    ThisResources = new Vector<ResourceComponent *>();

    BuildBaseResources();

    return ;
}

/// Close the component - destructor
ResourceManager::~ResourceManager()
{
    /// Remove ResourceManager from memory

    delete ThisResources;

    delete ThisResourceManager;

    //dtor
}

/// Entity Register Object
void ResourceManager::RegisterObject(Context* context)
{
    context->RegisterFactory<ResourceManager>();

    return;
}

/// Build Default Resources
void ResourceManager::BuildBaseResources(void)
{
    cout << "it got here" << endl;

    /// loop through each one
    for(unsigned int idx=0; idx<94; idx++)
    {
        /// Create Resource
        ResourceComponentInformation  AddResource;

        AddResource.Name = String(PeriodicElementIdentifiers[idx]);
        AddResource.Symbol = String(PeriodicSymbolIdentifiers[idx]);
        AddResource.Density = PeriodicDensityIdentifiers[idx];
        AddResource.ComponentState=RCState_PeriodicUnidentified;
        AddResource.ComponentType=RCType_PeriodicElement;
        AddResource.ComponentResource=String(PeriodicSymbolIdentifiers[idx]);

        cout << "Attempt" << AddResource.Name.CString() << " with density of " << AddResource.Density << endl;

        /// If the component was created
        if(ResourceComponent * AddResourceObject = new ResourceComponent(context_))
        {
            /// If properties was set
            AddResourceObject -> AddProperties(AddResource);

            /// Push the resource
            ThisResources->Push(AddResourceObject);
        }
    }

    return;
}

