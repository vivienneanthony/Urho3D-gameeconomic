#ifndef TRANSLATIONTABLERESOURCES_H
#define TRANSLATIONTABLERESOURCES_H


#include <Urho3D/Urho3D.h>
#include <string>

using namespace Urho3D;
using namespace std;


struct TranslationTableResourceInformation
{
    unsigned int ResourceID;
    String ResourceString;
};

class TranslationTableResources
{
public:
    Vector <TranslationTableResourceInformation> ResourcesTranslation;

    static unsigned int GetResourceTranslationInt(String InputString)
    {
        unsigned int  result = 0;
/*
        for(unsigned int idx=0; idx<ALIENRACESIZE; idx++)
        {
            if(String(AlienRacesIndentifiers[idx])==String(InputString))
            {
                result = AlienRaces(idx);
            }
        }*/
        return result;
    };

    static String GetResourceTranslationInt(unsigned int ResourceTranslationInput)
    {
        String result;

        /*
        for(unsigned int idx=0; idx<ALIENRACESIZE; idx++)
        {
            /// if alien race equal idx
            if(AlienRace == idx)
            {
                result = String(AlienRacesIndentifiers[idx]);
            }
        }*/
        return result;
    };

    /// Add a resource
    static void AddResourceTranslation(unsigned int value, String Symbol)
    {
        ResourcesTranslation.Push({value,Symbol});
        return;
    }

private:


};

#endif

