Team Name: Stealth Studios

- Game Concept - 
Explore the mysterious cave caused by a great earthquake with EXA and ELA. 
Reach the cave’s core by teleporting your way past obstacles and puzzles in the cave.


= Game Credits =
Stealth Studios
Team Members:
[BSGD]  Chan Wai Kit Terence	- Producer, AI, Input
[BSGD]  Javier Foo		- Graphics, Animation, Audio
[RTIS]  Lim Jia Hao		- Tech Lead, Sound Designer, Physics, Collision
[RTIS]  Loh Kai Yi		- Tools Programmer, Architecture, Quality Assurance
[BAGD]  Darren Ang Jun Kai	- Art Lead, Mechanics Design
[BAGD]  Chong Teck Hsun		- Design Lead, Level Design

Special Thanks to:
	*President Claude Comair
	*Instructors
	- Hosry Elie
	- Thompson Michael David
	- Pak Lawrence Zhi Heng
	- KEH Choon Wee
	- Teo Yi Quan Raymond

All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.


= Game Controls =
A & D key: 		Move left annd right
Spacebar: 		Jump
Mouse: 			Aim
Release Left Click: 	Shoot Projectile
Hold Left Click:	Activate Slow Motion
Right Click:		Teleport to Projectile

= Level Editor =

- System and Editor settings can be found in
  Root Folder > RogueEngine > Resources > Config
- To toggle Editor mode on/off, press F5
- To toggle Debug draw on/off , press F3
- To toggle VSync on/off, press F6
- To toggle Fullscreen on/off, press F11
Docking
- To dock the windows, drag and drop each window and release them

-----------------------------------------------------------------------------------------------------------------------------------
Menu Bar(Top left of editor)

File
-Load Scene
-Save Scene
-New Scene
-Save Scene As
-Exit

GameObject
-Load Prefab
-Create 2d sprite

-----------------------------------------------------------------------------------------------------------------------------------

Settings(Bottom right of editor)
-Change background style
-Show ImGui Demo

-----------------------------------------------------------------------------------------------------------------------------------

Profiler(Bottom right of editor)
-FPS displayed
-Time of each systems takes to load displayed
-Graph of time of each system displayed

-----------------------------------------------------------------------------------------------------------------------------------

Console(Bottom right of editor)
-Debugging information displayed
-Enter content in search bar to search debugging information
-Clear button to clear current debug
-----------------------------------------------------------------------------------------------------------------------------------

Hierarchy Window(Left of editor)
-Create empty gameobject
-Display current objects in the scene
-Search for tags and name
-Double click to select object

-----------------------------------------------------------------------------------------------------------------------------------

Project/File (Bottom of editor)
- Assets folder can be found in Root Folder > RogueEngine > Resources 
- Double click on folder hierarchy to load the files
- Drag and drop files to other windows of editor to load them

-----------------------------------------------------------------------------------------------------------------------------------

Viewport(Center of editor)
- Press play to play the current scene, if scene is paused, level will be saved
- Press pause to pause the current scene
- Press stop to stop the current scene and load the last saved scene
- Press Step to skip a certain number of frames when debugging
- Press Translate(Q)/Rotate(W)/Scale(E) to swop the modes to edit the properties
- Double click on objects in the viewport to select them
- Levels can be loaded by drag and drop of json files from the file system

Translate Mode
	-Drag and hold mouse button on the arrow to move object
	-Move red arrow to move along x axis
	-Move green arrow to move along y axis
	-Move blue box to move along both axis
Rotate Mode
	-Drag and hold out circle to rotate object
Scale Mode
	-Drag and hold mouse button on the arrow to scale object
	-Move red arrow to scale along x axis
	-Move green arrow scale move along y axis
-----------------------------------------------------------------------------------------------------------------------------------
Inspector (Right of editor)
-Toggle world camera with checkbox
-Set zoom (F1 button zoom in button in the scene and F2 to zoom out when world camera is not toggled)
-Toggle world gravity

When object is selected
-Press Add Component to attach components to the game object
-Press Delete Component to delete components to the game object
-Press Clone Object to clone the current game object with all the components data
-Press Delete Object to remove the current game object from the current scene
-Enter New Name and Apply to change name of the object
-Enter tag and press Add New Tag to add a new tag to the object
-Press Save As Prefab to save the current game object as a prefab with current data of the components
-Click on current components and drag the slider floats to edit data in the scene
-For audio and sprite component, those data can be loaded by drag and drop from the file system
-----------------------------------------------------------------------------------------------------------------------------------
Difference in Debug and Release
-Release does optimisations for the engine which results in higher FPS than debug
-Asserts will happen in debug and not in release mode