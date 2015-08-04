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


/// Insert into a Administrator
bool GameEconomicServer::insertDBAdministrator(AdministratorInformation &TempAdministrator)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    AdministratorInformation TemporaryDBAdministrator;

    /// Copy specific Administrators
    TemporaryDBAdministrator.Idx=0;
    TemporaryDBAdministrator.Username = TempAdministrator.Username.Trimmed();
    TemporaryDBAdministrator.Password= TempAdministrator.Password.Trimmed();
    TemporaryDBAdministrator.UniqueID = TempAdministrator.UniqueID.Trimmed();
    TemporaryDBAdministrator.AdminAccounts=TempAdministrator.AdminAccounts;

    if(TemporaryDBAdministrator.Username.Empty()
            ||TemporaryDBAdministrator.Password.Empty()
            ||TemporaryDBAdministrator.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBAdministrator(AdministratorInformation &TempAdministrator)";
        return false;
    }

    cout << "it got here" << endl;

    ///StateStart
    ///String PreparedStatement = String("INSERT INTO Administrators ( `AdministratorUsername`, `AdministratorPassword`, `AdministratorUniqueID`,`AdministratorAdminAccounts`) VALUES (");
    String PreparedStatement = String("INSERT INTO Administrators (AdministratorUsername, AdministratorPassword, AdministratorUniqueID,AdministratorAdminAccounts) VALUES (");

    /// Copy information to statement
    PreparedStatement.Append("'"+TemporaryDBAdministrator.Username+"'");
    PreparedStatement.Append(",'"+TemporaryDBAdministrator.Password+"'");
    PreparedStatement.Append(",'"+TemporaryDBAdministrator.UniqueID+"'");
    PreparedStatement.Append(","+String(TemporaryDBAdministrator.AdminAccounts)+"");

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

bool GameEconomicServer::deleteDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM Administrators WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="administratorusername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratoruniqueid")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
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

bool GameEconomicServer::editDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE Administrators SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i>0)
        {
            PreparedStatement.Append(",");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="administratorusername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratorpassword")
        {
            PreparedStatement.Append("AdministratorPassword = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratoruniqueid")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE AdministratorUniqueID='");

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

bool GameEconomicServer::verifyDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT AdministratorIdx FROM 'Administrators' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="administratorUsername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratorUniqueID")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
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


