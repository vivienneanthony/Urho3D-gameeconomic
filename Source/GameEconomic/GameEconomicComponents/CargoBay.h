#ifndef CARGOBAY_H
#define CARGOBAY_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// c++ native includes headers
#include <iostream>
#include <ctime>

/// c++ game include headers
#include "Resource.h"

using namespace std;
using namespace Urho3D;


/// Cargo bay
class CargoBay : public LogicComponent
{
    OBJECT (CargoBay);

public:
    CargoBay(Context * context);
    virtual ~CargoBay();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void Start(void);

    /// accessible functions
    bool GetCatalog(Vector<ResourceGame> &InputResourceList);
    bool AddCargo(Vector<ResourceGame> &InputResourceList);
    bool RemoveCargo(Vector<ResourceGame> &InputResourceList);
    bool EjectCargo(void);

protected:

private:
    unsigned int GetCargoCapacity(void);

    /// CargoBayclassification
    int CargoBayClass;

    /// create a array for Resources
    Vector<ResourceGame> Inventory;
};

#endif // CargoBay_H
