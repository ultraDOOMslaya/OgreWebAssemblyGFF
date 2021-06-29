#pragma once

#ifndef __PathPlanning_h_
#define __PathPlanning_h_

#include <deque>

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>

class PathPlanning
{
public:
	static void planPath(std::deque<Ogre::Vector3*>* mWalkList);
	static void alterTerrain(Ogre::Terrain* terrain, Ogre::TerrainGroup* terrainGroup, Ogre::Vector3 centerPosition, Ogre::Vector2 gridCoordinates);
};

#endif __PathPlanning_h_