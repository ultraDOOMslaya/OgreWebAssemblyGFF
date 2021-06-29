#pragma once
#ifndef __SimRunnable_h_
#define __SimRunnable_h_

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreBitesConfigDialog.h>
#include <OgreTrays.h>

#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>

#include "PathPlanning.h"

struct Cell {
	int x;
	int y;
};

class SimRunnable : public OgreBites::ApplicationContext, public OgreBites::InputListener, public OgreBites::TrayListener
{
public:
	SimRunnable();
	~SimRunnable();

	void setup(void);

	OgreBites::TrayManager* mTrayMgr;
	Ogre::SceneManager* mScnMgr;

	Ogre::Camera* mCamera;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneNode* mainNode;
	Ogre::SceneNode* cameraNode;
	Ogre::SceneNode* cameraAnchor;
	// OgreBites::SdkCameraMan* mCameraMan;

	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::TerrainGlobalOptions* mTerrainGlobals;

	bool mTerrainsImported;
	void configureTerrainDefaults(Ogre::Light* light);
	void initBlendMaps(Ogre::Terrain* terrain);
	void defineTerrain(long x, long y);

	// Map
	float width, height;
	std::vector<Cell> worldSpaceCells;

	// Create 'fields' on map
	void createFields(Ogre::Terrain* terrain, std::vector<std::pair<Ogre::Vector3, Ogre::Vector3>>);

	// Handle Animation
	bool nextLocation();
	std::deque<Ogre::Vector3*> mWalkList;

	Ogre::Real mDistance;
	Ogre::Real mWalkSpd;
	Ogre::Vector3 mDirection;
	Ogre::Vector3 mDestination;
	Ogre::AnimationState* mAnimationState;
	Ogre::Entity* mEntity;
	Ogre::SceneNode* mNode;

	bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	Ogre::Vector2 updateCoords();
	OgreBites::TextBox* mCoordsBox;
};

#endif __SimRunnable_h_