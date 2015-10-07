#ifndef AIFUZZYSTATEMACHINE_H
#define AIFUZZYSTATEMACHINE_H

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

#include "AIFuzzyStateMachineStates.h"
#include "AIFuzzyStateMachineState.h"

#include "Entity.h"
#include "Drone.h"

using namespace std;
using namespace Urho3D;

class AIFuzzyControl;

/// Class State Class
class AIFuzzyStateMachine : public AIFuzzyStateMachineState
{
    OBJECT(AIFuzzyStateMachine);

public:
    /// Constructor / Destructor
    AIFuzzyStateMachine(Context * context);

    virtual ~AIFuzzyStateMachine() {};

    /// Register
    static void RegisterObject(Context * context);

    /// Other Functions
    virtual void Set(AIMachineTypes type, AIFuzzyControl * control);
    virtual void Update(float dt);
    virtual void UpdateMachine(float dt);
    virtual void FixedUpdate(StringHash eventType, VariantMap& eventData);
    virtual void AddState(AIFuzzyStateMachineState* state);
    virtual bool IsActive(AIFuzzyStateMachineState* state);
    virtual void HandleUpdate(StringHash eventType, VariantMap& eventData);
    virtual void Reset();

    int M_Type;
protected:
    vector <AIFuzzyStateMachineState*> M_States;
    vector <AIFuzzyStateMachineState*> M_ActivatedStates;

private:
};

#endif // AIFUZZYSTATEMACHINE_H
