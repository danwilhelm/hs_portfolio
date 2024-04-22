//		 //¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\
//		||  Razor 3D FPS Engine  ||
//		||  By: Dayle Flowers    ||
//      ||   and Daniel Wilhelm  ||
//		||  (c)Xiero Games       ||
//		||  www.xierogames.com   ||
//		||-----------------------||
//		||  Created: 7.28.01     ||
//		||  Using DirectX 8.0a   ||
//		||  and Visual C++ 6     ||
//		||  Both(c)Microsoft     ||
//		 \\_____________________//

//NOTE: I took half this code from my DirectDraw Engine,
//       so it may not ALL be needed, check it out for urself tho

//Include Main Header
#include "Main.h"


//------------------------------------------------------------------------------------------\\
// MainLoop();
// This draws the screen and keeps up with the FPS
// This should be run from the WinMain function in the MessageLoop
BOOL MainLoop()
{
	// Clear the back buffer
	//D3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1, 0 );

	//Read the Keyboard Input
	RazorKeys.KeyboardRead();

   	// Begin the scene
    D3DDevice->BeginScene();

	// Set-up a list of triangle vertices
	VERTEX data[3];
	for(int i=0;i<3;i++)
	{
		data[i].x		= (FLOAT)(rand()%ScreenWidth);
		data[i].y		= (FLOAT)(rand()%ScreenHeight);
		data[i].z		= 1.0f;
		data[i].rhw		= 1.0f;
		data[i].color	= D3DCOLOR_XRGB(rand()%256,rand()%256,rand()%256);
	}
	
	// Set-up the shader (diffuse) and then draw the triangles
	D3DDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &data, sizeof(VERTEX));
    	
	// End the scene
	D3DDevice->EndScene();

    // Flip the back buffer to the display
    D3DDevice->Present( NULL, NULL, NULL, NULL );

	return true;
}
//------------------------------------------------------------------------------------------//


