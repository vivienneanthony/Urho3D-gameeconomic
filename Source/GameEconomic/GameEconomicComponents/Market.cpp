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
#include "Trader.h"

using namespace std;
using namespace Urho3D;

Market::Market(Context *context):
    LogicComponent(context)
    //ctor
{
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
    /// Clear
    MarketsSystem.Clear();

    return;
}

void Market::ConfigurePushNewMarket(MarketSystem &DBMarket)
{
    /// Apppend to market system
    MarketsSystem.Push(DBMarket);

    return;

}

/** @brief setMarkettype
  *
  * @todo: document this function
  */
bool Market::SetMarketType(String UniqueID)
{

    return true;
}

/** @brief displayMarket
  *
  * @todo: document this function
  */
MarketSystem Market::GetMarket(String UniqueID)
{
    /// loop through all the markets
    MarketSystem ResultMarket;

    for(unsigned int i=0; i<MarketsSystem.Size(); i++)
    {
        if(MarketsSystem.At(i).UniqueID==UniqueID)
        {
            /// return market system
            return MarketsSystem.At(i);
        }

    }
    return ResultMarket;
}


/** @brief selltoMarket
  *
  * @todo: document this function
  */
bool Market::SellToMarket(Vector<TradeInternal> &Selling, unsigned int TransactionFee, String BuyerUniqueID)
{
    bool Bidding=false;

    /// Get amrket
    MarketSystem MarketSellTo = GetMarket(BuyerUniqueID);

    /// if it is a close Market then leave
    if(MarketSellTo.MarketSystem==CLOSEDMARKET)
    {
        return false;
    }

    /// If there is a Market fee
    if(TransactionFee<MarketSellTo.MarketFee)
    {
        return false;
    }

    /// Create trade
    TransactionInformation NewTrade;

    /// Loop through each trade
    for(unsigned int i=0; i<Selling.Size(); i++)
    {
        /// copy info
        NewTrade.TradeGood = Selling.At(i).TradeGood;
        //   NewTrade.Seller  = BuyerUniqueID;
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
        // MarketExchange->Push(NewTrade);
    }/// copy data

    return true;
}

