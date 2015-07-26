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
#include <ctime>

#include "CargoBay.h"

using namespace std;
using namespace Urho3D;



CargoBay::CargoBay(Context * context)
    :LogicComponent(context),
    CargoBayClass(0)
{
    return; //ctor
}

CargoBay::~CargoBay()
{
    return;//dtor
}

/// Entity Register Object
void CargoBay::RegisterObject(Context* context)
{
    context->RegisterFactory<CargoBay>();

    return;
}

/// Start Function
void CargoBay::Start(void )
{
        //// Blank

        return;
}

/// Input Resource list
bool CargoBay::GetCatalog(Vector<ResourceGame> &InputResourceList)
{
    /// check if there is a Inventory
    if (Inventory.Size()==0)
    {
        return 0;
    }

    /// loop till end of Vector
    InputResourceList.Reserve(Inventory.Size());

    InputResourceList.Clear();

    /// copy
    for (unsigned int i=0; i<Inventory.Size(); i++)
    {
        InputResourceList.Push(Inventory.At(i));
    }

    return true;
}

/// add cargo
bool CargoBay::AddCargo(Vector<ResourceGame> &InputResourceList)
{
    /// set flag
    bool Found=false;
    unsigned int MatchingResourceTo=0;
    unsigned int MatchingResourceFrom=0;

    /// Total needed Resource space
    unsigned int Sum=0;

    for(unsigned int i=0; i<InputResourceList.Size(); i++)
    {
        Sum+=InputResourceList.At(i).Quantity;
    }

    /// getcapacity
    if(Sum>GetCargoCapacity())
    {
        cout << "\r\nCargo catalog is at full capacity. Sorry" << endl;

        return false;
    }

    /// loop through the list
    for(unsigned int i=0; i<InputResourceList.Size(); i++)
    {

        /// set flag
        Found=false;
        MatchingResourceTo=0;
        MatchingResourceFrom=0;

        /// loop through each item
        for(unsigned int j=0; j<Inventory.Size(); j++)
        {
            if(InputResourceList.At(i).ResourceType==Inventory.At(j).ResourceType)
            {
                MatchingResourceTo= j;
                MatchingResourceFrom= i;

                Found=true;
                break;
            }
        }

        /// If Resource is Found add Quantity or add
        if(Found)
        {
            Inventory.At(MatchingResourceTo).Quantity=Inventory.At(MatchingResourceTo).Quantity+InputResourceList.At(MatchingResourceFrom).Quantity;

        }
        else
        {
            Inventory.Push(InputResourceList.At(i));

        }
    }

    return true;
}

unsigned int CargoBay::GetCargoCapacity(void)
{
    return 1000000;
}

bool CargoBay::EjectCargo(void)
{
    /// wipe out Inventory
    Inventory.Clear();

    return true;
}


/// add cargo
bool CargoBay::RemoveCargo(Vector<ResourceGame> &InputResourceList)
{
    /// set flag
    bool Found=false;
    unsigned int MatchingResourceTo=0;
    unsigned int MatchingResourceFrom=0;


    /// loop through the list
    for(unsigned int i=0; i<InputResourceList.Size(); i++)
    {

        /// set flag
        Found=false;
        MatchingResourceTo=0;
        MatchingResourceFrom=0;

        /// loop through each item
        for(unsigned int j=0; j<Inventory.Size(); j++)
        {
            if(InputResourceList.At(i).ResourceType==Inventory.At(j).ResourceType)
            {
                MatchingResourceTo= j;
                MatchingResourceFrom= i;

                Found=true;
                break;
            }
        }

        /// If Resource is Found add Quantity or add
        if(Found)
        {
            Inventory.At(MatchingResourceTo).Quantity-=InputResourceList.At(MatchingResourceFrom).Quantity;

        }
    }

    return true;
}
