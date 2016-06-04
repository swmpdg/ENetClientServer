#include <cstdio>

#include <enet/enet.h>

#include <google/protobuf/stubs/common.h>

#include "networking/CNetworkBuffer.h"

#include "engine/CEngine.h"

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
			CEngine engine;

			//Initialize the engine and run the game loop.
			//Use port 1234 for the server port.
			if( engine.Initialize( 1234 ) )
			{
				bSuccess = engine.Run();
			}

			//Shut down the engine.
			engine.Shutdown();
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