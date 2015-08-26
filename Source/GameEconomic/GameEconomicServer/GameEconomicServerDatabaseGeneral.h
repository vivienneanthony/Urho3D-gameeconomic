#ifndef GameEconomicServerDatabaseGeneral_H_INCLUDE
#define GameEconomicServerDatabaseGeneral_H_INCLUDE

/// setup access db types
enum accessdbtable
{
    AccessTable_None,
    AccessTable_Faction,
    AccessTable_AlienRace,
    AccessTable_Starbase,
    AccessTable_Personality
};

struct FactionInformation
{
  String Name;
  String Prefix;
  String Description;
  String UniqueID;
};

struct AlienRaceInformation
{
  String Name;
  String Prefix;
  String Description;
  String AlignedFaction;
  String UniqueID;
};

#endif

