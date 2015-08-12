#ifndef PLAYER_H
#define PLAYER_H

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

using namespace std;
using namespace Urho3D;


#include "../Player.h"

/// Player class
class Player : public LogicComponent
{
public:
    /// public functions
    Player(Context *context);
    virtual ~Player();

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void Start(void);

    /// simple Adding and deleting Credits
    bool AddCredits(unsigned long int AddingCredits);
    bool SubtractCredits(unsigned long int SubtractingCredits);
protected:
private:
    unsigned long int Credits;
    String UniqueID;

    PlayerObject  * ThisPlayer;
};

#endif // PLAYER_H
