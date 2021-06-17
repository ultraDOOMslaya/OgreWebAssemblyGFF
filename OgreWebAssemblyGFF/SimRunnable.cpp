#include "SimRunnable.h"

SimRunnable::SimRunnable()
{
}
//----------------------------------------------------------------

SimRunnable::~SimRunnable()
{
}
//----------------------------------------------------------------

void SimRunnable::setup(void) {
	OgreBites::ApplicationContext::setup();
	addInputListener(this);

	Ogre::Root* root = getRoot();
	Ogre::ConfigDialog* dialog = OgreBites::getNativeConfigDialog();

	mScnMgr = root->createSceneManager();
	mScnMgr->addRenderQueueListener(getOverlaySystem());
	mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow(), this);
	mTrayMgr->showFrameStats(OgreBites::TrayLocation::TL_BOTTOMLEFT);

	addInputListener(mTrayMgr);

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mScnMgr);

	cameraAnchor = mScnMgr->getRootSceneNode()->createChildSceneNode("cameraAnchor");
	cameraNode = cameraAnchor->createChildSceneNode("cameraNode");
	mCamera = mScnMgr->createCamera("myCamera");
	cameraNode->setPosition(Ogre::Vector3::ZERO + Ogre::Vector3(940, 1050, 1158));
	cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TransformSpace::TS_WORLD);

	mCamera->setNearClipDistance(5);
	cameraNode->attachObject(mCamera);
	Ogre::Viewport* vp = getRenderWindow()->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	root->addFrameListener(this);

	//Higher the ambience: higher the brightness
	mScnMgr->setAmbientLight(Ogre::ColourValue(0.85f, 0.85f, 0.85f));
	Ogre::Light* spotLight = mScnMgr->createLight("SpotLight");
	spotLight->setType(Ogre::Light::LT_DIRECTIONAL);
	spotLight->setDirection(Ogre::Vector3(1.0f, -1.0f, -1.0f).normalisedCopy());
	spotLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
	spotLight->setSpecularColour(0.25f, 0.25f, 0.25f);
}
//----------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char* argv[])
#endif
	{
		// Create application object
		SimRunnable app;
		try {
			app.initApp();
			app.getRoot()->startRendering();
			app.closeApp();
			return 0;
		}
		catch (Ogre::Exception & e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}
		return 0;
	}
#ifdef __cplusplus
}
#endif