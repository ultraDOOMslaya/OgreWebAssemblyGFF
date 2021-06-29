#include "PathPlanning.h"



void PathPlanning::planPath(std::deque<Ogre::Vector3*>* mWalkList) {
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 1));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 2));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 3));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 4));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 5));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 6));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 7));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 8));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 9));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 10));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 11));
	mWalkList->push_back(new Ogre::Vector3(0, 2.5, 12));

	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 12));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 11));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 10));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 9));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 8));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 7));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 6));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 5));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 4));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 3));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 2));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 1));
	mWalkList->push_back(new Ogre::Vector3(1, 2.5, 0));

	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 0));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 1));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 2));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 3));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 4));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 5));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 6));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 7));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 8));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 9));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 10));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 11));
	mWalkList->push_back(new Ogre::Vector3(2, 2.5, 12));

	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 12));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 11));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 10));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 9));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 8));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 7));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 6));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 5));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 4));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 3));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 2));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 1));
	mWalkList->push_back(new Ogre::Vector3(3, 2.5, 0));
	
}


void getCellCenter(Ogre::Vector3 centerPosition, Ogre::Vector3* centerCellPosition)
{
	int x = centerPosition.x;
	int z = centerPosition.z;

	int cell_x = x / 80;
	int cell_z = z / 80;

	int cell_width = (cell_x * 80) + (80 / 2);
	int cell_height = (cell_z * 80) + (80 / 2);

	centerCellPosition->x = cell_width;
	centerCellPosition->z = cell_height;
}

void PathPlanning::alterTerrain(Ogre::Terrain* terrain, Ogre::TerrainGroup* terrainGroup, Ogre::Vector3 centerPosition, Ogre::Vector2 gridCoordinates) {
	Ogre::Real terrainSize = (terrain->getSize() - 1);
	Ogre::Vector3 tsPos;
	Ogre::Vector3 tsCenterPos;
	Ogre::Vector3 centerCellPos;
	terrain->getTerrainPosition(centerPosition, &tsPos);
	getCellCenter(centerPosition, &centerCellPos);
	terrain->getTerrainPosition(centerCellPos, &tsCenterPos);

	// blend pointers
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	int wsBlendStart_X = centerCellPos.x - (80 / 2);
	int wsBlendEnd_X = centerCellPos.x + (80 / 2);

	int wsBlendStart_Z = centerCellPos.z - (80 / 2);
	int wsBlendEnd_Z = centerCellPos.z + (80 / 2);

	Ogre::Vector3 wsBlendStart = Ogre::Vector3(wsBlendStart_X, 0, wsBlendStart_Z);
	Ogre::Vector3 wsBlendEnd = Ogre::Vector3(wsBlendEnd_X, 0, wsBlendEnd_Z);

	Ogre::uint16 blendMapSize = terrain->getLayerBlendMapSize();
	Ogre::Vector3 tsBlendStart;
	Ogre::Vector3 tsBlendEnd;

	terrain->getTerrainPosition(wsBlendStart, &tsBlendStart);
	terrain->getTerrainPosition(wsBlendEnd, &tsBlendEnd);

	Ogre::uint16 startTerrain_X = tsBlendStart.x * terrainSize;
	Ogre::uint16 endTerrain_X = tsBlendEnd.x * terrainSize;

	Ogre::uint16 startTerrain_Y = tsBlendStart.y * terrainSize;
	Ogre::uint16 endTerrain_Y = tsBlendEnd.y * terrainSize;

	Ogre::uint16 start_X = tsBlendStart.x * blendMapSize;
	Ogre::uint16 end_X = tsBlendEnd.x * blendMapSize;

	Ogre::uint16 start_Y = (1 - tsBlendStart.y) * blendMapSize;
	Ogre::uint16 end_Y = (1 - tsBlendEnd.y) * blendMapSize;

	int index = 0;

	for (Ogre::uint16 y = 0; y < blendMapSize; ++y)
	{
		for (Ogre::uint16 x = 0; x < blendMapSize; ++x)
		{

			if ((x >= start_X && x <= end_X) && (y >= start_Y && y <= end_Y))
			{
				pBlend0[index] = 0.5f;
				pBlend1[index] = 0.5f;

			}
			index++;
		}
	}

	blendMap0->dirty();
	blendMap0->update();
	blendMap1->dirty();
	blendMap1->update();
	terrainGroup->update();
}

