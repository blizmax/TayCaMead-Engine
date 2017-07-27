#pragma once

#include "LoggerDefines.h"

#include <fstream>
#include <stdexcept>
#include <vector>

#define TCMDEBUG(message)		::TCM::Debug::Logger::Log( \
	TCM::Debug::LogData( \
	TCM::Debug::LogLevel::DEBUG \
	, __LINE__ \
	, __FUNCTIONW__ \
	, __FILE__ \
	, static_cast<std::string>(message)))

#define TCMWARNING(message)		::TCM::Debug::Logger::Log( \
	TCM::Debug::LogData( \
	TCM::Debug::LogLevel::WARNING \
	, __LINE__ \
	, __FUNCTIONW__ \
	, __FILE__ \
	, static_cast<std::string>(message)))

#define TCMFAILURE(message)		::TCM::Debug::Logger::Log( \
	TCM::Debug::LogData( \
	TCM::Debug::LogLevel::FAILURE \
	, __LINE__ \
	, __FUNCTIONW__ \
	, __FILE__ \
	, static_cast<std::string>(message)))

#define TCMFATALERROR(message)		::TCM::Debug::Logger::Log( \
	TCM::Debug::LogData( \
	TCM::Debug::LogLevel::FATALERROR \
	, __LINE__ \
	, __FUNCTIONW__ \
	, __FILE__ \
	, static_cast<std::string>(message)))

#define TCMINFO(message)		::TCM::Debug::Logger::Log( \
	TCM::Debug::LogData( \
	TCM::Debug::LogLevel::INFO \
	, __LINE__ \
	, __FUNCTIONW__ \
	, __FILE__ \
	, static_cast<std::string>(message)))

#ifndef _DEBUG
	#undef TCMDEBUG
	#define TCMDEBUG(message) TCMINFO(message)
#endif

namespace TCM
{
	namespace Engine
	{
		template<typename... Args>
		class Event;
	}

	namespace Debug
	{
		template<typename T>
		using Event = Engine::Event<T>;

		/**
		 * \brief Warning level of log
		 */
		enum class LogLevel : uint8_t
		{
			DEBUG = 0x00,
			WARNING,
			FAILURE,
			FATALERROR,
			INFO
		};

		/**
		 * \brief Type of log
		 */
		enum class LogOption : uint8_t
		{
			NONE = 0x00,
			FILE = 0x01,
			CONSOLE = 0x02,
			BOTH = FILE | CONSOLE
		};

		struct LogData final
		{
			LogData( LogLevel level = LogLevel::DEBUG
			         , int line = 0
			         , std::wstring function = L"N/A"
			         , std::string file = "N/A"
			         , std::string message = "N/A" )
				: m_logLevel( level )
				, m_line( line )
				, m_function( function )
				, m_file( file )
				, m_message( message )
			{
			}

			LogLevel m_logLevel = LogLevel::DEBUG;
			int m_line = 0;
			std::wstring m_function = L"N/A";
			std::string m_file = "N/A";
			std::string m_message = "N/A";

			std::string BuildLogMessage() const;
		};

		typedef std::vector<LogData> LogDataList;

		/**
		 * \brief Class used for logging messages
		 */
		class Logger final
		{
		public:
			~Logger();

			/**
			 * \brief Log a message
			 * \param logData Structure containing all info for the log
			 */
			static TCMLOGGER_DLL void Log( const LogData& logData );

			static TCMLOGGER_DLL LogDataList& GetAllLogs();

			static TCMLOGGER_DLL LogDataList GetAllLogs( const LogLevel& level );

			/**
			* \brief Sets the logger output option
			* \param option The log type
			*/
			static TCMLOGGER_DLL void SetLogOption( const LogOption& option );

			static TCMLOGGER_DLL Event<LogData>* GetEvent();
		private:

#if _DEBUG
			Logger( const LogOption& logOption = LogOption::BOTH );
#else
			Logger( const LogOption& logOption = LogOption::FILE );
#endif
			/**
			 * \brief get the unique instance of the TCMLogger
			 * \return A reference to the TCMLogger
			 */
			static Logger& GetInstance();

			/**
			 * \brief Sets the logger output option
			 * \param option The log type
			 */
			void CoreSetLogOption( const LogOption& option );

			/**
			 * \brief Log a message
			 * \param logData Structure containing all info for the log
			 */
			void CoreLog( const LogData& logData );

			Event<LogData>* CoreGetEvent() const;

			/**
			 * \brief Current log options
			 */
			LogOption m_LogOption;

			/**
			 * \brief The log-file stream
			 */
			std::ofstream m_fileStream;

			LogDataList m_logDataList;

			Event<LogData>* m_event;
		};
	}
}
