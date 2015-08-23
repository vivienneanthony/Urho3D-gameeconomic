#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED


#include "Traits.h"

/// Player Object structure
struct PlayerAlliance
{
    /// Player alliances
    unsigned int AlienRace;
    bool AlienAllianceAligned;
};

struct PlayerBasicInfo
{
    /// name information
    String Firstname;
    String Middlename;
    String Lastname;
};

struct PlayerCharacteristics
{
    /// Player Characterteristics
    unsigned int Gender;
    unsigned int PersonalityTrait;
};

struct PlayerLevels
{
    /// Set player experience
    unsigned int Level;
    unsigned int Experience;
    unsigned int Reputation;

    /// player moves for surface of planet to a ship so it relations change for the x,y,z for respective object
    unsigned int Reputation1;
    unsigned int Reputation2;
    unsigned int Reputation3;
    unsigned int Reputation4;
    unsigned int Reputation5;
};

struct PlayerList
{
    /// name information
    String Firstname;
    String Middlename;
    String Lastname;
    unsigned int Level;
    unsigned int AlienRace;
    unsigned int AlienRaceAllianceAligned;
    unsigned int Gender;
    unsigned int PersonalityTrait;

    /// saved but unused
    String UniqueID;

};

struct PlayerObject
{
    /// healthy temporary
    int Health;

    /// name information
    String Firstname;
    String Middlename;
    String Lastname;

    /// Set player experience
    unsigned long Level;
    unsigned long Experience;
    unsigned long Reputation;

    /// player moves for surface of planet to a ship so it relations change for the x,y,z for respective object
    unsigned long Reputation1;
    unsigned long Reputation2;
    unsigned long Reputation3;
    unsigned long Reputation4;
    unsigned long Reputation5;

    /// character spec
    unsigned int AlienRace;
    bool AlienAllianceAligned;
    unsigned int Gender;
    unsigned int PersonalityTrait;

    /// Development
    unsigned TotalGroundPassiveTraits;
    Vector <Trait> GroundPassiveTrait;

    unsigned TotalSpacePassiveTraits;
    Vector <Trait> SpacePassiveTrait;

    /// Credits
    int Credits;

    /// Galaxy seed
    String GalaxySeed;

    /// saved but unused
    String UniqueID;
    String OwnerUniqueID;

};

#endif
