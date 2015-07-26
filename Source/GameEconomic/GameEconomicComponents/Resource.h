#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

#include <string>

/// game resource
struct ResourceGame
{
    int ResourceType;
    int Quantity;
};

using namespace std;

static const string ResourceName[] = {"gold", "silver", "copper", "iron",
                                      "nickel", "cobalt", "silica","diamond","lead","uranium"
                                     };

static const unsigned int ResourceValue[]= {1000,303,10,39,1000,353,230,39,90,303};

#endif // RESOURCE_H_INCLUDED
