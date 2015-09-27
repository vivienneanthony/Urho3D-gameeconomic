#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Scene/Node.h"
#include "Resource.h"

using namespace Urho3D;
using namespace std;

#include <string>

/// game resource
struct ResourceGame
{
    int ResourceType;
    int Quantity;
};

/// Mod structure based on previous info
struct ResourceInformation
{
    String ResourceName;
    int Quantity;
};

using namespace std;

/*
static const string ResourceName[] = {"gold", "silver", "copper", "iron",
                                      "nickel", "cobalt", "silica","diamond","lead","uranium"
                                     };
*/
static const unsigned int ResourceValue[]= {1000,303,10,39,1000,353,230,39,90,303};

#endif // RESOURCE_H_INCLUDED
