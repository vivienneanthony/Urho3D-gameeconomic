//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Graphics/AnimationController.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/IO/MemoryBuffer.h"
#include "../../../Urho3D/Physics/PhysicsEvents.h"
#include "../../../Urho3D/Physics/PhysicsWorld.h"
#include "../../../Urho3D/Physics/RigidBody.h"
#include "../../../Urho3D/Scene/Scene.h"
#include "../../../Urho3D/Scene/SceneEvents.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Core/Timer.h"
#include "../../../Urho3D/Graphics/Light.h"
#include "../../../Urho3D/Graphics/StaticModel.h"

#include "ResourceNodeComponent.h"
#include "ResourceComponent.h"
#include "ResourceManager.h"
#include "PowerComponent.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <locale>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <algorithm>


using namespace Urho3D;

using namespace std;
/// Base code
ResourceNodeComponent::ResourceNodeComponent(Context* context) :
    LogicComponent(context),
    ResourceNode_Type(RCType_None),
    IsOnlineFlag(false),
    ChangePowerSourceLightChange(0.0f),
    ChangePowerSourceLightTime(0.0f)
{
    /// Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);

    /// Empty the string
    ResourceNode_Name.Clear();
    ResourceNode_Type=RCType_None;

    return;
}

/// Registering a object
void ResourceNodeComponent::RegisterObject(Context* context)
{
    context->RegisterFactory<ResourceNodeComponent>("Existence");

    /// These macros register the class attributes to the Context for automatic load / save handling.
    // We specify the Default attribute mode which means it will be used both for saving into file, and network replication
    ATTRIBUTE("Resource Name", String,  ResourceNode_Name, String(""), AM_DEFAULT);
    ATTRIBUTE("Resource Type", uint, ResourceNode_Type, RCType_None,AM_DEFAULT);

    return;
}

/// Creation of a game object
void ResourceNodeComponent::Start()
{
    return;
}

/// Get Resource Type;
ResourceComponentType ResourceNodeComponent::GetResourceComponentType(void)
{
    /// Return the resource type
    return ResourceNode_Type;
}

/// Get Resource Name;
String ResourceNodeComponent::GetResourceComponentName(void)
{
    /// Return the resource Name
    return ResourceNode_Name;
}


/// Get Resource Type;
void ResourceNodeComponent::SetResourceComponentType(ResourceComponentType SetType)
{
    /// Return the resource type
    ResourceNode_Type=SetType;

    return;
}

/// Get Resource Type;
void ResourceNodeComponent::SetResourceComponentNameType(String ResourceName, ResourceComponentType SetType)
{
    /// Return the resource type
    ResourceNode_Name=ResourceName;
    ResourceNode_Type=SetType;

    return;
}

void ResourceNodeComponent::MapResources(ResourceManager * RefResourceManager)
{
/// Add a resource node
    ResourceNodeComponent * ResourceSet = this->GetNode()->GetComponent<ResourceNodeComponent>();

    /// get type and name
    String ResourceName=ResourceSet ->GetResourceComponentName();
    ResourceComponentType ResourceType = ResourceSet->GetResourceComponentType();

    /// Get component information if available works
    unsigned int ResourceIDX= RefResourceManager->GetResourceSymbolIdxInt(ResourceName);
    ResourceComponentInformation * ResourceInfo = RefResourceManager -> GetResourceInfo(ResourceIDX);

    /// If ResourceSet
    if(ResourceInfo&&ResourceSet)
    {

        cout << ResourceInfo->ComponentResource.CString() << endl;

        /// split inot a string
        stringstream ss(ResourceInfo->ComponentResource.CString());
        string item;
        vector<string> results;
        while (getline(ss, item, ':'))
        {
            results.push_back(item);
        }

        unsigned int idx=0;

        while(idx<results.size())
        {
            String Name=String(results.at(idx).c_str());
            String Quantity=String(results.at(idx+1).c_str());

            ResourceNodeInformation NewResource;

            NewResource.Resource.ResourceName = Name;
            NewResource.Resource.Quantity = strtoul(Quantity.CString(),NULL,0);
            NewResource.IntregrationType=RI_Permament;


            NodeResources.Push(NewResource);

            cout << "Adding " << Name.CString() << endl;
            idx+=2;
        }

    }
}

void ResourceNodeComponent::SetPower(double SetPowerTo)
{
    Node * ThisNode = this->GetNode();
    PowerComponent * ThisPower = ThisNode->GetComponent<PowerComponent>();

    /// If SetPower Exist
    if(ThisPower)
    {
        ThisPower->SetPower(SetPowerTo);
    }
}

bool ResourceNodeComponent::IsOnline(void)
{
    return IsOnlineFlag;
}

void ResourceNodeComponent::AddNodeResource(ResourceNodeInformation NewResource)
{
    /// Resource
    NodeResources.Push(NewResource);

    return;
}

