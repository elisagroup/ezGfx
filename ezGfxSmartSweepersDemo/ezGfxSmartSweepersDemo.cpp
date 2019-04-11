
#include "ezGfx.h"
#include "CController.h"

//The controller class for this simulation
CController*	g_pController = NULL;

//create an instance of the parameter class.
CParams   g_Params;

void drawGraphics()
{
	// add your drawing code here
	ezGfxClear();

	g_pController->Render();
}

void processKeyboard(int key, int state)
{
	if (state == EZGFX_RELEASED)
		return;

	switch (key)
	{
	case 'f':
	case 'F':
		g_pController->FastRenderToggle();
		break;
	case 'r':
	case 'R':
	{
		if (g_pController)
		{
			delete g_pController;
		}

		//setup the new controller
		g_pController = new CController;
		break;
	}
	}
}

void idle()
{
	static float totalElapsed = 0;
	totalElapsed += ezGfxGetElapsed();

	if (totalElapsed >= 1.f / CParams::iFramesPerSecond || g_pController->FastRender())
	{
		totalElapsed = 0;

		if (g_pController->Update())
		{
			ezGfxUpdate();
		}
		else
		{
			//we have a problem, we should quit here
		}
	}
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, idle, processKeyboard, nullptr);
	ezGfxInitWindowParameters(CParams::WindowWidth, CParams::WindowHeight, "Smart sweepers AI demo");

	// create the controller
	g_pController = new CController;

	// this loop only returns when you exit the app
	ezGfxMainLoop();

	delete g_pController;

	return 0;
}
