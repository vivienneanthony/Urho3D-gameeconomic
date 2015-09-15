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
#include "../ResourceComponent.h"

using namespace std;
using namespace Urho3D;

/// couple of functions needed
/// getcurrent list of items
Starbase::Starbase(Context * context):
    LogicComponent(context)
{

    /// Create a new vector
    StarbaseNodes = new Vector<StarbaseNodeInformation>();
 ///   StarbaseNodes.Clear();

    return;
}

/// Starbase destructor
Starbase::~Starbase()
{
    //dtor

    return;
}


/// Register Starbase
void Starbase::RegisterObject(Context* context)
{
    context->RegisterFactory<Starbase>();

    return;
}

/// Starbase initialization
void Starbase::Start(void)
{
    return;
}



/// Public Variables
unsigned int Starbase::GetBaseNodes(void)
{
    return StarbaseNodes->Size();


}

StarbaseNodeInformation Starbase::GetBaseNode(unsigned int idx)
{
    StarbaseNodeInformation ReturnNode = StarbaseNodes->At(idx);

    return ReturnNode;
}

void Starbase::PushNode(Node * NewNode, ResourceComponentType NewNodeComponentType)
{
    /// Create a new node
    StarbaseNodeInformation * SaveNode = new StarbaseNodeInformation();

    SaveNode->StarbaseNode=NewNode;
    SaveNode->StarbaseNodeType=NewNodeComponentType;

///    StarbaseNodes->Push(SaveNode);

    return;
}
