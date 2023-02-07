// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Vertex.h"
#include "Shaders.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#include <conio.h>

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

manager::ResourceManager* resourceManager;
manager::SceneManager* sceneManager;

int Init ( ESContext *esContext )
{
	// Test Resource Manager
	resourceManager = manager::ResourceManager::getInstance();
	resourceManager->Init("../Resources/Configuration/", "resourceManager.xml");
	resourceManager->DebugPrintResources();

	// Test Scene Manager
	sceneManager = manager::SceneManager::getInstance();
	sceneManager->Init("../Resources/Configuration/", "sceneManager.xml");
	sceneManager->DebugPrintAll();

	std::cout << "Managers done!\n";

	Vector3 colorBg = Globals::getColorBG();
	glClearColor(colorBg.x, colorBg.y, colorBg.z, 0.0f);

	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	
}

void CleanUp()
{
	delete resourceManager;
	delete sceneManager;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	if (Init(&esContext) != 0)
		return 0;

	esCreateWindow ( &esContext, Globals::getWindowName().c_str(), Globals::getScreenWidth(), Globals::getScreenHeight(), ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

