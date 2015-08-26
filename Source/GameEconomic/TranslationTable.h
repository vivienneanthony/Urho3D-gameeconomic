#ifndef TRANSLATIONTABLE_H
#define TRANSLATIONTABLE_H


#include <Urho3D/Urho3D.h>
#include <string>
using namespace Urho3D;
using namespace std;

#define ALIENRACESIZE 7
#define PERSONALITYLIMIT 8

enum AlienRaces
{
    AlienRace_Federation
};

static const char* AlienRacesIndentifiers[] =
{
    "vulcan","feds","nevs","ofaugath", "ateyin", "nume", "situ","conf"
};


static const char * PersonaltyIdentifiers[] =
{
    "Calm", "Dynamic","Disciplined", "Mature", "Stern", "Playful", "Seductive", "Anxious"
};

class TranslationTable
{
public:
    static AlienRaces GetAlienRaceInt(String InputString)
    {
        AlienRaces result = AlienRace_Federation;

        for(unsigned int idx=0; idx<ALIENRACESIZE; idx++)
        {
            if(String(AlienRacesIndentifiers[idx])==String(InputString))
            {
                result = AlienRaces(idx);
            }
        }
        return result;
    };

    static String GetAlienRaceInt(int AlienRace)
    {
        String result;

        for(unsigned int idx=0; idx<ALIENRACESIZE; idx++)
        {
            /// if alien race equal idx
            if(AlienRace == idx)
            {
                result = String(AlienRacesIndentifiers[idx]);
            }
        }
        return result;
    };


    static String GetPersonalityInt(unsigned int personalityidentifier)
    {
        String result;

        for(unsigned int idx=0; idx<PERSONALITYLIMIT; idx++)
        {
            /// if alien race equal idx
            if(personalityidentifier == idx)
            {
                const char * results = PersonaltyIdentifiers[idx];
                result = String(results);
            }
        }
        return result;
    };
};

#endif
