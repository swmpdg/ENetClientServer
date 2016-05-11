#include <cstdio>
#include <iostream>
#include <thread>

#include "messages/cl_sv_messages/ClientCmd.pb.h"

#include "CGame.h"

bool CGame::Initialize( const enet_uint16 uiPort )
{
	m_uiPort = uiPort;

	printf( "Starting server\n" );

	//Initialize server with support for 1 concurrent client.
	if( !m_Server.Initialize( 1, uiPort ) )
	{
		printf( "Failed to start server\n" );
		return false;
	}

	printf( "Starting client\n" );

	if( !m_Client.Initialize() )
	{
		printf( "Failed to start client\n" );
		return false;
	}

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
	printf( "Running\n" );

	std::thread ioThread( IOThread, this );

	while( !m_bTerminate )
	{
		m_Server.RunFrame();
		m_Client.RunFrame();

		if( m_IOMutex.try_lock() )
		{
			if( m_bInputPending )
			{
				m_bInputPending = false;

				if( m_szInput == "connect" && !m_Client.IsConnected() )
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
				else if( m_szInput.find( "sendmessage" ) == 0 && m_Client.IsConnected() )
				{
					cl_sv_messages::ClientCmd cmd;

					const size_t uiStart = strlen( "sendmessage" ) + 1;

					cmd.set_command( ( m_szInput.length() > uiStart ? m_szInput.substr( uiStart ) : std::string() ) + "\n" );

					if( m_Client.SendMessage( CLSVMessage::CLIENTCMD, cmd ) )
					{
						printf( "client: Sent message\n" );
					}
					else
					{
						printf( "client: Failed to send message\n" );
					}
				}
				else if( m_szInput == "disconnect" && m_Client.IsConnected() )
				{
					m_Client.DisconnectFromServer();
				}
				else if( m_szInput == "finish" && !m_Client.IsConnected() )
				{
					printf( "Finished\n" );

					m_bTerminate = true;
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
				m_bInputPending = true;

				std::getline( std::cin, m_szInput );
			}

			m_IOMutex.unlock();
		}
	}
}