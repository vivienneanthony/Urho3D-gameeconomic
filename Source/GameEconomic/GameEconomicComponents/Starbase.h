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

    /// Start node
    virtual void Start(void);

    /// Add additional node
    virtual void PushNode(Node * NewNode, ResourceComponentType NewNodeComponentType);

    /// Set Online status
    virtual void SetOnline(bool SetOnline);

    /// Get used power and total power
    virtual double GetUsedPower(void);
    virtual double GetTotalPower(void);

    /// Oxysgen
    virtual float GetOxygen(void);

    /// Public Variables - This never changes
    unsigned int GetBaseNodes(void);
    StarbaseNodeInformation GetBaseNode(unsigned int idx);

protected:

private:
    /// All nodes
    Vector <StarbaseNodeInformation> * StarbaseNodes;

    /// Power related functions
    double TotalPower;
    double UsedPower;

    /// is Online
    bool IsOnline;

    /// Not used right now - Not Used
    double Oxygen;

    /// Starbase name - Not used
    String * Name;
};

#endif // Starbase_H
