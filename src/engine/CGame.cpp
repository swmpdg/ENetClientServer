#include <cstdio>
#include <iostream>
#include <thread>

#include "utility/CWorldTime.h"
#include "utility/Util.h"

#include "messages/cl_sv_messages/ClientCmd.pb.h"

#include "lib/LibInterface.h"
#include "shared/Utility.h"
#include "utility/CCommand.h"

#include "CGame.h"

#undef GetCurrentTime

bool CGame::Initialize( const enet_uint16 uiPort )
{
	m_uiPort = uiPort;

	const CreateInterfaceFn factories[] = 
	{
		CreateInterface
	};

	printf( "Connecting server\n" );

	if( !m_Server.Connect( factories, ARRAYSIZE( factories ) ) )
	{
		return false;
	}

	printf( "Starting server\n" );

	//Initialize server with support for 1 concurrent client.
	if( !m_Server.Initialize( 1, uiPort ) )
	{
		printf( "Failed to start server\n" );
		return false;
	}

	printf( "Connecting client\n" );

	if( !m_Client.Connect( factories, ARRAYSIZE( factories ) ) )
	{
		return false;
	}

	printf( "Starting client\n" );

	if( !m_Client.Initialize() )
	{
		printf( "Failed to start client\n" );
		return false;
	}

	//TODO: use a real map name when it works.
	m_Server.ChangeLevel( "foo.bsp" );

	return true;
}

void CGame::Shutdown()
{
	if( m_Client.IsInitialized() )
	{
		printf( "Shutting down client\n" );

		m_Client.Shutdown();
	}

	if( m_Server.IsInitialized() )
	{
		printf( "Shutting down server\n" );

		m_Server.Shutdown();
	}
}

bool CGame::Run()
{
	return RunGameLoop();
}

bool CGame::RunGameLoop()
{
	printf( "Running\n" );

	std::thread ioThread( IOThread, this );

	while( !m_bTerminate )
	{
		WorldTime.TimeChanged( GetCurrentTime() );

		m_Server.RunFrame();
		m_Client.RunFrame();

		if( m_IOMutex.try_lock() )
		{
			if( m_bInputPending )
			{
				m_bInputPending = false;

				CCommand command( m_szInput.c_str() );

				if( command.IsValid() )
				{
					if( strcmp( command.Arg( 0 ), "connect" ) == 0 && !m_Client.IsConnected() )
					{
						printf( "Connecting to server..." );

						fflush( stdout );

						if( m_Client.ConnectToServer( "localhost", m_uiPort ) )
						{
							printf( "Connection succeeded\n" );
						}
						else
						{
							printf( "Failed to connect to server\n" );
							return false;
						}
					}
					else if( strcmp( command.Arg( 0 ), "sendmessage" ) == 0 && m_Client.IsConnected() )
					{
						cl_sv_messages::ClientCmd cmd;

						cmd.set_command( std::string( command.GetArgumentsString() ) + "\n" );

						if( m_Client.SendMessage( CLSVMessage::CLIENTCMD, cmd ) )
						{
							printf( "client: Sent message\n" );
						}
						else
						{
							printf( "client: Failed to send message\n" );
						}
					}
					else if( strcmp( command.Arg( 0 ), "disconnect" ) == 0 && m_Client.IsConnected() )
					{
						m_Client.DisconnectFromServer();
					}
					else if( strcmp( command.Arg( 0 ), "finish" ) == 0 && !m_Client.IsConnected() )
					{
						printf( "Finished\n" );

						m_bTerminate = true;
					}
					else
					{
						if( m_Client.IsConnected() )
						{
							if( !m_Client.ClientCommand( command ) )
							{
								if( !m_Server.ClientCommand( command ) )
								{
									printf( "Unknown command: %s\n", command.Arg( 0 ) );
								}
							}
						}
						else
						{
							//TODO: this shouldn't be here. These commands should be handled through the server console.
							if( !m_Server.ClientCommand( command ) )
							{
								printf( "Unknown command: %s\n", command.Arg( 0 ) );
							}
						}
					}
				}
				else
				{
					printf( "Invalid command \"%s\"\n", m_szInput.c_str() );
				}
			}

			m_IOMutex.unlock();
		}
	}

	ioThread.join();

	return true;
}

void CGame::IOThread( CGame* pGame )
{
	pGame->RunIO();
}

void CGame::RunIO()
{
	while( !m_bTerminate )
	{
		if( m_IOMutex.try_lock() )
		{
			if( !m_bInputPending )
			{
				std::getline( std::cin, m_szInput );

				if( !m_szInput.empty() )
					m_bInputPending = true;
			}

			m_IOMutex.unlock();
		}
	}
}