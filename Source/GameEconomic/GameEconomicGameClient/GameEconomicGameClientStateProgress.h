#ifndef GAMEECONOMICGAMECLIENTSTATEPROGRESS_H
#define GAMEECONOMICGAMECLIENTSTATEPROGRESS_H

enum MapDataType
{
    MapData_Clustered,
    MapData_NonClustered
};

enum MapDataElementType
{
    MapDataElement_Unknown,
    MapDataElement_StructureFloor,
    MapDataElement_StructureWall,
    MapDataElement_StructureRoof,
    MapDataElement_StructureCeiling,
    MapDataElement_StructureForceFieldBayDoor,
    MapDataElement_ElementCeilingLight,
    MapDataElement_CargoAreaMarker
};

#endif
