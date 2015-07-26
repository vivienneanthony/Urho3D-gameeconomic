#ifndef MARKET_H
#define MARKET_H

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

struct Trade
{
    ResourceGame TradeGood;
    unsigned int AskingPrice;
    unsigned long TimeLimit;
    unsigned int LastBid;
    string LastBidder;
    unsigned Bids;
    string Seller;
    string TransactionID;
    string MarketID; /// filled by application
};

struct TradeInternal
{
    ResourceGame TradeGood;
    unsigned int AskingPrice;
    unsigned long TimeLimit;
    bool Bidding;

};

struct MarketSystem
{
    string MarketName;
    unsigned int MarketSystem;
    unsigned int MarketFee;
    bool Bidding;
    string UniqueID;
};

using namespace std;
using namespace Urho3D;

class URHO3D_API Market: public LogicComponent
{
    OBJECT(Market);

public:
    Market(Context * context);
    virtual ~Market();

    /// Registration
    static void RegisterNewSubsystem(Context* context);
    void Start(void);

    /// functions need to change for muitple markets
    bool SetMarketType(unsigned int MarketType);
    bool SellToMarket(Vector<TradeInternal> &Selling, unsigned int TransactionFee, string BuyerUniqueID);

    bool GetMarket(void);

protected:
private:
    Vector<Trade> * MarketExchange;
    unsigned int MarketSystem;
    unsigned int MarketFee;
    bool Bidding;
};

#endif // MARKET_H
