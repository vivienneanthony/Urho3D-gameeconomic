#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#define CLOSEDMARKET    0
#define OPENMARKET      1
#define BLACKMARKET     2

#include <iostream>
#include <vector>

#include "Resource.h"
#include "Entity.h"
#include "CargoBay.h"
#include "Trader.h"

using namespace std;
using namespace Urho3D;

/// couple of functions needed
/// getcurrent list of items

Trader::Trader(Context * context):
    Entity(context),
    MyTraderCargoBay(NULL),
    Credits(0),
    TraderType(0)
{


    /// initalizae
    MyTraderCargoBay = new CargoBay(context_);


    /// Create a new trader
    ThisTrader = new TraderInformation();
}

Trader::~Trader()
{
    //dtor
}

void Trader::Start(void)
{

    return;
}

/// Set Parameters for loading resource
void Trader::SetParameters(TraderInformation &DBTrader)
{

    /// copy values
    String Name = DBTrader.Name;
    UniqueID = DBTrader.UniqueID;
    Credits = DBTrader.Credits;
    TraderType = DBTrader.TraderType;

    /// copy values
    ThisTrader->Name = DBTrader.Name;
    ThisTrader->UniqueID = DBTrader.UniqueID;
    ThisTrader->Credits = DBTrader.Credits;
    ThisTrader->TraderType = DBTrader.TraderType;
    ThisTrader->AlienRace = DBTrader.AlienRace;
    ThisTrader->AlienAllianceAligned = DBTrader.AlienAllianceAligned;


    return;
}

bool Trader::Hail(void)
{
    /// displayname
    cout << "My name is Dorhlu. What do you want?" << endl;

    cout << "\r\nLaughing out loud. I'm your trusty Trader. You want to sell something to me?" << endl;

    return false;
}

/// Register Trader
void Trader::RegisterObject(Context* context)
{
    context->RegisterFactory<Trader>();

    return;
}


/// get Tradeablegoods
bool Trader::TradableGoodsSell(Vector<ResourceGame> &TradableResources)
{
    /// Scan through all items
    Vector<ResourceGame> TemporaryCargo;

    /// Get temporarycago
    bool Success = MyTraderCargoBay->GetCatalog(TemporaryCargo);

    if(Success)
    {
        /// loop through goods
        for(unsigned int i=0; i<TemporaryCargo.Size(); i++)
        {
            /// transfer tradable Resources to tradablegoods
            TradableResources.Push(TemporaryCargo[i]);
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool Trader::TradableGoodsBuy(unsigned int TypeOfGoods) /// list of goods that can be brougth from someone
{
    /// set type of good to 1 meaning basic Resources
    unsigned int Types=1;

    return Types;

}

unsigned int Trader::MakeTradePlayerToTraderSell(Vector<ResourceGame> &Trading)
{
    /// fist AddCargo
    bool Success = MyTraderCargoBay->AddCargo(Trading);

    /// Total needed Resource space
    unsigned int Sum=0;
    unsigned int TransactionCharge;

    /// If Success add Sum then figure out fee
    if(Success)
    {

        for(unsigned int i=0; i<Trading.Size(); i++)
        {
            Sum+=Trading.At(i).Quantity*ResourceValue[Trading.At(i).ResourceType];
        }

        /// take out transaction
        TransactionCharge=Sum*.05;

    }
    else
    {
        return 0;
    }

    /// Calculate returned Credit
    Credits+=TransactionCharge;

    return Sum-TransactionCharge;
}


unsigned int Trader::MakeTradePlayerToTraderBuy(Vector<ResourceGame> &Buying, unsigned TransactionFee)
{

    /// Total needed Resource space
    unsigned int Sum=0;
    unsigned int TransactionCharge;
    bool HaveQuantity=false;
    bool Found=false;

    /// Get Inventory
    Vector<ResourceGame> Inventory;

    MyTraderCargoBay->GetCatalog(Inventory);

    /// verify enougth Credits
    for(unsigned int i=0; i<Buying.Size(); i++)
    {
        Sum+=Buying.At(i).Quantity*ResourceValue[Buying.At(i).ResourceType];
    }

    /// Add 100 to TransactionFee
    TransactionCharge=Sum+100;

    if(TransactionFee<TransactionCharge)
    {
        cout << "\r\nYou dont have enougth money." << endl;
        return false;
    }

    /// now check if its in the Inventory
    for(unsigned int i=0; i<Buying.Size(); i++)
    {
        /// loop through each item
        for(unsigned int j=0; j<Inventory.Size(); j++)
        {
            if(Buying.At(i).ResourceType==Inventory.At(j).ResourceType)
            {
                if(Buying.At(i).Quantity<Inventory.At(j).Quantity)
                {
                    HaveQuantity++;
                }
                Found++;
                break;
            }
        }
    }

    ///If Found less then what's being brought'
    if(Found<Buying.Size()&&HaveQuantity<Buying.Size()&&Found!=HaveQuantity)
    {
        cout << "\r\nI don't have what you want" << endl;

        return false;
    }

    /// Everything is good
    bool Success = MyTraderCargoBay->RemoveCargo(Buying);

    if(Success)
    {
        Credits+=TransactionFee;
    }

    return true;
}

bool Trader::IsTrading(void)
{
    return true;
}
