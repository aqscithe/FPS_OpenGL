#pragma once

#include <iostream>
#include <fstream>
#include <assert.h> 

#include <direct.h>

namespace Core
{
	namespace Debug
	{
		enum class LogType
		{
			DEBUG = 0,
			INFO = 1,
			WARNING = 2,
			ERROR = 3,
			FATAL = 4
		};

		enum class LogFile
		{
			MESSAGE = 0,
			ERROR = 1
		};

		enum class LogOptions
		{
			MESSAGE,
			ERROR,
			HYBRID
		};

		//Singleton
		class Log
		{
		public:
			static Log* getInstance();

			void             print(const std::string& statement, const LogType& logType);
			void             configureLogFiles();
			void             redirectLogs(const LogOptions& outputType);
			void             endRedirect(const LogOptions& outputType);

			bool             enabled = true;
			int              logDir = _mkdir(".\\Logs");

		private:
			void             printTitle(const LogFile& logfile);
			void             logHeader(const LogFile& logFile);
			void             logDivider(const LogFile& logFile);
			void             printDateTime(const LogFile& logFile);

			std::ofstream    outMessage{ "Logs/message.log", std::ofstream::app };
			std::ofstream    outError{ "Logs/error.log", std::ofstream::app };
			std::streambuf*	 coutbuf = std::cout.rdbuf(); //save old buf
			std::streambuf*	 cerrbuf = std::cerr.rdbuf();

			static Log* instance;
		};

		namespace Assertion
		{
			void assertApi(int i);

			template<class T>
			void assertShrdPtr(const std::shared_ptr<T> ptr)
			{
				assert(ptr != nullptr);
			}
		}
	}
}