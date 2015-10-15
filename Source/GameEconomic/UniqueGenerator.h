#ifndef UNIQUEGENERATOR_H
#define UNIQUEGENERATOR_H


#include <Urho3D/Urho3D.h>
#include <string>
using namespace Urho3D;
using namespace std;

/// create a const string
static const char Choices[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

class UniqueGenerator
{
public:
    static string * GenerateUniqueID(unsigned int length)
    {
        /* initialize random seed: */
        //srand (time(NULL));

        /// create id string
        string * NewGenerateUniqueID;

        NewGenerateUniqueID = new string();

        /// createa char int
        unsigned char selectedchar;

        for(unsigned int i=0; i<length; i++)
        {
            ///if i is 1 or above
            if(i)
            {
                do
                {
                    selectedchar = Choices[rand()%62];
                }
                while(selectedchar==NewGenerateUniqueID->at(i-1));
            }
            else
            {
                selectedchar = Choices[rand()%62];
            }

            NewGenerateUniqueID->push_back(selectedchar);
        }

        return NewGenerateUniqueID;
    }
};

#endif
