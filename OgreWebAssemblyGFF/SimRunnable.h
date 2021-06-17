#pragma once
#ifndef __SimRunnable_h_
#define __SimRunnable_h_

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreBitesConfigDialog.h>
#include <OgreTrays.h>


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
	Ogre::SceneNode* cameraNode;
	Ogre::SceneNode* cameraAnchor;
};

#endif __SimRunnable_h_