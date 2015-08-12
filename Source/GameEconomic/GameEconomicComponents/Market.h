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



using namespace std;
using namespace Urho3D;

struct MarketInformation
{
    String MarketName;
    unsigned int MarketSystem;
    unsigned int MarketFee;
    bool Bidding;
    String UniqueID;
    unsigned int AlienRace;
    bool AlienAllianceAligned;
};


struct TransactionInformation
{
    ResourceGame TradeGood;
    unsigned int AskingPrice;
    unsigned long TimeLimit;
    unsigned int LastBid;
    String LastBidder;
    unsigned Bids;
    String Seller;
    String UniqueID;
    String Market; /// filled by application
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
    String MarketName;
    unsigned int MarketSystem;
    unsigned int MarketFee;
    bool Bidding;
    String UniqueID;
    unsigned int AlienRace;
    bool AlienAllianceAligned;
};


enum MarketTypes
{
    MarketOpen,
    MarketClosed,
    MarketBlack
};

class URHO3D_API Market: public LogicComponent
{
    OBJECT(Market);

public:
    Market(Context * context);
    virtual ~Market();

    /// Registration
    static void RegisterNewSubsystem(Context* context);
    void ConfigurePushNewMarket(MarketSystem &DBMarket);
    void Start(void);

    /// functions need to change for muitple markets
    bool SetMarketType(String UniqueID);
    bool SellToMarket(Vector<TradeInternal> &Selling, unsigned int TransactionFee, String BuyerUniqueID);

    MarketSystem GetMarket(String UniqueID);

protected:
private:
    Vector<MarketSystem> MarketsSystem;

};

#endif // MARKET_H
