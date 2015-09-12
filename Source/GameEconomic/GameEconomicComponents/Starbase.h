#ifndef STARBASECOMPONENT_H_INCLUDE
#define STARBASECOMPONENT_H_INCLUDE

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Graphics/Light.h"

#include "../Starbase.h"
#include "../ResourceComponent.h"

#include "Resource.h"

/// define Player information
#include <iostream>


using namespace Urho3D;
using namespace std;


/// node information
struct StarbaseNodeInformation
{
    Node * StarbaseNode;
    ResourceComponentType StarbaseNodeType;
};


class Starbase :  public LogicComponent
{
    OBJECT (Starbase);

public:
    Starbase(Context * context);
    virtual ~Starbase();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    void Start(void);
    void PushNode(Node * NewNode, ResourceComponentType NewNodeComponentType);

    /// Public Variables
    unsigned int GetBaseNodes(void);
    StarbaseNodeInformation GetBaseNode(unsigned int idx);

protected:
    Vector <StarbaseNodeInformation> StarbaseNodes;
private:


};

#endif // Starbase_H
