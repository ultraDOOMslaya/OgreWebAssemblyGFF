# OgreWebAssemblyGFF

This project uses Ogre3d (specifically Ogre1) as a 3d graphics rendering engine. Ogre3d can be worked with in different configurations and operating system but this setup guide is specifically for MS Windows using Visual Studio. VS Community can be used to setup and work with the project.

## Project Setup

### Microsoft Visual Studio

After opening the project SLN in visual studio, configuration must be set so the project can run with the OgreSDK. You can open the project configuration by going to:

> Project -> ${ProjectName} Properties

in Visual Studio.

#### Setting Project Configuration

1) At the top of the dialogue, set the configuration to **Release** and the Platform to **x64**.

##### SubMenu: Configuration Properties (top level)

This tells visual studio where the generated exe will be and what it will be named.

1) Under Debugging:

* Set **Command** to `$(OGRE_HOME)\bin\$(ProjectName).exe`
* Set **Working Directory** to `$(OGRE_HOME)\bin`

##### SubMenu: Configuration Properties > C/C++

This tells visual studio where to find header files for external libraries such as the OgreSDK.

1) Under General:

* Set **Additional Include Directories** to:

  `
  $(OGRE_HOME)\include\OGRE\RTShaderSystem;
  $(OGRE_HOME)\include\OGRE\Terrain;
  $(OGRE_HOME)\include\OGRE\Overlay;
  $(OGRE_HOME)\include\OGRE\Bites;
  $(OGRE_HOME)\include\OGRE;
  $(OGRE_HOME)\include;
  %(AdditionalIncludeDirectories)
  `

More may need to be added depending upon which Ogre tooling is being used.

##### SubMenu: Configuration Properties > Linker

This tells visual studio where to find the DLL's for external tooling.

1) Under General:

* Set **Additional Library Directories** to:

  `
  $(OGRE_HOME)\lib;
  $(OGRE_HOME)\lib\OGRE;
  %(AdditionalLibraryDirectories)
  `

More may need to be added depending upon which Ogre tooling is being used.

2) Under Input:

* Set **Additional Dependencies** to:

  `
  OgreBites.lib
  OgreMain.lib
  OgreOverlay.lib
  OgreRTShaderSystem.lib
  OgreTerrain.lib
  `

More may need to be added depending upon which Ogre tooling is being used. All libraries added should exist within one of the library directories added in step 1.

3) Under System:

* Set **Subsystem** to:

  `
  Windows (/SUBSYSTEM:WINDOWS)
  `

This allows VS to find the main method.

##### SubMenu: Configuration Properties > Build Events

This tells visual studio where put the exe after it has been compiled. The exe needs to be placed where the Ogre dlls are so: $(OGRE_HOME)\bin

1) Under Post-Build Event:

* Set **Command Line** to:

  copy "$(OutDir)\$(TargetFileName)" "$(OGRE_HOME)\bin"

#### Running the Project

The solution configuration and platform must be set appropriately in order to run the project. You can find these dropdowns next to the start button in visual studio.

1) Set the Solution Configurations dropdown to: `Release`
2) Set the Solution Platform to: `x64`

Hitting the start button will run the project.
