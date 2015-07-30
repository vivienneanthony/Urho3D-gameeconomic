#ifndef CONNECTORDB_H
#define CONNECTORDB_H

#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Container/Str.h"



#include <mysql_connection.h>
#include <mysql_driver.h>


// Include the Connector/C++ headers
#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"


using namespace std;
using namespace sql;
using namespace Urho3D;

// define a structure of information for database
struct databaseconnection
{
    string hostname;
    string username;
    string password;
    string schema;
};

// class to connect to MySql Database
class URHO3D_API connectorDB : public LogicComponent
{
    OBJECT(connectorDB);

public:
    connectorDB(Context * context);
    virtual ~connectorDB();
    static void RegisterNewSubsystem(Context* context);

    // main class functions
    int setHost(string dbhost, string dbuser, string dbpassword);
    int connectHost(void);
    int setDB(string Schema);
    int executeQuery(string query);
    int checkTable(string checktable);
    int executePreparedStatement(string preparedstatement);
    Vector<String> executePreparedStatementResult(string preparedstatement);

protected:

private:

    bool VerifyValidConfiguration(void);

    /// Sql related pointers
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    sql::Statement *stmt;
    sql::ResultSetMetaData *res_meta;
    ///res_meta = rs -> getMetaData();
    ///int numcols = res_meta -> getColumnCount();

    /// db setup
    databaseconnection MySqlConnection;
    string schema;
};

#endif // CONNECTORDB_H
