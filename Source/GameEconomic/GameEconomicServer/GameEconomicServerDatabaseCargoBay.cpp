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
#include <Urho3D/Input/Input.h>Ɔ
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

#include "../GameEconomicComponents/Resource.h"
#include "../GameEconomicComponents/CargoBay.h"

#include <Urho3D/DebugNew.h>


using namespace std;
using namespace Urho3D;


/// Insert into a CargoBays
bool GameEconomicServer::insertDBCargoBays(CargoBaysInformation &TempCargoBays)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    CargoBaysInformation TemporaryDBCargoBays;

    /// Copy specific CargoBays
    TemporaryDBCargoBays.Class = TempCargoBays.Class;
    TemporaryDBCargoBays.Owner= TempCargoBays.Owner.Trimmed();
    TemporaryDBCargoBays.UniqueID = TempCargoBays.UniqueID.Trimmed();

    if(TemporaryDBCargoBays.Owner.Empty()||TemporaryDBCargoBays.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBCargoBays(CargoBaysInformation &TempCargoBays)";
        return false;
    }

    cout << "it got here" << endl;

    ///StateStart
    String PreparedStatement = String("INSERT INTO CargoBays (`CargoBayIdx`, `CargoBayClass`, `CargoBayOwner`, `CargoBayUniqueID`) VALUES ( ");

    /// Copy information to statement
    PreparedStatement.Append(""+String(TemporaryDBCargoBays.Class)+"");
    PreparedStatement.Append(",'"+TemporaryDBCargoBays.Owner+"'");
    PreparedStatement.Append(",'"+TemporaryDBCargoBays.UniqueID+"'");


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

bool GameEconomicServer::deleteDBCargoBays(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM CargoBays WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="cargobayowner")
        {
            PreparedStatement.Append("CargoBayOwner = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobayuniqueid")
        {
            PreparedStatement.Append("CargoBayUniqueID = '"+TableNameParameter.At(i)+"'");
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

bool GameEconomicServer::editDBCargoBays(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE CargoBays SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i>0)
        {
            PreparedStatement.Append(",");
        }
        /// unique values
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="cargobayowner")
        {
            PreparedStatement.Append("CargoBayOwner = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobayuniqueid")
        {
            PreparedStatement.Append("CargoBayUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE CargoBaysUniqueID='");

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

bool GameEconomicServer::verifyDBCargoBays(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT CargoBaysIdx FROM 'CargoBays' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }

        /// unique values
        if(TableName.At(i).ToLower()=="cargobayowner")
        {
            PreparedStatement.Append("CargoBayOwner = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobayuniqueid")
        {
            PreparedStatement.Append("CargoBayUniqueID = '"+TableNameParameter.At(i)+"'");
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


Vector<String> GameEconomicServer::selectDBCargoBays(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM CargoBays");

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
        if(TableName.At(i).ToLower()=="cargobayowner")
        {
            PreparedStatement.Append("CargoBayOwner = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobayuniqueid")
        {
            PreparedStatement.Append("CargoBayUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    cout << PreparedStatement.CString() << endl;

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    Results = connectionDB -> executePreparedStatementResult(mysqlpreparedstatement);


    return Results;
}




/// Insert into a CargoBayCatalog
bool GameEconomicServer::insertDBCargoBayCatalog(CargoBayCatalogInformation &TempCargoBayCatalog)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    CargoBayCatalogInformation TemporaryDBCargoBayCatalog;

    /// Copy specific CargoBayCatalog
    TemporaryDBCargoBayCatalog.TradeGood = TempCargoBayCatalog.TradeGood;
    TemporaryDBCargoBayCatalog.Bay= TempCargoBayCatalog.Bay.Trimmed();
    TemporaryDBCargoBayCatalog.UniqueID = TempCargoBayCatalog.UniqueID.Trimmed();

    if(TemporaryDBCargoBayCatalog.Bay.Empty()
            ||TemporaryDBCargoBayCatalog.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBCargoBayCatalog(CargoBayCatalogInformation &TempCargoBayCatalog)";
        return false;
    }

    cout << "it got here" << endl;

    ///StateStart
    String PreparedStatement = String("INSERT INTO CargoBayCatalog (`INSERT INTO CargoBayCatalog (`CargoBayCatalogResourceType`, `CargoBayCatalogResourceQuantity`, `CargoBayCatalogBay`, `CargoBayCatalogUniqueID`) VALUES (");

    /// Copy information to statement
    PreparedStatement.Append(""+String(TemporaryDBCargoBayCatalog.TradeGood.ResourceType)+"");
    PreparedStatement.Append(""+String(TemporaryDBCargoBayCatalog.TradeGood.Quantity)+"");
    PreparedStatement.Append(",'"+TemporaryDBCargoBayCatalog.Bay+"'");
    PreparedStatement.Append(",'"+TemporaryDBCargoBayCatalog.UniqueID+"'");


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

bool GameEconomicServer::deleteDBCargoBayCatalog(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM CargoBayCatalog WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="cargobaycatalogowner")
        {
            PreparedStatement.Append("CargoBayCatalogBay  = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobaycataloguniqueid")
        {
            PreparedStatement.Append("CargoBayCatalogUniqueID = '"+TableNameParameter.At(i)+"'");
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

bool GameEconomicServer::editDBCargoBayCatalog(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE CargoBayCatalog SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i>0)
        {
            PreparedStatement.Append(",");
        }
        /// unique values
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="cargobaycatalogowner")
        {
            PreparedStatement.Append("CargoBayCatalogBay  = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobaycataloguniqueid")
        {
            PreparedStatement.Append("CargoBayCatalogUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE CargoBayCatalogUniqueID='");

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

bool GameEconomicServer::verifyDBCargoBayCatalog(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT CargoBayCatalogIdx FROM 'CargoBayCatalog' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }

        /// unique values
        if(TableName.At(i).ToLower()=="cargobaycatalogowner")
        {
            PreparedStatement.Append("CargoBayCatalogBay  = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobaycataloguniqueid")
        {
            PreparedStatement.Append("CargoBayCatalogUniqueID = '"+TableNameParameter.At(i)+"'");
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


Vector<String> GameEconomicServer::selectDBCargoBayCatalog(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM CargoBayCatalog");

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
        if(TableName.At(i).ToLower()=="cargobaycatalogowner")
        {
            PreparedStatement.Append("CargoBayCatalogBay  = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="cargobaycataloguniqueid")
        {
            PreparedStatement.Append("CargoBayCatalogUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

    cout << PreparedStatement.CString() << endl;

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    Results = connectionDB -> executePreparedStatementResult(mysqlpreparedstatement);


    return Results;
}
