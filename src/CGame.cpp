#include <cstdio>
#include <iostream>
#include <thread>

#include "utility/CWorldTime.h"
#include "utility/Util.h"

#include "messages/cl_sv_messages/ClientCmd.pb.h"

#include "CGame.h"

#undef GetCurrentTime

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

	//TODO: should be somewhere else
	m_pServerTable = m_Server.GetNetStringTableManager().CreateTable( "table" );

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

	size_t uiStringOffset = 0;

	while( !m_bTerminate )
	{
		WorldTime.TimeChanged( GetCurrentTime() );

		m_Server.RunFrame();
		m_Client.RunFrame();

		if( !m_pClientTable && m_Client.IsFullyConnected() )
		{
			//TODO: needs a callback instead
			m_pClientTable = m_Client.GetNetStringTableManager().CreateTable( "table" );

			if( m_pClientTable )
			{
				printf( "Created client copy of the table\n" );
			}
			else
			{
				printf( "Couldn't create client copy of the table\n" );
			}
		}

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

					m_pClientTable = nullptr;

					uiStringOffset = 0;
				}
				else if( m_szInput == "finish" && !m_Client.IsConnected() )
				{
					printf( "Finished\n" );

					m_bTerminate = true;
				}
				else if( m_szInput == "addstring" )
				{
					char szBuffer[ 256 ];

					for( size_t uiIndex = 0; uiIndex < 100; ++uiIndex )
					{
						snprintf( szBuffer, sizeof( szBuffer ), "foo %u", uiStringOffset++ );

						m_pServerTable->Add( szBuffer );
					}
				}
				else if( m_szInput == "liststrings" )
				{
					printf( "Number of strings in table %s: %u\n", m_pClientTable->GetName(), m_pClientTable->GetStringCount() );

					for( size_t uiIndex = 0; uiIndex < m_pClientTable->GetStringCount(); ++uiIndex )
					{
						printf( "String %u: %s\n", uiIndex, m_pClientTable->GetString( uiIndex ) );
					}
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