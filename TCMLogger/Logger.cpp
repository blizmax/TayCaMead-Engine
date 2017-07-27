#include "TCMLogger/Logger.h"

#include <memory>
#include <mutex>

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <Windows.h>
#include <Shlwapi.h>
#include <direct.h>

#pragma comment(lib, "Shlwapi.lib")

#include "TCMMemory/TCMEventManager.hpp"

namespace TCM
{
	namespace Debug
	{
#pragma region Constructor and co.
		Logger::Logger( const LogOption& logOption )
			: m_LogOption( logOption )
		{
			CoreSetLogOption( logOption );
			m_event = new Event<LogData>();
		}

		std::string LogData::BuildLogMessage() const
		{
			std::string message = "";

			switch ( m_logLevel )
			{
				case LogLevel::DEBUG:
				{
					message += "[DEBUG]";
					break;
				}

				case LogLevel::FAILURE:
				{
					message += "[FAILURE]";
					break;
				}

				case LogLevel::FATALERROR:
				{
					message += "[FATAL ERROR]";
					break;
				}

				case LogLevel::INFO:
				{
					message += "[INFO]";
					break;
				}

				case LogLevel::WARNING:
				{
					message += "[WARNING]";
					break;
				}

				default:
				{
					message += "[???]";
					break;
				}
			}

			std::string func( m_function.begin(), m_function.end() );
			std::string file = m_file.substr( m_file.find_last_of( '\\' ) + 1, m_file.npos );

			message += "\t" + m_message + "\n";
			message += "\t(" + file + ":" + std::to_string( m_line ) + " - [" + func + "()]) \n\n";

			return message;
		}

		Logger::~Logger()
		{
			m_fileStream.close();
			delete m_event;
		}

		Logger& Logger::GetInstance()
		{
			static std::unique_ptr<Logger> m_instance;
			static std::once_flag m_onceFlag;

			std::call_once( m_onceFlag,
			                []()
			                {
				                m_instance.reset( new Logger() );
			                }
			);
			return *( m_instance.get() );
		}

		void Logger::CoreSetLogOption( const LogOption& option )
		{
			m_LogOption = option;

			if ( static_cast<uint8_t>(m_LogOption) & static_cast<uint8_t>(LogOption::FILE) )
			{
				time_t current_time = time( nullptr );
				struct tm* now = new tm;
				localtime_s( now, &current_time );

				std::stringstream fileName;
				fileName << "Log-";
				fileName << std::to_string( now->tm_year + 1900 ); // tm_year returns current year - 1900
				fileName << "-";
				fileName << std::setw( 2 ) << std::setfill( '0' ) << std::to_string( now->tm_mon + 1 ); // month = [0->11]
				fileName << "-";
				fileName << std::setw( 2 ) << std::setfill( '0' ) << std::to_string( now->tm_mday );
				fileName << "-";
				fileName << std::setw( 2 ) << std::setfill( '0' ) << std::to_string( now->tm_hour );
				fileName << "h";
				fileName << std::setw( 2 ) << std::setfill( '0' ) << std::to_string( now->tm_min );
				fileName << "m";
				fileName << std::setw( 2 ) << std::setfill( '0' ) << std::to_string( now->tm_sec );
				fileName << "s.log";
				delete now;

				TCHAR path[MAX_PATH];

				if ( GetModuleFileName( nullptr, path, MAX_PATH ) == ERROR_INSUFFICIENT_BUFFER ) // Create directory for logs, if already exist, skip,
					std::cerr << "Cannot retrieve exe directory path because it exceed 260 characters...\nPlease move it close to your hard drive directory." << std::endl;

				PathRemoveFileSpec( path );
				std::wstring wpath( path );

				wpath.append( L"\\logs\\" );
				_wmkdir( wpath.data() );
				std::string fullPath( wpath.begin(), wpath.end() );
				fullPath += fileName.str();

				m_fileStream.open( fullPath );

				if ( !m_fileStream.is_open() )
				{
					m_LogOption = LogOption::CONSOLE;
					std::cerr << "File cannot be opened, switching to Console only." << std::endl;
				}
				if ( !m_fileStream.good() )
				{
					m_LogOption = LogOption::CONSOLE;
					std::cerr << "File stream to log file is broken ! Cannot write to file, switching to Console only." << std::endl;
				}
			}
		}
#pragma endregion

#pragma region Static public methods
		void Logger::Log( const LogData& logData )
		{
			GetInstance().CoreLog( logData );
		}

		LogDataList& Logger::GetAllLogs()
		{
			return GetInstance().m_logDataList;
		}

		LogDataList Logger::GetAllLogs( const LogLevel& level )
		{
			LogDataList allData = GetInstance().m_logDataList;
			LogDataList list;
			LogDataList::iterator it = allData.begin();
			LogDataList::iterator end = allData.end();

			for ( ; it != end; ++it )
				if ( ( *it ).m_logLevel == level )
					list.push_back( *it );

			return list;
		}

		void Logger::SetLogOption( const LogOption& option )
		{
			GetInstance().CoreSetLogOption( option );
		}

		Event<LogData>* Logger::GetEvent()
		{
			return GetInstance().CoreGetEvent();
		}
#pragma endregion

#pragma region Private methods
		void Logger::CoreLog( const LogData& logData )
		{
			m_logDataList.push_back( logData );

			std::string message = logData.BuildLogMessage();
			if ( static_cast<uint8_t>(m_LogOption) & static_cast<uint8_t>(LogOption::CONSOLE) )
				std::cout << message;
			if ( static_cast<uint8_t>(m_LogOption) & static_cast<uint8_t>(LogOption::FILE) )
				m_fileStream << message;

			m_event->Invoke( logData );

			if ( logData.m_logLevel == LogLevel::FATALERROR )
				exit( -1 );
		}

		Event<LogData>* Logger::CoreGetEvent() const
		{
			return m_event;
		}
#pragma endregion
	}
}
