#ifndef Accounts_H
#define Accounts_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// define Accounts information

#include <iostream>

#include "../Accounts.h"

using namespace std;
using namespace Urho3D;


/// Accounts class
class Accounts : public LogicComponent
{
public:
    /// public functions
    Accounts(Context *context);
    virtual ~Accounts();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void Start(void);

protected:
private:
    unsigned long int Credits;
    String UniqueID;
};

#endif // Accounts_H
