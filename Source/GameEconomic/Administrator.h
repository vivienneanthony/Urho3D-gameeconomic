#ifndef ADMINISTRATORS_H_INCLUDED
#define ADMINISTRATORS_H_INCLUDED

#include <Urho3D/Urho3D.h>

#include <iostream>
#include <ctime>

#define ACCOUNTFILE "account.xml"

using namespace std;
using namespace Urho3D;

// define useraccount
struct AdministratorInformation
{
    long int Idx;
    String Username;
    String Email;
    String Password;
    String UniqueID;
    int AdminAccounts;
};

#endif // ADMINSITRATORS_H_INCLUDED




