#ifndef Administrators_H
#define Administrators_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// define Administrators information

#include <iostream>

#include "../Administrators.h"

using namespace std;
using namespace Urho3D;


/// Administrators class
class Administrators : public LogicComponent
{
public:
    /// public functions
    Administrators(Context *context);
    virtual ~Administrators();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void Start(void);

protected:
private:
    unsigned long int Credits;
    String UniqueID;
};

#endif // Administrators_H
