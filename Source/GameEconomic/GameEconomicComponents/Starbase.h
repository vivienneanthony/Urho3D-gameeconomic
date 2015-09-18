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

#include "Resource.h"
#include "../ResourceComponent.h"
#include "../Starbase.h"



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

 /// Handle update. Called by LogicComponent base class.
    virtual void FixedUpdate(float timeStep);

    void Start(void);
    void PushNode(Node * NewNode, ResourceComponentType NewNodeComponentType);

    void SetOnline(bool SetOnline);

    double GetUsedPower(void);
    double GetTotalPower(void);

    /// Public Variables
    unsigned int GetBaseNodes(void);
    StarbaseNodeInformation GetBaseNode(unsigned int idx);
protected:

private:
    Vector <StarbaseNodeInformation> * StarbaseNodes;

    double TotalPower;
    double UsedPower;

    bool IsOnline;
};

#endif // Starbase_H
