#ifndef GameEconomicServerClientConsole_H
#define GameEconomicServerClientConsole_H


// Copyright (c) 2008-2015 the Urho3D project.
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

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/IOEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/IO/VectorBuffer.h>
#include <Urho3D/Core/Thread.h>
#include <kNet/MessageConnection.h>
#include <kNet/MessageConnection.h>

#include "../GameEconomicComponents/ServerConsoleInterface.h"


using namespace std;
using namespace Urho3D;


/// This first example, maintaining tradition, prints a "Hello World" message.
/// Furthermore it shows:
///     - Using the Sample / Application classes, which initialize the Urho3D engine and run the main loop
///     - Adding a Text element to the graphical user interface
///     - Subscribing to and handling of update events
class GameEconomicServerClientConsole : public LogicComponent, public Thread
{
    OBJECT(GameEconomicServerClientConsole);

public:
    /// Construct.
    GameEconomicServerClientConsole(Context* context);
    virtual ~GameEconomicServerClientConsole();

    /// Process the connection in the worker thread until closed.
    virtual void ThreadFunction();

    /// Register
    static void RegisterObject(Context * context);
    void Start(void);

    void ConfigureNetwork(networkconfiguration &tempnetwork);
protected:

private:
    void Connect(networkconfiguration &tempnetwork);;
    void SendMessage(String Message);

    bool serverconnection;
    void ShowPrompt(void);

    /// copy configuration
    networkconfiguration NetworkConfiguration;

};

#endif
