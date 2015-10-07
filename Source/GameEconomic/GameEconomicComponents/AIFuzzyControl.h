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

//#include "Drone.h"

using namespace std;
using namespace Urho3D;

/// Set Control Type
enum ControlType
{
    CONTROLTYPE_NONE,
    CONTROLTYPE_HUMAN,
    CONTROLTYPE_AI
};

enum AIFuzzyLogic
{
    FuzzyLogic_Idle
};

class Drone;

/// Fuzzy Control
class AIFuzzyControl: public LogicComponent
{
    OBJECT(AIFuzzyControl);
public:
    /// Constructor and Destructor
    AIFuzzyControl(Context * context);
    virtual ~AIFuzzyControl();

    /// Register
    static void RegisterObject(Context * context);

    /// Update and Initialize
    virtual void FixedUpdate(float dt);
    virtual void Initialize(void);

    /// Set Ship
    void SetAIDrone(Drone* ThisEntity);

    ///Data
    SharedPtr<Drone> M_Entity;

    /// Control Type
    ControlType M_Type;

    /// Target
    AIFuzzyLogic M_Logic;

protected:
private:
};

#endif


