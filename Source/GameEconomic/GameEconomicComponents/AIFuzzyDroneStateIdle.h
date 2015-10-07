#ifndef AIFUZZYDRONESTATEIDLE_H
#define AIFUZZYDRONESTATEIDLE_H

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

using namespace std;
using namespace Urho3D;

class AIFuzzyDroneStateIdle : public AIFuzzyStateMachineState
{
    OBJECT(AIFuzzyDroneStateIdle);

public:
    /// Add Idle Constructor
    AIFuzzyDroneStateIdle(Context * context);

    /// Second constructor
    AIFuzzyDroneStateIdle(Context * context, AIFuzzyControl* SetParent);

    virtual ~AIFuzzyDroneStateIdle();

    /// Fixed Update
    void FixedUpdate(StringHash eventType, VariantMap& eventData);

    void Set(AIStatesTypes SetState, AIFuzzyControl* SetParent);
    void Update(float dt);
    float CalculateActivation();
    void Exit();

protected:
    float changeFocusVector;
    float changeFocusVectorTime;

private:
};

#endif
