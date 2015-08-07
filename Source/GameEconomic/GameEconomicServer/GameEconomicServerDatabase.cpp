//
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

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Core/Thread.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>

#include <iostream>
#include <signal.h>
#include <sstream>
#include <fstream>
#include <vector>

#include "GameEconomicServer.h"

#include "../GameEconomicComponents/ServerConsoleInterface.h"
#include "../GameEconomicComponents/connectorDB.h"
#include "../GameEconomicComponents/Player.h"
#include "../Accounts.h"
#include "../Administrator.h"
#include "../GameEconomicServer/Networking.h"

#include "signalHandler.hpp"
#include <pthread.h>

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;

/// verify
bool GameEconomicServer::VerifyIdentityDB(DBTable mode, String Username, String Password)
{
    Vector<String> TableNames;
    Vector<String> TableFields;

    if(mode==DBAccount)
    {
        TableNames.Push("accountusername");

        TableFields.Push(Username);

        /// if a valid email was given
        if(EmailValidCheck(Username)==true)
        {
            TableNames.Push("accountemail");
        }
    }
    else if(mode==DBAdministrator)
    {
        TableNames.Push("administratorusername");

        TableFields.Push(Username);
    }
    else
    {
        return false;
    }

    Vector<String> Results;

    if(mode==DBAccount)
    {
        Results = selectDBAccount(TableNames,TableFields);
    }

    if(mode==DBAdministrator)
    {
        Results = selectDBAdministrator(TableNames,TableFields);
    }

    /// If no results return
    if(Results.At(1)==String("0"))
    {
        return false;
    }

    String ResultsPassword;

    if(mode==DBAccount)
    {
        ResultsPassword=Results.At(8);
    }

    if(mode==DBAdministrator)
    {
        ResultsPassword=Results.At(4);
    }

    if(ResultsPassword.ToLower()!=Password.ToLower())
    {
        return false;
    }

    return true;
}
