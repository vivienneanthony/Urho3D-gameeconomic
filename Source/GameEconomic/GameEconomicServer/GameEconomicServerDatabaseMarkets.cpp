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
#include "../GameEconomicComponents/Accounts.h"
#include "../Administrator.h"
#include "../Networking.h"

#include "../GameEconomicComponents/Trader.h"
#include "../GameEconomicComponents/Market.h"

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;



/// Insert into a Market
bool GameEconomicServer::insertDBMarket(MarketInformation &TempMarket)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    MarketInformation TemporaryDBMarket;

    /// Copy specific Markets
    TemporaryDBMarket.MarketName = TempMarket.MarketName.Trimmed();
    TemporaryDBMarket.MarketFee=TempMarket.MarketFee;
    TemporaryDBMarket.MarketSystem=TempMarket.MarketSystem;
    TemporaryDBMarket.Bidding=TempMarket.Bidding;
    TemporaryDBMarket.UniqueID=TempMarket.UniqueID;
    TemporaryDBMarket.AlienRace = TempMarket.AlienRace;
    TemporaryDBMarket.AlienAllianceAligned = TempMarket.AlienAllianceAligned;


    if(TemporaryDBMarket.MarketName.Empty()
            ||TemporaryDBMarket.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBMarket(MarketInformation &TempMarket)";
        return false;
    }


    ///StateStart
    String PreparedStatement = String("INSERT INTO Markets (`MarketName`,`MarketSystem`, `MarketFee`,`MarketAlienRace`,`MarketAlienAllianceAligned`, `MarketBidding`, `MarketUniqueID`) VALUES (");


    /// Copy information to statement
    PreparedStatement.Append("'"+TemporaryDBMarket.MarketName+"'");
    PreparedStatement.Append(","+String(TemporaryDBMarket.MarketSystem)+"");
    PreparedStatement.Append(","+String(TemporaryDBMarket.MarketFee)+"");
    PreparedStatement.Append(","+String(TemporaryDBMarket.AlienRace)+"");
    PreparedStatement.Append(","+String((unsigned int)TemporaryDBMarket.AlienAllianceAligned)+"");
    PreparedStatement.Append(","+String(TemporaryDBMarket.Bidding)+"");;
    PreparedStatement.Append(",'"+TemporaryDBMarket.UniqueID+"");

    /// Create the rest of the statement
    PreparedStatement.Append(");");

    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB ->executeQuery(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::deleteDBMarket(Vector<String> TableName,Vector<String> TableNameParameter)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();


    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("DELETE FROM Markets WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="marketname")
        {
            PreparedStatement.Append("MarketName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="marketuniqueid")
        {
            PreparedStatement.Append("MarketUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" LIMIT 1;");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    cout << PreparedStatement.CString() << endl;

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::editDBMarket(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }



    /// Prepare statement
    String PreparedStatement("UPDATE Markets SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i>0)
        {
            PreparedStatement.Append(",");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="marketname")
        {
            PreparedStatement.Append("MarketName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="marketuniqueid")
        {
            PreparedStatement.Append("MarketUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE MarketUniqueID='");

    PreparedStatement.Append(UniqueID);

    PreparedStatement.Append("' LIMIT 1;");

    /// Change statement to string
    string mysqlpreparedstatement = string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::verifyDBMarket(Vector<String> TableName,Vector<String> TableNameParameter)
{


    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();


    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("SELECT MarketIdx FROM Markets WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="marketname")
        {
            PreparedStatement.Append("MarketName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="marketuniqueid")
        {
            PreparedStatement.Append("MarketUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }


    return true;
}





/// select market
Vector<String> GameEconomicServer::selectDBMarket(Vector<String> TableName,Vector<String> TableNameParameter)
{

    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// Set up a single statement
    bool multipletablenames=false;

    Vector<String> Results;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return Results;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("SELECT * FROM Markets");

    if(TableName.Size()!=0)
    {
        PreparedStatement.Append(" WHERE ");
    }

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }

        /// unique values
        if(TableName.At(i).ToLower()=="marketname")
        {
            PreparedStatement.Append("MarketName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="marketuniqueid")
        {
            PreparedStatement.Append("MarketUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    Results = connectionDB -> executePreparedStatementResult(mysqlpreparedstatement);

    return Results;
}

/// Insert into a Transaction
bool GameEconomicServer::insertDBMarketTransaction(TransactionInformation &TempTransaction)
{
    /// get db
    connectorDB * connectionDBMarketTransaction = GetSubsystem<connectorDB>();

    /// temporary user
    TransactionInformation TemporaryDBMarketTransaction;

    /// Copy specific Transactions
    TemporaryDBMarketTransaction.TradeGood = TempTransaction.TradeGood;
    TemporaryDBMarketTransaction.AskingPrice=TempTransaction.AskingPrice;
    TemporaryDBMarketTransaction.TimeLimit=TempTransaction.TimeLimit;
    TemporaryDBMarketTransaction.LastBid=TempTransaction.LastBid;
    TemporaryDBMarketTransaction.LastBidder=TempTransaction.LastBidder;
    TemporaryDBMarketTransaction.Bids=TempTransaction.Bids;
    TemporaryDBMarketTransaction.Seller=TempTransaction.Seller.Trimmed();
    TemporaryDBMarketTransaction.UniqueID=TempTransaction.UniqueID.Trimmed();
    TemporaryDBMarketTransaction.Market=TempTransaction.Market.Trimmed();


    if(TemporaryDBMarketTransaction.Seller.Empty()
            ||TemporaryDBMarketTransaction.UniqueID.Empty()
            ||TemporaryDBMarketTransaction.Market.Empty())
    {
        cout << "Cannot create prepared statement for insertDBMarketTransaction(TransactionInformation &TempTransaction)";
        return false;
    }


    ///State Start
    String PreparedStatement = String("INSERT INTO MarketTransactions (`TransactionResourceType`, `TransactionResourceQuanity`, `TransactionAskingPrice`, `TransactionEndTime`, `TransactionLastBid`, `TransactionLastBidder`, `TransactionBids`, `TransactionSeller`, `TransactionUniqueID`, `TransactionMarket`) VALUES ( ");


    /// Copy information to statement
    PreparedStatement.Append(""+String(TemporaryDBMarketTransaction.TradeGood.ResourceType)+"");
    PreparedStatement.Append(","+String(TemporaryDBMarketTransaction.TradeGood.Quantity)+"");
    PreparedStatement.Append(","+String(TemporaryDBMarketTransaction.AskingPrice)+"");
    PreparedStatement.Append(","+String(TemporaryDBMarketTransaction.TimeLimit)+"");
    PreparedStatement.Append(","+String(TemporaryDBMarketTransaction.LastBid)+"");;
    PreparedStatement.Append(",'"+TemporaryDBMarketTransaction.LastBidder+"");
    PreparedStatement.Append(","+String(TemporaryDBMarketTransaction.Bids)+"");;
    PreparedStatement.Append(",'"+TemporaryDBMarketTransaction.Seller+"");
    PreparedStatement.Append(",'"+TemporaryDBMarketTransaction.UniqueID+"");
    PreparedStatement.Append(",'"+TemporaryDBMarketTransaction.Market+"");

    /// Create the rest of the statement
    PreparedStatement.Append(");");

    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDBMarketTransaction ->executeQuery(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::deleteDBMarketTransaction(Vector<String> TableName,Vector<String> TableNameParameter)
{
    /// get db
    connectorDB * connectionDBMarketTransaction = GetSubsystem<connectorDB>();


    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("DELETE FROM MarketTransactions WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="transactionname")
        {
            PreparedStatement.Append("TransactionName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionuniqueid")
        {
            PreparedStatement.Append("TransactionUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionseller")
        {
            PreparedStatement.Append("TransactionSeller = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" LIMIT 1;");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    cout << PreparedStatement.CString() << endl;

    /// attempt to write
    if(!connectionDBMarketTransaction -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::editDBMarketTransaction(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
{
    /// get db
    connectorDB * connectionDBMarketTransaction = GetSubsystem<connectorDB>();

    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }



    /// Prepare statement
    String PreparedStatement("UPDATE Transactions SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i>0)
        {
            PreparedStatement.Append(",");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="transactionname")
        {
            PreparedStatement.Append("TransactionName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionuniqueid")
        {
            PreparedStatement.Append("TransactionUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionseller")
        {
            PreparedStatement.Append("TransactionSeller = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE TransactionUniqueID='");

    PreparedStatement.Append(UniqueID);

    PreparedStatement.Append("' LIMIT 1;");

    /// Change statement to string
    string mysqlpreparedstatement = string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDBMarketTransaction -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::verifyDBMarketTransaction(Vector<String> TableName,Vector<String> TableNameParameter)
{

    /// get db
    connectorDB * connectionDBMarketTransaction = GetSubsystem<connectorDB>();


    /// Set up a single statement
    bool multipletablenames=false;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return false;
    }

    /// Table name does not match
    if(TableName.Empty()||TableNameParameter.Empty())
    {
        return false;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("SELECT TransactionIdx FROM MarketTransactions WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="transactionname")
        {
            PreparedStatement.Append("TransactionName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionuniqueid")
        {
            PreparedStatement.Append("TransactionUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionseller")
        {
            PreparedStatement.Append("TransactionSeller = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDBMarketTransaction -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }


    return true;
}


/// select Transaction
Vector<String> GameEconomicServer::selectDBMarketTransaction(Vector<String> TableName,Vector<String> TableNameParameter)
{

    /// get db
    connectorDB * connectionDBMarketTransaction = GetSubsystem<connectorDB>();

    /// Set up a single statement
    bool multipletablenames=false;

    Vector<String> Results;

    /// Table name does not match
    if(TableName.Size()!=TableNameParameter.Size())
    {
        return Results;
    }

    /// Muiltiple parameters
    if(TableName.Size()>1)
    {
        multipletablenames=true;
    }

    /// Prepare statement
    String PreparedStatement("SELECT * FROM MarketTransactions");

    if(TableName.Size()!=0)
    {
        PreparedStatement.Append(" WHERE ");
    }

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }

        /// unique values
        if(TableName.At(i).ToLower()=="transactionname")
        {
            PreparedStatement.Append("TransactionName = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionuniqueid")
        {
            PreparedStatement.Append("TransactionUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="transactionseller")
        {
            PreparedStatement.Append("TransactionSeller = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    Results = connectionDBMarketTransaction -> executePreparedStatementResult(mysqlpreparedstatement);

    return Results;
}

Vector<String> GameEconomicServer::ListAllDBMarkets(void)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBMarket(TableNames,TableFields);

    return Results;
}
