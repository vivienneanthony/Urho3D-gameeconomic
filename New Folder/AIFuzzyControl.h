#ifndef AIFUZZYCONTROL_H
#define AIFUZZYCONTROL_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Input/Controls.h"

#include <iostream>
#include <vector>

#include "Drone.h"
///#include "Entity.h"

#include "AIFuzzyControl.h"

using namespace std;
using namespace Urho3D;

/// Set Control Type
enum ControlType
{
    CONTROLTYPE_NONE,
    CONTROLTYPE_HUMAN,
    CONTROLTYPE_AI
};

class Drone;

/// Fuzzy Control
class AIFuzzyControl: public LogicComponent
{
    OBJECT(AIFuzzyControl);
public:
    /// Constructor and Destructor
    AIFuzzyControl(Context * context):LogicComponent(context)
    {
        M_Entity = NULL;
        M_Type = CONTROLTYPE_NONE;
    };

    virtual ~AIFuzzyControl() {};

    /// Register
    static void RegisterObject(Context * context)
    {
        context->RegisterFactory<AIFuzzyControl>();
    };

    /// Update and Initialize
    virtual void Update(float dt) {}
    virtual void Initialize() {}

    /// Set Ship
    void SetDrone(Drone* ThisEntity)
    {
        M_Entity = ThisEntity;
    };

    ///Data
    Drone *M_Entity;

    ControlType M_Type;
};

#endif


