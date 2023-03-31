// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Vertex.h"
#include "Shaders.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputController.h"

#include <conio.h>

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

manager::ResourceManager* resourceManager;
manager::SceneManager* sceneManager;
manager::InputController* inputController;

GLfloat maxTime = 0.05f;
GLfloat currentTime;

int Init ( ESContext *esContext )
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Test Resource Manager
	resourceManager = manager::ResourceManager::getInstance();
	resourceManager->Init("../Resources/Configuration/", "resourceManager.xml");
	resourceManager->DebugPrintResources();

	// Test Scene Manager
	sceneManager = manager::SceneManager::getInstance();
	sceneManager->Init("../Resources/Configuration/", "sceneManager.xml");
	sceneManager->DebugPrintAll();

	std::cout << "Managers done!\n";

	// Test Input Controller
	inputController = manager::InputController::getInstace();


	Vector3 colorBg = Globals::getColorBG();
	glClearColor(colorBg.x, colorBg.y, colorBg.z, 0.0f);

	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager->Draw(esContext);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	currentTime += deltaTime;
	if (currentTime > maxTime) {
		currentTime -= maxTime;
		sceneManager->Update(esContext, deltaTime);
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	inputController->DoAction(key);
}

void CleanUp()
{
	delete resourceManager;
	delete sceneManager;
	delete inputController;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow(&esContext, Globals::getWindowName().c_str(), Globals::getScreenWidth(), Globals::getScreenHeight(), ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

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

