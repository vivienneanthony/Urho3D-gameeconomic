#ifndef TRADER_H
#define TRADER_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// define Player information
#include <iostream>

#include "Resource.h"

using namespace std;
using namespace Urho3D;


#include "Entity.h"
#include "CargoBay.h"
#include "Resource.h"

#define TRADER_ENTITY           0

using namespace std;

class Trader :  public Entity
{
    OBJECT (Trader);

public:
    Trader(Context * context);
    virtual ~Trader();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void Start(void);

    /// Other functions
    bool Hail(void);
    bool TradableGoodsSell(Vector<ResourceGame> &tradableResources); /// get tradable goods and send list of goods being sold
    bool TradableGoodsBuy(unsigned typeofgoods); /// list of goods that can be brougth from someone
    unsigned int MakeTradePlayerToTraderSell(Vector<ResourceGame> &Trading);
    unsigned int MakeTradePlayerToTraderBuy(Vector<ResourceGame> &buying,unsigned int TransactionFee);
    bool IsTrading();

protected:
private:
    unsigned int TraderType;
    unsigned long int Credits;
    String Name;
    CargoBay * MyTraderCargoBay;
    String UniqueID;
};

#endif // TRADER_H
