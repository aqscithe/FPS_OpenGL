#include "Debug.h"

#include <date/date.h>

using namespace Core::Debug;

Log* Log::instance = 0;

Log* Log::getInstance()
{
	if (instance == 0)
	{

		instance = new Log();
	}

	return instance;
}

void Log::redirectLogs(const LogOptions& type)
{
	switch (type)
	{
	case LogOptions::MESSAGE:
		std::cout.rdbuf(outMessage.rdbuf());
		break;
	case LogOptions::ERROR:
		std::cerr.rdbuf(outError.rdbuf());
		break;
	case LogOptions::HYBRID:
		std::cout.rdbuf(outMessage.rdbuf());
		std::cerr.rdbuf(outError.rdbuf());
		break;
	default:
		LogType logError = LogType::ERROR;
		std::string statement = "Invalid output stream code: " + static_cast<int>(type);
		Log::print(statement, logError);
		break;
	}
}

void Log::endRedirect(const LogOptions& type)
{
	switch (type)
	{
	case LogOptions::MESSAGE:
		std::cout.rdbuf(coutbuf);
		break;
	case LogOptions::ERROR:
		std::cout.rdbuf(cerrbuf);
		break;
	case LogOptions::HYBRID:
		std::cout.rdbuf(outMessage.rdbuf());
		std::cerr.rdbuf(outError.rdbuf());
		break;
	default:
		LogType logError = LogType::ERROR;
		std::string statement = "Invalid output stream code: " + static_cast<int>(type);
		Log::print(statement, logError);
		break;
	}
}

void    Log::configureLogFiles()
{
	auto msgFile = LogFile::MESSAGE;
	auto errFile = LogFile::ERROR;
	auto hybridOption = LogOptions::HYBRID;

	redirectLogs(hybridOption);
	logHeader(msgFile);
	logHeader(errFile);
	endRedirect(hybridOption);
}

void Log::logHeader(const LogFile& logFile)
{
	logDivider(logFile);
	printTitle(logFile);
	printDateTime(logFile);
}

void Log::printTitle(const LogFile& logFile)
{
	switch (logFile)
	{
	case LogFile::MESSAGE:
		std::cout << "MESSAGE LOGS..." << std::endl;
		break;
	case LogFile::ERROR:
		std::cerr << "ERROR LOGs..." << std::endl;
		break;
	default:
		LogType logError = LogType::ERROR;
		std::string statement = "Log::printTitle() - Not a valid log file code: " + static_cast<int>(logFile);
		print(statement, logError);
		break;
	}
}

void Log::printDateTime(const LogFile& logFile)
{
	auto tp = std::chrono::system_clock::now();
	auto dp = date::floor<date::days>(tp);
	auto ymd = date::year_month_day{ dp };
	auto time = date::make_time(std::chrono::duration_cast<std::chrono::milliseconds>(tp - dp));

	switch (logFile)
	{
	case LogFile::MESSAGE:
		std::cout << ymd.year() << "-" << ymd.month() << "-" << ymd.day() << " " << time.hours().count()
			<< ":" << time.minutes().count() << ":" << time.seconds().count() << std::endl;
		std::cout << std::endl;
		break;
	case LogFile::ERROR:
		std::cerr << ymd.year() << "-" << ymd.month() << "-" << ymd.day() << " " << time.hours().count()
			<< ":" << time.minutes().count() << ":" << time.seconds().count() << std::endl;
		std::cerr << std::endl;
		break;
	default:
		LogType logError = LogType::ERROR;
		std::string statement = "Log::printDateTime() - Not a valid log file code: " + static_cast<int>(logFile);
		print(statement, logError);
		break;
	}
}

void Log::logDivider(const LogFile& logFile)
{
	switch (logFile)
	{
	case LogFile::MESSAGE:
		std::cout << "==========================================================================" << std::endl;
		std::cout << "==========================================================================" << std::endl;
		break;
	case LogFile::ERROR:
		std::cerr << "==========================================================================" << std::endl;
		std::cerr << "==========================================================================" << std::endl;
		break;
	default:
		LogType logError = LogType::ERROR;
		std::string statement = "Log::logDivider() - Not a valid log file code: " + static_cast<int>(logFile);
		print(statement, logError);
		break;
	}

}

void	Log::print(const std::string& statement, const LogType& logType)
{
	auto msgOption = LogOptions::MESSAGE;
	auto errOption = LogOptions::ERROR;
	auto hybridOption = LogOptions::HYBRID;
	if (enabled)
	{
		switch (logType)
		{
		case LogType::INFO:
			Log::redirectLogs(msgOption);
			std::cout << "INFO LOG: " << statement << std::endl;
			Log::endRedirect(msgOption);
			std::cout << "INFO LOG: " << statement << std::endl;
			break;
		case LogType::ERROR:
			Log::redirectLogs(errOption);
			std::cerr << "ERROR LOG: " << statement << std::endl;
			Log::endRedirect(errOption);
			std::cout << "ERROR LOG: " << statement << std::endl;
			exit(1);
			break;
		case LogType::WARNING:
			Log::redirectLogs(errOption);
			std::cerr << "WARNING LOG: " << statement << std::endl;
			Log::endRedirect(errOption);
			std::cerr << "WARNING LOG: " << statement << std::endl;
			break;
		case LogType::FATAL:
			Log::redirectLogs(errOption);
			std::cerr << "FATAL LOG: " << statement << std::endl;
			Log::endRedirect(errOption);
			std::cerr << "FATAL LOG: " << statement << std::endl;
			exit(1);
			break;
		case LogType::DEBUG:
			Log::redirectLogs(msgOption);
			std::cout << "DEBUG LOG: " << statement << std::endl;
			Log::endRedirect(msgOption);
			std::cout << "DEBUG LOG: " << statement << std::endl;
			break;
		default:
			break;
		}
	}

}