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

;

/// Build Default Resources
void ResourceManager::BuildBaseResources(void)
{
    cout << "it got here" << endl;

    /// loop through each one
    for(unsigned int idx=0; idx<95; idx++)
    {
        /// Create Resource
        ResourceComponentInformation  AddResource;

        /// insert information
        AddResource.Name = String(PeriodicNameIdentifiers[idx]);
        AddResource.Symbol = String(PeriodicSymbolIdentifiers[idx]);
        AddResource.Density = PeriodicDensityIdentifiers[idx];
        AddResource.ComponentState= (ResourceComponentState) PeriodicStateIdentifiers[idx];
        AddResource.ComponentType=(ResourceComponentType) PeriodicTypeIdentifiers[idx];
        AddResource.ComponentResource=String(PeriodicSymbolIdentifiers[idx]);



        /// If the component was created
        if(ResourceComponent * AddResourceObject = new ResourceComponent(context_))
        {
            /// If properties was set
            AddResourceObject -> AddProperties(AddResource);

            /// Push the resource
            ThisResources->Push(AddResourceObject);
        }
    }

    /// loop through each one
    for(unsigned int idx=0; idx<5; idx++)
    {
        /// Create Resource
        ResourceComponentInformation  AddResource;

        /// insert information
        AddResource.Name = String(RawMaterialNameIdentifiers[idx]);
        AddResource.Symbol = String(RawMaterialSymbolIdentifiers[idx]);
        AddResource.Density = RawMaterialDensityIdentifiers[idx];
        AddResource.ComponentState= (ResourceComponentState) RawMaterialStateIdentifiers[idx];
        AddResource.ComponentType=(ResourceComponentType) RawMaterialTypeIdentifiers[idx];
        AddResource.ComponentResource=String(RawMaterialSymbolIdentifiers[idx]);

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

/// loop through each one
    for(unsigned int idx=0; idx<6; idx++)
    {
        /// Create Resource
        ResourceComponentInformation  AddResource;

        /// insert information
        AddResource.Name = String(ComponentsToolsNameIdentifiers[idx]);
        AddResource.Symbol = String(ComponentsToolsSymbolIdentifiers[idx]);
        AddResource.Density = ComponentsToolsDensityIdentifiers[idx];
        AddResource.ComponentState= (ResourceComponentState) ComponentsToolsStateIdentifiers[idx];
        AddResource.ComponentType=(ResourceComponentType) ComponentsToolsTypeIdentifiers[idx];
        AddResource.ComponentResource=String(ComponentsToolsSymbolIdentifiers[idx]);


        /// If the component was created
        if(ResourceComponent * AddResourceObject = new ResourceComponent(context_))
        {
            /// If properties was set
            AddResourceObject -> AddProperties(AddResource);

            /// Push the resource
            ThisResources->Push(AddResourceObject);
        }
    }

    for(unsigned int idx=0; idx<27; idx++)
    {
        /// Create Resource
        ResourceComponentInformation  AddResource;

        /// insert information
        AddResource.Name = String(PrimaryResourcesNameIdentifiers[idx]);
        AddResource.Symbol = String(PrimaryResourcesSymbolIdentifiers[idx]);
        AddResource.Density = PrimaryResourcesDensityIdentifiers[idx];
        AddResource.ComponentState= (ResourceComponentState) PrimaryResourcesStateIdentifiers[idx];
        AddResource.ComponentType=(ResourceComponentType) PrimaryResourcesTypeIdentifiers[idx];
        AddResource.ComponentResource=String(PrimaryResourcesSymbolIdentifiers[idx]);

        /// If the component was created
        if(ResourceComponent * AddResourceObject = new ResourceComponent(context_))
        {
            /// If properties was set
            AddResourceObject -> AddProperties(AddResource);

            /// Push the resource
            ThisResources->Push(AddResourceObject);
        }
    }

    /// Add resources
    unsigned int idx=0;
    unsigned int offset=0;
    unsigned int i=0;

    /// loop and add
    do
    {
        /// Get the symbol name
        ///String ResourceSymbol = ThisResources->At(idx)->Symbol;
        ResourceComponentInformation * ThisResource = ThisResources->At(idx)->GetProperties();

        /// if name is water change offset
        if(ThisResource->ComponentType==RCType_RawMaterial&&offset<1000)
        {
            offset=1000;
            i=0;
        }

        if(ThisResource->ComponentType==RCType_Component&&offset<2000)
        {
            offset=2000;
            i=0;
        }

        if(ThisResource->ComponentType==RCType_Tool&&offset<3000)
        {
            offset=3000;
            i=0;
        }

        if(ThisResource->ComponentType==RCType_Structural&&offset<4000)
        {
            offset=4000;
            i=0;
        }

        if(ThisResource->ComponentType==RCType_Forcefield&&offset<5000)
        {
            offset=5000;
            i=0;
        }

        if(ThisResource->ComponentType==RCType_Light&&offset<6000)
        {
            offset=6000;
            i=0;
        }

        /// Create a new object
        TranslationTableResourceInformation newObject;
        newObject.ResourceID=offset+i;
        newObject.ResourceString=ThisResource->Symbol;
        newObject.ResourceType=ThisResource->ComponentType;

        cout << "Adding Resource to Resource Manager : " << offset+i << " " <<  ThisResource->Symbol.CString() << endl;

        /// Add object
        TranslationLookup.Push(newObject);

        /// increase counters
        i++;
        idx++;
    }

    while(idx<ThisResources->Size());

    return;
}

/// get resource
void ResourceManager::AddResourceToTableLookup(TranslationTableResourceInformation SaveResource)
{
    if(SaveResource.ResourceString.Empty())
    {
        return;

    }

    /// Add a new resource
    TranslationLookup.Push(SaveResource);

    return;
}

int ResourceManager::GetResourceSymbolInt(String InputString)
{
    /// below 1
    unsigned int result=-1;

    for(unsigned int idx=0; idx<TranslationLookup.Size(); idx++)
    {
        if(String(TranslationLookup.At(idx).ResourceString)==String(InputString))
        {
            result = TranslationLookup.At(idx).ResourceID;
            break;
        }
    }
    return result;
}


TranslationTableResourceInformation ResourceManager::GetResourceSymbolLookup(String InputString)
{
    /// below 1
    TranslationTableResourceInformation newObject;

    /// set valuues
    newObject.ResourceID=-1;
    newObject.ResourceString.Clear();
    newObject.ResourceType=RCType_None;

    for(unsigned int idx=0; idx<TranslationLookup.Size(); idx++)
    {
        if(String(TranslationLookup.At(idx).ResourceString)==String(InputString))
        {
            newObject.ResourceID= TranslationLookup.At(idx).ResourceID;
            newObject.ResourceType=TranslationLookup.At(idx).ResourceType;
            break;
        }
    }

    return newObject;
}
