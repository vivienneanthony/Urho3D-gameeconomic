#ifndef TRAITS_H_INCLUDE
#define TRAITS_H_INCLUDE

/// enum
enum GroundPassiveTrait
{
    GroundPassiveDefault=0
};

enum SpacePassiveTrait
{
    SpacePassiveDefault=0
};

enum SpecialPassiveTrait
{
    SpecialPassiveDefault=0
};

/// Traits
struct Trait
{
    unsigned int TraitType;
    unsigned int ReputationTrait; // 0 - none, then rest maybe from rep
    bool TraitActive;
};


#endif
