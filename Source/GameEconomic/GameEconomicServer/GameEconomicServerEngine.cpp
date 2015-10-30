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

#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
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
#include <cstdlib>

#include "GameEconomicServer.h"

#include "../GameEconomicComponents/ServerConsoleInterface.h"
#include "../GameEconomicComponents/Trader.h"
#include "../GameEconomicComponents/connectorDB.h"
#include "../GameEconomicComponents/Market.h"


#include <Urho3D/DebugNew.h>

using namespace std;
using namespace Urho3D;



/// Load traders
void GameEconomicServer::SceneLoad(void)
{

    connectorDB * connectionDB = GetSubsystem<connectorDB>();
    Market * market = GetSubsystem<Market>();

    unsigned int NumCols;
    unsigned int NumRows;


    if(connectionDB->checkTable("Traders"))
    {
        /// Connect to database load traders
        Vector<String> ResultsTraders = ListAllDBTraders();

        /// Get worth and column
        NumCols = atoi(ResultsTraders.At(0).CString());
        NumRows = atoi(ResultsTraders.At(1).CString());

        for(unsigned int i=0; i<NumRows; i++)
        {
            /// get based index for each row
            int index=(i*NumCols)+2;

            /// Create a node for each
            Node * newTrader = scene_->CreateChild("Trader");

            /// Create trader component
            Trader * newTraderComponent = newTrader->CreateComponent<Trader>();

            /// create and copy
            TraderInformation TraderParameters;

            TraderParameters.Name=ResultsTraders.At(index+2);
            TraderParameters.UniqueID=ResultsTraders.At(index+5);
            TraderParameters.TraderType=atoi(ResultsTraders.At(index+1).CString());
            TraderParameters.Credits=atoi(ResultsTraders.At(index+4).CString());

            /// Output for Debug

            cout << "Debug: Creating trader node with parameters ("<< TraderParameters.Name.CString() << "," << TraderParameters.UniqueID.CString() << "," << TraderParameters.TraderType << "," << TraderParameters.Credits << ") "<< endl;

            newTraderComponent->SetParameters(TraderParameters);
        }
    }

    if(connectionDB->checkTable("Markets"))
    {
        /// Connect to database load traders
        Vector<String> ResultsMarkets = ListAllDBMarkets();

        /// Get worth and column
        NumCols = atoi(ResultsMarkets.At(0).CString());
        NumRows = atoi(ResultsMarkets.At(1).CString());

        for(unsigned int i=0; i<NumRows; i++)
        {
            /// get based index for each row
            int index=(i*NumCols)+2;


            /// create and copy
            MarketSystem MarketSystemParameters;

            MarketSystemParameters.MarketName=ResultsMarkets.At(index+1);
            MarketSystemParameters.MarketSystem=atoi(ResultsMarkets.At(index+2).CString());
            MarketSystemParameters.Bidding=atoi(ResultsMarkets.At(index+3).CString());
            MarketSystemParameters.MarketFee=atoi(ResultsMarkets.At(index+4).CString());
            MarketSystemParameters.UniqueID=ResultsMarkets.At(index+5);

            /// Output for Debug

            cout << "Debug: Pusing new trader to market subsystem with parameters ("<< MarketSystemParameters.MarketName.CString() << "," << MarketSystemParameters.MarketFee << "," << MarketSystemParameters.MarketSystem << "," << MarketSystemParameters.Bidding << "," << MarketSystemParameters.UniqueID.CString() << ") "<< endl;

            market->ConfigurePushNewMarket(MarketSystemParameters);
        }
    }



    return;
}
