#include "SimRunnable.h"

namespace Offsets {
	/** Map builders **/
	extern const float worldSize = 2000;
	extern const float baseHeight = 200;

	extern const int cellSize = 80;
	extern const float heightOffset = 200;

	extern const float brushSize = 0.025;
}

SimRunnable::SimRunnable()
	: mTerrainGroup(0),
	mTerrainGlobals(0)
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
	// SDL_ShowCursor(SDL_DISABLE);

	Ogre::Root* root = getRoot();
	Ogre::ConfigDialog* dialog = OgreBites::getNativeConfigDialog();

	mScnMgr = root->createSceneManager();
	mScnMgr->addRenderQueueListener(getOverlaySystem());
	mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow(), this);
	mTrayMgr->showFrameStats(OgreBites::TrayLocation::TL_BOTTOMLEFT);

	addInputListener(mTrayMgr);

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mScnMgr);

	cameraAnchor = mScnMgr->getRootSceneNode()->createChildSceneNode("camAnchor");
	cameraNode = cameraAnchor->createChildSceneNode("camNode");
	mCamera = mScnMgr->createCamera("myCam");
	cameraAnchor->setPosition(Ogre::Vector3::ZERO + Ogre::Vector3(500, 500, 1000));
	//camAnchor->setPosition(Ogre::Vector3::ZERO + Ogre::Vector3(1683, 1900, 2116));
	cameraNode->lookAt(Ogre::Vector3(500, 0, 80), Ogre::Node::TransformSpace::TS_WORLD);

//
	cameraNode->attachObject(mCamera);
	Ogre::Viewport* vp = getRenderWindow()->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	root->addFrameListener(this);

	mScnMgr->setAmbientLight(Ogre::ColourValue(0.85f, 0.85f, 0.85f));
	Ogre::Light* spotLight = mScnMgr->createLight("SpotLight");
	spotLight->setType(Ogre::Light::LT_DIRECTIONAL);
	spotLight->setDirection(Ogre::Vector3(1.0f, -1.0f, -1.0f).normalisedCopy());
	spotLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
	spotLight->setSpecularColour(0.25f, 0.25f, 0.25f);

	/** Terrain loading **/
	mTerrainGlobals = new Ogre::TerrainGlobalOptions();


	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
		mScnMgr,
		Ogre::Terrain::ALIGN_X_Z,
		513, Offsets::worldSize);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrainNewEdit"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
	mTerrainGroup->setOrigin(Ogre::Vector3(1000, 0, 1000));

	configureTerrainDefaults(spotLight);

	defineTerrain(0, 0);

	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();

	// Set up grid cells
	int width = Offsets::worldSize / Offsets::cellSize;
	int height = Offsets::worldSize / Offsets::cellSize;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Cell cell;
			cell.x = x;
			cell.y = y;
			worldSpaceCells.push_back(cell);
		}
	}

	// Map Grid is initialized, time for a coloring book
	std::vector<std::pair<Ogre::Vector3, Ogre::Vector3>> testFieldList;

	// Rectangles on the map to be fields
	// First coordianate is upper left corner, second is bottom right corner
	testFieldList.push_back(std::make_pair(Ogre::Vector3(1, 0, 1), Ogre::Vector3(5, 0, 6)));

	testFieldList.push_back(std::make_pair(Ogre::Vector3(0, 0, 8), Ogre::Vector3(6, 0, 9)));

	testFieldList.push_back(std::make_pair(Ogre::Vector3(7, 0, 1), Ogre::Vector3(8, 0, 2)));

	// Create a checkerboard for visualization purposes
	
	for (int i = 0; i < worldSpaceCells.size(); i++)
	{
		int cellX = worldSpaceCells[i].x;
		int cellY = worldSpaceCells[i].y;
		if (cellX % 2 == 0 && cellY % 2 == 0)
		{
			testFieldList.push_back(std::make_pair(Ogre::Vector3(cellX, 0, cellY), Ogre::Vector3(cellX+1, 0, cellY+1)));
		}
		else if (cellX % 2 != 0 && cellY % 2 != 0)
		{
			testFieldList.push_back(std::make_pair(Ogre::Vector3(cellX, 0, cellY), Ogre::Vector3(cellX + 1, 0, cellY + 1)));
		}
	}
	
	Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
	while (ti.hasMoreElements())
	{
		Ogre::Terrain* t = ti.getNext()->instance;
		createFields(t, testFieldList);
	}

	mScnMgr->setSkyBox(true, "Examples/SpaceSkyBox", 300);

	/** Set up 'tractor' - atm just any entity */
	Ogre::Entity* myEntity = mScnMgr->createEntity("robot.mesh");
	Ogre::SceneNode* myNode = mScnMgr->getRootSceneNode()->createChildSceneNode();

	myNode->setPosition(80, 200, 80);
	//myNode->roll(Ogre::Degree(90.0), Ogre::Node::TS_PARENT);
	//myNode->yaw(Ogre::Degree(-90));
	//myNode->setScale(1, 1, 1);
	myNode->attachObject(myEntity);


}

