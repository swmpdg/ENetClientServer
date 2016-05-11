#include <cstdio>

#include <enet/enet.h>

#include <google/protobuf/stubs/common.h>

#include "utility/CNetworkBuffer.h"

#include "CGame.h"

int main( int iArgc, char* pszArgV[] )
{
	bool bSuccess = false;

	//Start up ENet.
	const int iENetResult = enet_initialize();

	if( iENetResult == 0 )
	{
		//Setup network masks. Needed for bit aligned data.
		CNetworkBuffer::InitMasks();

		{
			CGame game;

			//Initialize the game and run the game loop.
			//Use port 1234 for the server port.
			if( game.Initialize( 1234 ) )
			{
				bSuccess = game.Run();
			}

			//Shut down the game.
			game.Shutdown();
		}

		//Release protobuf memory.
		google::protobuf::ShutdownProtobufLibrary();

		//Shut down ENet.
		enet_deinitialize();
	}
	else
	{
		printf( "ENet failed to initialize with error code '%d'\n", iENetResult );
	}

	//Wait for input.
	printf( "Press ENTER to continue..." );

	getchar();

	return bSuccess ? EXIT_SUCCESS : EXIT_FAILURE;
}