//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/Urho3D.h>

#include <iostream>

#include "../../../Urho3D/Graphics/AnimationController.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/IO/MemoryBuffer.h"
#include "../../../Urho3D/Physics/PhysicsEvents.h"
#include "../../../Urho3D/Physics/PhysicsWorld.h"
#include "../../../Urho3D/Physics/RigidBody.h"
#include "../../../Urho3D/Scene/Scene.h"
#include "../../../Urho3D/Scene/SceneEvents.h"
#include "../../../Urho3D/Scene/LogicComponent.h"
#include "../../../Urho3D/Core/Timer.h"

#include "InteractObject.h"
#include "InteractEvent.h"



using namespace Urho3D;

using namespace std;

/// Base code
InteractObject::InteractObject(Context* context) :
    LogicComponent(context)
{

/// Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);

/// Debug
    cout << "Debug: Interact Object Component Constructor" << endl;

}

/// Registering a object
void InteractObject::RegisterObject(Context* context)
{
    context->RegisterFactory<InteractObject>("Existence");


    /// Debug
    cout << "Debug: Interact Object Register Factory " << endl;


    return;
}

/// Creation of a Interact object
void InteractObject::Start()
{

    return;
}

/// Fix update
void InteractObject::FixedUpdate(float timeStep)
{
    return;
}

