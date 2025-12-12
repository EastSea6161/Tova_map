#pragma once

/**
 * Import KTDB ÀÇ status
 */
enum KEMNodeType
{
	KEMNodeTypeZone   = 0,
	KEMNodeTypeNormal = 1,
    KEMNodeTypeFreightWareHouse = 2,
    KEMNodeTypeRail   = 9
};

enum KEMNodeFacilitiesType
{
	KEMNFacilitiesAirPort         = 1,
	KEMNFacilitiesAirportTerminal = 2,
	KEMNFacilitiesFarmersMarket   = 3,
	KEMNFacilitiesTruckTerminal   = 4,
	KEMNFacilitiesRailCY          = 5,
	KEMNFacilitiesPort            = 6,
	KEMNFacilitiesPortComplex     = 7,
	KEMNFacilitiesICD             = 8,
	KEMNFacilitiesITF             = 9,
	KEMNFaciliteiesRailStation    = 10

};