Vector<String> GameEconomicServer::selectDBAdministrator(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM Administrators");

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
        /// unique values
        if(TableName.At(i).ToLower()=="administratorusername")
        {
            PreparedStatement.Append("AdministratorUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="administratoruniqueid")
        {
            PreparedStatement.Append("AdministratorUniqueID = '"+TableNameParameter.At(i)+"'");
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

/// Insert into a account
bool GameEconomicServer::insertDBAccount(AccountInformation &TempAccount)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// time related variables
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    /// get current time
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    /// convert time
    strftime (buffer,40,"%Y%m%d %r",timeinfo);

    /// temporary user
    AccountInformation TemporaryDBAccount;

    /// Copy specific accounts
    TemporaryDBAccount.Idx=0;
    TemporaryDBAccount.Username = TempAccount.Username.Trimmed();
    TemporaryDBAccount.Firstname = TempAccount.Firstname.Trimmed();
    TemporaryDBAccount.Middlename = TempAccount.Middlename.Trimmed();
    TemporaryDBAccount.Lastname = TempAccount.Lastname.Trimmed();
    TemporaryDBAccount.Email = TempAccount.Email.Trimmed();
    TemporaryDBAccount.Password = TempAccount.Password.Trimmed();
    TemporaryDBAccount.UniqueID = TempAccount.Lastname.Trimmed()+TempAccount.Firstname.Trimmed();

    if(TemporaryDBAccount.Username.Empty()
            ||TemporaryDBAccount.Firstname.Empty()
            ||TemporaryDBAccount.Middlename.Empty()
            ||TemporaryDBAccount.Lastname.Empty()
            ||TemporaryDBAccount.Email.Empty()
            ||TemporaryDBAccount.Password.Empty()
            ||TemporaryDBAccount.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBAccount(AccountInformation &TempAccount)";
        return false;
    }

    /// Convert time to buffer
    String DateTime(buffer);

    /// Now create the SQL Statement

    ///StateStart
    String PreparedStatement = String("INSERT INTO `Accounts` (`AccountUsername`, `AccountFirstname`, `AccountMiddlename`, `AccountLastname`, `AccountEmail`, `AccountPassword`, `AccountLastLogin`, `AccountUniqueIdx`) VALUES(");

    /// Copy information to statement
    PreparedStatement.Append("'"+TemporaryDBAccount.Username+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Firstname+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Middlename+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Lastname+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Email+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.Password+"'");
    PreparedStatement.Append(",'"+DateTime+"'");
    PreparedStatement.Append(",'"+TemporaryDBAccount.UniqueID+"'");

    /// Create the rest of the statement
    PreparedStatement.Append(");");

    string mysqlpreparedstatement =string(PreparedStatement.CString());


    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::deleteDBAccount(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM Accounts WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(";");

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

bool GameEconomicServer::editDBAccount(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE Accounts SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(", ");
        }
        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE AccountUniqueID='");

    PreparedStatement.Append(UniqueID);

    PreparedStatement.Append("' LIMIT 1;");

    cout << PreparedStatement.CString() << endl;

    /// Change statement to string
    string mysqlpreparedstatement =string(PreparedStatement.CString());

    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::verifyDBAccount(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT AccountIdx FROM 'Accounts' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
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


Vector<String> GameEconomicServer::selectDBAccount(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM Accounts");

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
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="accountusername")
        {
            PreparedStatement.Append("AccountUsername = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountfirstname")
        {
            PreparedStatement.Append("AccountFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountmiddlename")
        {
            PreparedStatement.Append("AccountMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountlastname")
        {
            PreparedStatement.Append("AccountLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountuniqueid")
        {
            PreparedStatement.Append("AccountUniqueID = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountemail")
        {
            PreparedStatement.Append("AccountEmail = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="accountpassword")
        {
            PreparedStatement.Append("AccountPassword = '"+TableNameParameter.At(i)+"'");
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



/// Insert into a Player
bool GameEconomicServer::insertDBPlayer(PlayerObject &TempPlayer)
{
    /// get db
    connectorDB * connectionDB = GetSubsystem<connectorDB>();

    /// temporary user
    PlayerObject TemporaryDBPlayer;

    /// Copy specific Players
    TemporaryDBPlayer.Health=0;
    TemporaryDBPlayer.Firstname = TempPlayer.Firstname.Trimmed();
    TemporaryDBPlayer.Middlename = TempPlayer.Middlename.Trimmed();
    TemporaryDBPlayer.Lastname = TempPlayer.Lastname.Trimmed();
    TemporaryDBPlayer.Level = TempPlayer.Level;
    TemporaryDBPlayer.Experience = TempPlayer.Experience;
    TemporaryDBPlayer.Reputation = TempPlayer.Reputation;
    TemporaryDBPlayer.Reputation1 = TempPlayer.Reputation1;
    TemporaryDBPlayer.Reputation2 = TempPlayer.Reputation2;
    TemporaryDBPlayer.Reputation3 = TempPlayer.Reputation3;
    TemporaryDBPlayer.Reputation4 = TempPlayer.Reputation4;
    TemporaryDBPlayer.Reputation5 = TempPlayer.Reputation5;
    TemporaryDBPlayer.AlienRace = TempPlayer.AlienRace;
    TemporaryDBPlayer.AlienAllianceAligned = TempPlayer.AlienAllianceAligned;
    TemporaryDBPlayer.Gender = TempPlayer.Gender;
    TemporaryDBPlayer.PersonalityTrait = TempPlayer.PersonalityTrait;
    TemporaryDBPlayer.UniqueID = TempPlayer.UniqueID.Trimmed();


    if(TemporaryDBPlayer.Firstname.Empty()
            ||TemporaryDBPlayer.Middlename.Empty()
            ||TemporaryDBPlayer.Lastname.Empty()
            ||TemporaryDBPlayer.UniqueID.Empty())
    {
        cout << "Cannot create prepared statement for insertDBPlayer(PlayerObject &TempPlayer)";
        return false;
    }
    ///StateStart
    String PreparedStatement = String("INSERT INTO Players (`PlayerFirstname`, `PlayerMiddlename`, `PlayerLastname`, `PlayerLevel`, `PlayerExperience`, `PlayerReputation`,`PlayerReputation1`,`PlayerReputation2`,`PlayerReputation3`,`PlayerReputation4`,`PlayerReputation5`,`PlayerAlienRace`,`PlayerAlienAllianceAligned`,`PlayerGender`,`PlayerPersonalityTrait`, `PlayerUniqueID`) Values (");

    /// Copy information to statement
    PreparedStatement.Append("'"+TemporaryDBPlayer.Firstname+"'");
    PreparedStatement.Append(",'"+TemporaryDBPlayer.Middlename+"'");
    PreparedStatement.Append(",'"+TemporaryDBPlayer.Lastname+"'");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Level)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Experience)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation1)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation2)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation3)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation4)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Reputation5)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.AlienRace)+"");
    PreparedStatement.Append(","+String((unsigned int)TemporaryDBPlayer.AlienAllianceAligned)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.Gender)+"");
    PreparedStatement.Append(","+String(TemporaryDBPlayer.PersonalityTrait)+"");
    PreparedStatement.Append(",'"+TemporaryDBPlayer.UniqueID+"'");

    /// Create the rest of the statement
    PreparedStatement.Append(");");

    string mysqlpreparedstatement =string(PreparedStatement.CString());


    /// attempt to write
    if(!connectionDB -> executePreparedStatement(mysqlpreparedstatement))
    {
        return false;
    }

    return true;
}

bool GameEconomicServer::deleteDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("DELETE FROM 'Players' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
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

bool GameEconomicServer::editDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter, String UniqueID)
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
    String PreparedStatement("UPDATE 'Players' SET ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(", ");
        }
        /// unique values
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };

        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
        };
    }

    /// end statement
    PreparedStatement.Append(" WHERE PlayerUniqueID='");

    PreparedStatement.Append(UniqueID);

    PreparedStatement.Append("' LIMIT 1;");

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

bool GameEconomicServer::verifyDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT PlayerIdx FROM 'Players' WHERE ");

    /// loop through each one
    for(unsigned int i=0; i<TableName.Size(); i++)
    {
        if(multipletablenames&&i!=0)
        {
            PreparedStatement.Append(" AND ");
        }
        /// unique values
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };

        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
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


Vector<String> GameEconomicServer::selectDBPlayer(Vector<String> TableName,Vector<String> TableNameParameter)
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
    String PreparedStatement("SELECT * FROM Players");

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
        /// unique values
        /// unique values
        if(TableName.At(i).ToLower()=="playerfirstname")
        {
            PreparedStatement.Append("PlayerFirstname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playermiddlename")
        {
            PreparedStatement.Append("PlayerMiddlename = '"+TableNameParameter.At(i)+"'");
        };

        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        if(TableName.At(i).ToLower()=="playerlastname")
        {
            PreparedStatement.Append("PlayerLastname = '"+TableNameParameter.At(i)+"'");
        };
        /// unique values
        if(TableName.At(i).ToLower()=="playeruniqueid")
        {
            PreparedStatement.Append("PlayerUniqueID = '"+TableNameParameter.At(i)+"'");
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

String GameEconomicServer::ListAllDBAccounts(Urho3D::Connection * sender)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBAccount(TableNames,TableFields);

    TableNames.Clear();
    TableFields.Clear();

    String ResultsString;

    for(unsigned int i=0; i<Results.Size(); i++)
    {
        /// Append
        ResultsString.Append(" "+Results.At(i));

    }

    return ResultsString;
}

String GameEconomicServer::ListAllDBPlayers(Urho3D::Connection * sender)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBPlayer(TableNames,TableFields);

    TableNames.Clear();
    TableFields.Clear();

    String ResultsString;

    for(unsigned int i=0; i<Results.Size(); i++)
    {
        /// Append
        ResultsString.Append(" "+Results.At(i));

    }

    return ResultsString;
}

String GameEconomicServer::ListAllDBAdministrators(void)
{
    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;
    Vector<String> Results;

    Results = selectDBAdministrator(TableNames,TableFields);

    TableNames.Clear();
    TableFields.Clear();

    String ResultsString;

    for(unsigned int i=0; i<Results.Size(); i++)
    {
        /// Append
        ResultsString.Append(" "+Results.At(i));

    }

    return ResultsString;
}

bool GameEconomicServer::UpdateSingleDBAdministrator(AdministratorInformation &TempAdministrator)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("administratorusername");
    TableNames.Push("administratorpassword");

    /// copy parameters
    TableFields.Push(TempAdministrator.Username);
    TableFields.Push(TempAdministrator.Password);

    bool Results = editDBAdministrator(TableNames,TableFields, TempAdministrator.UniqueID);

    return Results;
}

