#ifndef ResourceManager_H_INCLUDED
#define ResourceManager_H_INCLUDED

#include "ResourceComponent.h"

///
/// Periodic Materials
///

static const char * PeriodicNameIdentifiers[] =
{
    "Hydrogen", "Helium","Lithium","Beryllium","Boron","Carbon","Nitrogen","Oxygen","Fluorine","Neon","Sodium","Magnesium","Aluminum","Silicon","Phosphorus","Sulfur","Chlorine","Argon","Potassium","Calcium","Scandium","Titanium","Vanadium","Chromium","Manganese","Iron","Cobalt","Nickel","Copper","Zinc","Gallium","Germanium","Arsenic","Selenium","Bromine","Krypton","Rubidium","Strontium","Yttrium","Zirconium","Niobium","Molybdenum","Technetium","Ruthenium","Rhodium","Palladium","Silver","Cadmium","Indium","Tin","Antimony","Tellurium","Iodine","Xenon","Cesium","Barium","Lanthanum","Cerium","Praseodymium","Neodymium","Promethium","Samarium","Europium","Gadolinium","Terbium","Dysprosium","Holmium","Erbium","Thulium","Ytterbium","Lutetium","Hafnium","Tantalum","Tungsten","Rhenium","Osmium","Iridium","Platinum","Gold","Mercury","Thallium","Lead","Bismuth","Polonium","Astatine","Radon","Francium","Radium","Actinium","Thorium","Protactinium","Uranium","Neptunium","Plutonium","Americium"
};

static const char * PeriodicSymbolIdentifiers[] =
{
    "H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am"
};

static const char * PeriodicCompositeResourceIdentifiers[] =
{
    "H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am"
};

static const float PeriodicDensityIdentifiers[]=
{
    0.09f,0.18f,0.53f,1.85f,2.34f,2.26f,1.25f,1.43f,1.7f,0.9f,0.97f,1.74f,2.7f,2.33f,1.82f,2.07f,3.21f,1.78f,0.86f,1.55f,2.99f,4.54f,6.11f,7.19f,7.43f,7.87f,8.9f,8.9f,8.96f,7.13f,5.91f,5.32f,5.72f,4.79f,3.12f,3.75f,1.63f,2.54f,4.47f,6.51f,8.57f,10.22f,11.5f,12.37f,12.41f,12.02f,10.5f,8.65f,7.31f,7.31f,6.68f,6.24f,4.93f,5.9f,1.87f,3.59f,6.15f,6.77f,6.77f,7.01f,7.3f,7.52f,5.24f,7.9f,8.23f,8.55f,8.8f,9.07f,9.32f,6.9f,9.84f,13.31f,16.65f,19.35f,21.04f,22.6f,22.4f,21.45f,19.32f,13.55f,11.85f,11.35f,9.75f,9.3f,0.01f,9.73f,0.01f,5.5f,10.07f,11.72f,15.4f,18.95f,20.2f,19.84f,13.67f
};

static const ResourceComponentState PeriodicStateIdentifiers[]=
{
    RCState_Gaseous,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Gaseous,RCState_Gaseous,RCState_Gaseous,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Gaseous,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid
};

static const ResourceComponentType PeriodicTypeIdentifiers[]=
{
    RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement,RCType_PeriodicElement
};

///
/// Raw Materials
///
static const char * RawMaterialNameIdentifiers[] =
{
    "Water","Steel","Stainless Steel", "Silica","Aluminosilicate Glass"
};


static const char * RawMaterialSymbolIdentifiers[] =
{
    "Water","Steel","StainlessSteel", "Silica","AluminGlass"
};

static const char * RawMaterialCompositeResourceIdentifiers[] =
{
    "H2:0:1","Fe:1:C:1","Fe:1:Cr:1:Ni:1", "Si:1:O:2","Al:2:O:3"
};

static const float RawMaterialDensityIdentifiers[]=
{
    1.0f,1.0f,1.0f,1.0f,1.0f
};

static const ResourceComponentState RawMaterialStateIdentifiers[]=
{
    RCState_Liquid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid
};

static const ResourceComponentType RawMaterialTypeIdentifiers[]=
{
    RCType_RawMaterial,RCType_RawMaterial,RCType_RawMaterial,RCType_RawMaterial,RCType_RawMaterial
};

/// Tier3

static const char * ComponentsToolsNameIdentifiers[] =
{
    "Processing Unit","Focused Beam Emitter","Variable Beam Emitter","LED Matrix Emitter","Focused Beam Cutter", "Focused Beam Welder"
};

static const char * ComponentsToolsSymbolIdentifiers[] =
{

    "ProcessingUnit","FousedBeamEmitter","VariableBeamEmitter","LedMatrixEmitter","FocusedBeamCutter", "FocusedBeamWelder"
};

static const char * ComponentsToolsCompositeResourceIdentifiers[] =
{

    "Silicia:1", "Silicia:1", "Silicia:2", "Silicia:1:AluminGlass:1", "Silicia:1:StainlessSteel:3:ProcessingUnit:1:FocusedBeamEmitter:1", "Silicia:1:StainlessSteel:3:ProcessingUnit:1:FocusedBeamEmitter:1"
};

static const float ComponentsToolsDensityIdentifiers[]=
{

    1.0f,1.0f,1.0f,1.0f,1.0f,1.0f
};

static const ResourceComponentState ComponentsToolsStateIdentifiers[]=
{
    RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid
};

static const ResourceComponentType ComponentsToolsTypeIdentifiers[]=
{
    RCType_Component,RCType_Component,RCType_Tool,RCType_Tool,RCType_Tool,RCType_Tool
};


///Tier 4
static const char * PrimaryResourcesNameIdentifiers[] =
{
    "Floor","Wall2","Wall3","Wall4","Wall5","Wall6","Wall7","Wall8","Wall9","Wall10","Wall11","Wall12","Wall13","Wall14","Wall15","Wall16","Wall17","Wall18","Ceiling","Structure","Structure","Structure", "Forcefield","Forcefield","Forcefield","Forcefield","Light"
};

static const char * PrimaryResourcesSymbolIdentifiers[] =
{

    "GenericType1","GenericType2","GenericType3","GenericType4","GenericType5","GenericType6","GenericType7","GenericType8","GenericType9","GenericType10","GenericType11","GenericType12","GenericType13","GenericType14","GenericType15","GenericType16","GenericType17","GenericType18","GenericType19","GenericType20","GenericType21","GenericType22","GenericType24","GenericType25","GenericType26","GenericType27","GenericType28"
};

static const char * PrimaryResourcesCompositeResourceIdentifiers[] =
{

    "StainlessSteel:100","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:200","StainlessSteel:100","","","","","","","StainlessSteel:2:LedMatrixEmitter:1:AlimuGlass:4"
};

static const float PrimaryResourcesDensityIdentifiers[]=
{

    1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f
};

static const ResourceComponentState PrimaryResourcesStateIdentifiers[]=
{
    RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_Solid,RCState_NotApplicable,RCState_NotApplicable,RCState_NotApplicable,RCState_NotApplicable,RCState_Solid
};

static const ResourceComponentType PrimaryResourcesTypeIdentifiers[]=
{
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Structural,
    RCType_Forcefield,
    RCType_Structural,RCType_Forcefield,RCType_Forcefield,RCType_Forcefield,RCType_Light
};


struct ResourceManagerInformation
{
    /// Name and Prefix
    String Name;
};

struct TranslationTableResourceInformation
{
    unsigned int ResourceID;
    String ResourceString;
    ResourceComponentType ResourceType;
};


#endif