/// Fix update
void ResourceNodeComponent::FixedUpdate(float timeStep)
{
    /// set Power requirement to 400
    Node * ThisNode = this->GetNode();
    PowerComponent * ThisPower = ThisNode->GetComponent<PowerComponent>();

    bool IsOnCurrently = false;
    double CurrentInputPower = 0.0f;
    double CurrentOutputPower = 0.0f;
    double MininiumPowerRequirement = 0.0f;

    if(ThisPower)
    {
        /// Set Minimum Requirement
        CurrentInputPower = ThisPower->GetInputPower();

        CurrentOutputPower = ThisPower->GetPower();

        IsOnCurrently = ThisPower->GetPowerOn();
    }


    /// Iff resource is a light
    if(ResourceNode_Type==RCType_Light)
    {
        MininiumPowerRequirement = 400.0f;

        /// Get Light
        Node * ThisLightNode = ThisNode->GetChild("Generic_Light");
        Light * ThisLight = ThisLightNode->GetComponent<Light>();

        /// If CurrentInputPower less then the minimium requirement
        if(CurrentInputPower<MininiumPowerRequirement)
        {
            /// if Online then turn offline
            if(IsOnlineFlag==true||IsOnCurrently==true)
            {
                /// Set is Onflag to true;
                IsOnlineFlag=false;

                /// Turn Light Off
                ThisLight->SetBrightness(0.0f);
            }

            /// set off
            ThisPower->SetPowerOn(false);
        }
        else
        {
            /// If there is a change between the online flag and on currently
            if(IsOnCurrently!=IsOnlineFlag)
            {
                /// If Online
                if(IsOnCurrently)
                {
                    /// Compute usablee range for light
                    double CreatePower= CurrentInputPower/MininiumPowerRequirement;

                    /// make sure power is 1.0f or below
                    CreatePower = CreatePower>1.0f?1.0f:CreatePower;

                    CreatePower*=.37f;  /// Set Resistance

                    ThisLight->SetBrightness(CreatePower);

                    IsOnlineFlag=true;
                }
                else
                {
                    ThisLight->SetBrightness(0.0f);

                    IsOnlineFlag=false;
                }
            }
        }
    }

    /// If resource is a light
    if(ResourceNode_Type==RCType_Forcefield)
    {
        /// Set Minimum Requirement
        MininiumPowerRequirement = 3200.0f;

        /// If CurrentInputPower less then the minimium requirement
        if(CurrentInputPower<MininiumPowerRequirement)
        {
            if(IsOnCurrently==true)
            {
                /// set off
                ThisPower->SetPowerOn(false);
            }
        }
    }

    /// If resource is a light
    if(ResourceNode_Type==RCType_RefrigerationUnit)
    {
        /// Set Minimum Requirement
        MininiumPowerRequirement = 10.0f;

        /// If CurrentInputPower less then the minimium requirement
        if(CurrentInputPower<MininiumPowerRequirement)
        {
            if(IsOnCurrently==true)
            {
                /// set off
                ThisPower->SetPowerOn(false);
            }
        }
    }

    /// If resource is a light
    if(ResourceNode_Type==RCType_ReplicationPrinter)
    {
        MininiumPowerRequirement = 10.0f;

        /// Get Light
        Node * ThisLightNode = ThisNode->GetChild("Generic_Light");
        Light * ThisLight = ThisLightNode->GetComponent<Light>();


        /// If CurrentInputPower less then the minimium requirement
        if(CurrentInputPower<MininiumPowerRequirement)
        {
            if(IsOnCurrently==true)
            {
                ThisLight->SetBrightness(0.0f);

                /// Set is Onflag to true;
                IsOnlineFlag=false;

            }

            /// set off
            ThisPower->SetPowerOn(false);

        }
        else
        {

            /// If there is a change between the online flag and on currently
            if(IsOnCurrently!=IsOnlineFlag)
            {
                /// If Online
                if(IsOnCurrently)
                {
                    ThisLight->SetBrightness(1.0f);

                    IsOnlineFlag=true;
                }
                else
                {
                    ThisLight->SetBrightness(0.0f);

                    IsOnlineFlag=false;
                }
            }
        }
    }

    /// If resource is a light
    if(ResourceNode_Type==RCType_PowerSource)
    {
        if(ChangePowerSourceLightChange<=0.0f)
        {
            /// if no vector time then set a timeframe
            ChangePowerSourceLightTime=Random(1.0f)+1.0f;

            ChangePowerSourceLightChange=ChangePowerSourceLightTime;

            /// Get Light
            Node * ThisLightNode = ThisNode->GetChild("Generic_Light");
            Light * ThisLight = ThisLightNode->GetComponent<Light>();

            ThisLight->SetBrightness(Random(0.3f)+0.6f);
        }
        else
        {
            /// Decrease focus time
            ChangePowerSourceLightChange-=timeStep;
        }
    }

    return;
}

unsigned int ResourceNodeComponent::TotalNodeResources(void)
{
    return NodeResources.Size();

}


ResourceNodeInformation ResourceNodeComponent::GetNodeResource(unsigned int index)
{
    return NodeResources.At(index);
}
