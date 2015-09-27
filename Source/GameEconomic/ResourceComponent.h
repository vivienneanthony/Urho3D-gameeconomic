#ifndef ResourceComponent_H_INCLUDED
#define ResourceComponent_H_INCLUDED

enum ResourceComponentType
{
    /// Resource list
    RCType_None,
    RCType_PeriodicUnidentified,
    RCType_PeriodicElement,
    RCType_RawMaterial,
    RCType_Component,
    RCType_Tool,
    RCType_Forcefield,
    RCType_Structural,
    RCType_Light,
    RCType_PowerSource,
    RCType_ReplicationPrinter,
    RCType_RefrigerationUnit,
    RCType_Drone,
    RCType_NotApplicable=99999
};

enum ResourceComponentState
{
    RCState_None,
    RCState_PeriodicUnidentified,
    RCState_Gaseous,
    RCState_Liquid,
    RCState_Solid,
    RCState_NotApplicable=99999
};

struct ResourceComponentInformation
{
    /// Name and Prefix
    String Name;
    String Symbol;

    /// Element
    ResourceComponentType ComponentType;
    ResourceComponentState ComponentState;

    /// Desriptor
    float Density;

    /// ResourceComponents
    String ComponentResource;
};

#endif
