#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include <iostream>
#include <vector>



#include "Starbase.h"
#include "ResourceNodeComponent.h"
#include "PowerComponent.h"
#include "../ResourceComponent.h"


using namespace std;
using namespace Urho3D;




/// couple of functions needed
/// getcurrent list of items
Starbase::Starbase(Context * context):
    LogicComponent(context)
    ,TotalPower(0.0f)
    ,UsedPower(0.0f)
    ,Oxygen(0.0f)
    ,Name(NULL)
    ,StarbaseNodes(NULL)
    ,IsOnline(false)
{

    /// Create a new vector
    StarbaseNodes = new Vector<StarbaseNodeInformation>();

    /// Clear irrelevant information
    StarbaseNodes-> Empty();

    return;
}

/// Starbase destructor
Starbase::~Starbase()
{

    delete StarbaseNodes;

    /// Do nothing

    return;
}

/// Register Starbase
void Starbase::RegisterObject(Context* context)
{
    /// Register Starbase
    context->RegisterFactory<Starbase>();

    return;
}

/// Starbase initialization
void Starbase::Start(void)
{

    /// Do nothing

    return;
}

/// Public Variables
unsigned int Starbase::GetBaseNodes(void)
{
    return StarbaseNodes->Size();
}

/// Get node information
StarbaseNodeInformation Starbase::GetBaseNode(unsigned int idx)
{
    StarbaseNodeInformation ReturnNode = StarbaseNodes->At(idx);

    return ReturnNode;
}

/// Get total power
double Starbase::GetTotalPower(void)
{
    return TotalPower;
}

/// Get used power
double Starbase::GetUsedPower(void)
{
    return UsedPower;
}

/// Add node to the starbase
void Starbase::PushNode(Node * NewNode, ResourceComponentType NewNodeComponentType)
{
    /// Create a new node
    StarbaseNodeInformation SaveNode;

    SaveNode.StarbaseNode=NewNode;
    SaveNode.StarbaseNodeType=NewNodeComponentType;

    StarbaseNodes->Push(SaveNode);

    return;
}

/// Set online status
void Starbase::SetOnline(bool SetFlag)
{
    IsOnline=SetFlag;

    return;
}

/// Fix update
void Starbase::FixedUpdate(float timeStep)
{
    /// If the base is online
    if(IsOnline)
    {
        /// Set totalpower
        TotalPower=0.0f;

        /// Get Total Power
        for(unsigned int idx=0; idx<StarbaseNodes->Size(); idx++)
        {
            Node * BaseNode = StarbaseNodes->At(idx).StarbaseNode;
            ResourceNodeComponent * BaseNodeResourceNodeComponent = BaseNode -> GetComponent<ResourceNodeComponent>();

            /// if component exist
            if(BaseNodeResourceNodeComponent)
            {
                /// If resourcetype is a power
                if(BaseNodeResourceNodeComponent->GetResourceComponentType()==RCType_PowerSource)
                {
                    PowerComponent * BaseNodePowerComponent = BaseNode->GetComponent<PowerComponent>();

                    if(BaseNodePowerComponent)
                    {
                        TotalPower+= BaseNodePowerComponent->GetPower();
                    }
                }
            }
        }

        /// Create a remaining power
        double RemainingPower=TotalPower;
        double output=0.0f;

        /// Get Total Power
        for(unsigned int idx=0; idx<StarbaseNodes->Size(); idx++)
        {
            Node * BaseNode = StarbaseNodes->At(idx).StarbaseNode;
            ResourceNodeComponent * BaseNodeResourceNodeComponent = BaseNode -> GetComponent<ResourceNodeComponent>();
            PowerComponent * Power = BaseNode->GetComponent<PowerComponent>();


            /// if component exist
            if(BaseNodeResourceNodeComponent)
            {
                /// If resourcetype is a power
                if(BaseNodeResourceNodeComponent->GetResourceComponentType()==RCType_Light)
                {
                    if(RemainingPower-400.0f>0.0f)
                    {
                        /// Set power if its enougth
                        Power->SetPower(400.0f);

                        output=Power->GetPower();

                        RemainingPower-=400.0f-output; //0
                    }
                    else
                    {
                    }
                }

                /// Force field
                if(BaseNodeResourceNodeComponent->GetResourceComponentType()==RCType_Forcefield)
                {
                    if(RemainingPower-3200.0f>0.0f)
                    {
                        /// Set power if its enougth
                        Power ->SetPower(3200.0f);

                        output=Power->GetPower();

                        RemainingPower-=3200.0f-output;

                    }
                    else
                    {
                        BaseNodeResourceNodeComponent->SetPower(0.0f);
                    }
                }

                /// Force Refrigeration Unit
                if(BaseNodeResourceNodeComponent->GetResourceComponentType()==RCType_RefrigerationUnit)
                {
                    if(RemainingPower-10.0f>0.0f)
                    {
                        /// Set power if its enougth
                        Power->SetPower(10.0f);

                        output=Power->GetPower();

                        RemainingPower-=10.0f-output;

                    }
                    else
                    {
                        BaseNodeResourceNodeComponent->SetPower(0.0f);
                    }
                }

                /// Force ReplicationPrinter
                if(BaseNodeResourceNodeComponent->GetResourceComponentType()==RCType_ReplicationPrinter)
                {
                    if(RemainingPower-10.0f>0.0f)
                    {
                        /// Set power if its enougth
                        Power->SetPower(10.0f);

                        output=Power->GetPower();

                        RemainingPower-=10.0f-output;

                    }
                    else
                    {
                        BaseNodeResourceNodeComponent->SetPower(0.0f);
                    }
                }
            }
        }

        UsedPower=TotalPower-RemainingPower;
    }

    /// air levels
    Oxygen=2048.0f;

    return;
}

/// void
float Starbase::GetOxygen(void)
{
    return Oxygen;
}
