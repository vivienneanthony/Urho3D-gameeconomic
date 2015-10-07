#ifndef AIFUZZYSTATEMACHINESTATE_H
#define AIFUZZYSTATEMACHINESTATE_H

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

using namespace Urho3D;
using namespace std;

class AIFuzzyControl;

/// Machine State Class
class AIFuzzyStateMachineState: public LogicComponent
{
    OBJECT (AIFuzzyStateMachineState);

public:
    /// Constructor and Destructor
    AIFuzzyStateMachineState(Context * context);
    virtual ~AIFuzzyStateMachineState();

    /// Register
    static void RegisterObject(Context * context);

    /// Enter, Set,Update, Set
    virtual void Set(AIStatesTypes type, AIFuzzyControl* parent);
    virtual void FixedUpdate(StringHash eventType, VariantMap& eventData);
    virtual void Update(float dt);
    virtual void Enter(void);
    virtual void Exit(void);

    /// Initialize
    virtual void Initialize(void);

    /// Calculate Activation
    virtual float CalculateActivation(void);

    /// Check Lower Bound
    virtual unsigned int CheckLowerBound(float LBound);

    /// Check Upperbound
    virtual unsigned int CheckUpperBound(float UBound);

    /// Check Bounds
    virtual unsigned int CheckBound(float LB, float UB);

    /// Virtual
    virtual void HandleUpdate(StringHash eventType, VariantMap& eventData);;

protected:
    AIFuzzyControl * M_Parent;
    AIStatesTypes M_Type;
    float M_ActivationLevel;
private:
};

#endif // AIFUZZYSTATEMACHINESTATE_H
