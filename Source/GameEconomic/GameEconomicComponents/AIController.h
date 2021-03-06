#ifndef AICONTROLLER_H
#define AICONTROLLER_H

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


#include "Entity.h"


const int CTRL_UP = 1;
const int CTRL_DOWN = 2;
const int CTRL_LEFT = 4;
const int CTRL_RIGHT = 8;
const int CTRL_FIRE = 16;
const int CTRL_JUMP = 32;
const int CTRL_ELEVATE = 64;
const int CTRL_DELEVATE = 128;
const int CTRL_ALL = 255;

using namespace std;
using namespace Urho3D;

class Drone;

/// Main class
class AIController : public LogicComponent
{
    OBJECT(AIController);

public:

    /// Constructors & Destructors
    AIController(Context* context);
    virtual ~AIController();

    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Called when the component is added to a scene node. Other components may not yet exist.
    virtual void Start();
    virtual void ResetAI();

protected:

private:
    float colliding;

};

#endif