void SimRunnable::createFields(Ogre::Terrain* terrain, std::vector<std::pair<Ogre::Vector3, Ogre::Vector3>> fields)
{
	Ogre::Vector3 tsPos;
	Ogre::Vector3 tsCenterPos;
	Ogre::Vector3 centerCellPos;

	// blend pointers
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	Ogre::Real terrainSize = (terrain->getSize() - 1);

	Ogre::uint16 blendMapSize = terrain->getLayerBlendMapSize();
	Ogre::Vector3 tsBlendStart;
	Ogre::Vector3 tsBlendEnd;

	for (int i = 0; i < fields.size(); i++)
	{
		Ogre::Vector3 startCell = fields[i].first;
		Ogre::Vector3 endCell = fields[i].second;

		terrain->getTerrainPosition(startCell * Offsets::cellSize, &tsBlendStart);
		terrain->getTerrainPosition(endCell * Offsets::cellSize, &tsBlendEnd);

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
					pBlend0[index] = 0.25f;
					pBlend1[index] = 1.0f;
				}
				index++;
			}
		}
	}

	blendMap0->dirty();
	blendMap0->update();
	blendMap1->dirty();
	blendMap1->update();
	mTerrainGroup->update();
}

//----------------------------------------------------------------
void SimRunnable::configureTerrainDefaults(Ogre::Light* light)
{
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(6000);

	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mScnMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
	importData.terrainSize = 513;
	importData.worldSize = Offsets::worldSize;
	importData.inputScale = 200;
	importData.minBatchSize = 33;
	importData.maxBatchSize = 65;

	importData.layerList.resize(3);
	
	// Base layer, 'grass'
	importData.layerList[0].worldSize = 300;
	importData.layerList[0].textureNames.push_back("jungle_0_diffusespecular.dds");
	importData.layerList[0].textureNames.push_back("jungle_0_normalheight.dds");

	// Blend layer to make the grass look prettier
	importData.layerList[1].worldSize = 500;
	importData.layerList[1].textureNames.push_back("jungle_4_diffusespecular.dds");
	importData.layerList[1].textureNames.push_back("jungle_4_normalheight.dds");

	// Field layer
	importData.layerList[2].worldSize = 200;
	importData.layerList[2].textureNames.push_back("tiaga_seamless_0_diffusespecular.png");
	importData.layerList[2].textureNames.push_back("tiaga_seamless_0_normalheight.png");
}

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("white.bmp", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();

}

void SimRunnable::defineTerrain(long x, long y)
{
	//mTerrainGroup->defineTerrain(x, y);
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	//Ogre::String filename = "terrain_bogaloo.dat";
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
	{
		mTerrainGroup->defineTerrain(x, y);
	}
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);
		mTerrainsImported = true;
	}
}
//----------------------------------------------------------------

void SimRunnable::initBlendMaps(Ogre::Terrain* terrain)
{
	float minHeight0 = 20;
	float fadeDist0 = 15;
	float minHeight1 = 70;
	float fadeDist1 = 15;

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			*pBlend0++ = 0.1f;
			*pBlend1++ = 0.1f;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
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