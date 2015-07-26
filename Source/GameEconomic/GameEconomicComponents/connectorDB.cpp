// mysql related headers
#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

#include <mysql_connection.h>
#include <mysql_driver.h>


// Include the Connector/C++ headers
#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"


/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>

#include "connectorDB.h"

using namespace std;
using namespace sql;
using namespace Urho3D;

// main code for connector
connectorDB::connectorDB(Context * context):
    LogicComponent(context)
{
    //ctor
    /// empty out everything
    MySqlConnection.hostname.clear();
    MySqlConnection.password.clear();
    MySqlConnection.username.clear();

    schema.clear();

    //cout << "\r\n(debug)connectorDB class constructer...";

    return;
}

connectorDB::~connectorDB()
{
    //dtor

    //debug output
    // << "\r\n(debug)connectorDB class deconstructer...";

    return;
}


/// Register Subsystem
void connectorDB::RegisterNewSubsystem(Context* context)
{
    context -> RegisterSubsystem(new connectorDB(context));

    return;
}

// actual code
/// function to set the configuration
int connectorDB::setHost(string dbhost, string dbuser, string dbpassword)
{
    /// set success
    bool success=false;

    /// set information
    MySqlConnection.hostname=dbhost;
    MySqlConnection.username=dbuser;
    MySqlConnection.password=dbpassword;

    return success;
}

/// Code to setup which databse
int connectorDB::setDB(string Schema)
{
    bool success=false;

    /// Set database name
    MySqlConnection.schema=Schema;
    schema=Schema;

    return success;
}

/// Code to connect to host
int connectorDB::connectHost()
{
    bool success=false;

    /// Attempt connection and connect to Host
    try
    {
        /// Connect to databse
        driver = get_driver_instance();
        con = driver->connect(MySqlConnection.hostname,MySqlConnection.username,MySqlConnection.password);

        con->setSchema(schema);

        /// Turn off the autocommit
        con -> setAutoCommit(0);

        success=true;

        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "     << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<     " )" << endl;

        success=false;
    }

    return success;
}

bool connectorDB::VerifyValidConfiguration(void)
{

    if(MySqlConnection.hostname.empty()
            ||MySqlConnection.username.empty()
            ||MySqlConnection.password.empty()
            ||MySqlConnection.schema.empty()
            ||schema.empty())
    {

        return false;
    }

    return true;
}

/// Code to setup excute a query
int connectorDB::executeQuery(string query)
{
    /// Set success flag
    bool success = false;

    /// Verify valid configuration
    if(!VerifyValidConfiguration())
    {
        return false;
    }

    /// Attempt and execute query
    try
    {
        /// Connect to database
        driver = get_driver_instance();
        con = driver->connect(MySqlConnection.hostname,MySqlConnection.username,MySqlConnection.password);

        /// Set schema
        con->setSchema(schema);

        /// Turn off the autocommit
        con -> setAutoCommit(0);

        // create a statement
        stmt = con->createStatement();

        // should execute query
        stmt->execute(query.c_str());

        /// Query executed
        success = true;

        delete stmt;
        delete con;

    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "     << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<     " )" << endl;

        /// Query failed
        success=false;
    }
    return success;
}


/// Code to setup excute a query
int connectorDB::checkTable(string checktable)
{
    /// Set success flag
    bool success = false;

    /// Verify valid configuration
    if(!VerifyValidConfiguration())
    {
        return false;
    }

    string preparedquery= string("SELECT 1 FROM ")+checktable;

    /// Attempt and execute query
    try
    {
        /// Connect to database
        driver = get_driver_instance();
        con = driver->connect(MySqlConnection.hostname,MySqlConnection.username,MySqlConnection.password);

        /// Set schema
        con->setSchema(schema);

        /// Turn off the autocommit
        con -> setAutoCommit(0);

        /// create a statement
        pstmt = con->prepareStatement(preparedquery);
        res = pstmt->executeQuery();

        if(res)
        {
            /// Query executed
            success = true;

        }
        else
        {
            success =false;
        }

        delete pstmt;
        delete res;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "     << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<     " )" << endl;

        /// Query failed
        success=false;

        return success;
    }
    return success;
}

/// Code to setup excute a query
int connectorDB::executePreparedStatement(string preparedstatement)
{
    /// Set success flag
    bool success = false;

    /// Verify valid configuration
    if(!VerifyValidConfiguration())
    {
        return false;
    }

    /// Attempt and execute query
    try
    {
        /// Connect to database
        driver = get_driver_instance();

        con = driver->connect(MySqlConnection.hostname,MySqlConnection.username,MySqlConnection.password);

        /// Set schema
        con->setSchema(schema);

        /// Turn off the autocommit
        con -> setAutoCommit(0);

        size_t pos = 0;

        /// create a statement
        pstmt = con->prepareStatement(preparedstatement);
        res = pstmt->executeQuery();


        /// delete statement and results
        delete pstmt;
        delete res;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "     << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<     " )" << endl;

        /// Query failed
        success=false;

    }
    return success;
}
