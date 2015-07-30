#ifndef ACCOUNT_H_INCLUDED
#define ACCOUNT_H_INCLUDED

#include <Urho3D/Urho3D.h>

#include <iostream>
#include <ctime>

#define ACCOUNTFILE "account.xml"

using namespace std;
using namespace Urho3D;

// define useraccount
struct AccountInformation
{
    long int AccountIdx;
    String Username;
    String Email;
    String Firstname;
    String Middlename;
    String Lastname;
    String Password;
    String UniqueID;
    time_t LastLogin;
};

#endif // ACCOUNT_H_INCLUDED


