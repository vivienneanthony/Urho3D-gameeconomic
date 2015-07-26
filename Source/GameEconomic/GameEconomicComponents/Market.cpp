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
#include <ctime>

#include "Resource.h"
#include "Market.h"

using namespace std;
using namespace Urho3D;

Market::Market(Context *context):
    LogicComponent(context),
    MarketExchange(NULL),
    MarketSystem(0),
    MarketFee(0),
    Bidding(0)
{
    //ctor
}

Market::~Market()
{
    //dtor
}


/// Register Subsystem
void Market::RegisterNewSubsystem(Context* context)
{
    context -> RegisterSubsystem(new Market(context));
    return;
}

/** @brief start
  *
  * @todo: start
  */
void Market::Start(void)
{
    /// initializeMarket
    MarketExchange = new Vector<Trade>;

    /// clear Market on initiate
    MarketExchange -> Clear();

    MarketFee=0;
    Bidding=false;

    return;
}

/** @brief setMarkettype
  *
  * @todo: document this function
  */
bool Market::SetMarketType(unsigned int MarketType)
{
    /// set Market type
    MarketSystem = MarketType;

    return true;
}

/** @brief displayMarket
  *
  * @todo: document this function
  */
bool Market::GetMarket(void)
{
    return true;
}

/** @brief selltoMarket
  *
  * @todo: document this function
  */
bool Market::SellToMarket(Vector<TradeInternal> &Selling, unsigned int TransactionFee, string BuyerUniqueID)
{
    bool Bidding=false;

    /// if it is a close Market then leave
    if(MarketSystem==CLOSEDMARKET)
    {
        return false;
    }

    /// If there is a Market fee
    if(TransactionFee<MarketFee)
    {
        return false;
    }

    /// Create trade
    Trade NewTrade;

    /// Loop through each trade
    for(unsigned int i=0; i<Selling.Size(); i++)
    {
        /// copy info
        NewTrade.TradeGood = Selling.At(i).TradeGood;
        NewTrade.Seller  = BuyerUniqueID;
        NewTrade.AskingPrice = Selling.At(i).AskingPrice;
        NewTrade.TimeLimit=Selling.At(i).TimeLimit;

        /// If Bidding is null
        if(Bidding==false)
        {
            NewTrade.Bids=0;
            NewTrade.LastBid=0;
            NewTrade.LastBidder="";
        }

        /// add to Market
        MarketExchange->Push(NewTrade);
    }/// copy data

    return true;
}