bool GameEconomicServer::DeleteSingleDBAdministrator(AdministratorInformation &TempAdministrator)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("administratoruniqueid");

    /// copy parameters
    TableFields.Push(TempAdministrator.UniqueID);

    bool Results = deleteDBAdministrator(TableNames,TableFields);

    return Results;
}

bool GameEconomicServer::UpdateSingleDBAccount(AccountInformation &TempAccount)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("accountusername");
    TableNames.Push("accountfirstname");
    TableNames.Push("accountmiddlename");
    TableNames.Push("accountlastname");
    TableNames.Push("accountemail");
    TableNames.Push("accountpassword");

    /// copy parameters
    TableFields.Push(TempAccount.Username);
    TableFields.Push(TempAccount.Firstname);
    TableFields.Push(TempAccount.Middlename);
    TableFields.Push(TempAccount.Lastname);
    TableFields.Push(TempAccount.Email);
    TableFields.Push(TempAccount.Password);

    bool Results = editDBAccount(TableNames,TableFields, TempAccount.UniqueID);

    return Results;
}


bool GameEconomicServer::DeleteSingleDBAccount(AccountInformation &TempAccount)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("accountuniqueid");

    /// copy parameters
    TableFields.Push(TempAccount.UniqueID);

    bool Results = deleteDBAccount(TableNames,TableFields);

    return Results;
}



bool GameEconomicServer::UpdateSingleDBPlayer(PlayerObject &TempPlayer)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy field
    TableNames.Push("playerfirstname");
    TableNames.Push("playermiddlename");
    TableNames.Push("playerlastname");

    /// copy parameters
    TableFields.Push(TempPlayer.Firstname);
    TableFields.Push(TempPlayer.Middlename);
    TableFields.Push(TempPlayer.Lastname);

    bool Results = editDBPlayer(TableNames,TableFields, TempPlayer.UniqueID);

    return Results;
}


bool GameEconomicServer::DeleteSingleDBPlayer(PlayerObject &TempPlayer)
{

    /// Create empty tables and fields and get results
    Vector<String> TableNames;
    Vector<String> TableFields;

    /// copy fields
    TableNames.Push("playeruniqueid");

    /// copy parameters
    TableFields.Push(TempPlayer.UniqueID);

    bool Results = deleteDBPlayer(TableNames,TableFields);

    return Results;
}

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
