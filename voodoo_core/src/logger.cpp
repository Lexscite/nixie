#include "voodoo/logger.h"

namespace voodoo
{
	Logger::Logger()
	{
		fs_.open("log.txt");

#ifdef VOODOO_LOG_CONSOLE_ENABLED
		if (AllocConsole()) {
			freopen("CONOUT$", "w", stdout);
			SetConsoleTitle(L"voodoo | Debug Console");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}

		cout_buffer_ = std::cout.rdbuf(&console_buffer_);
		cerr_buffer_ = std::cerr.rdbuf(&console_buffer_);
#endif
	}

	Logger::~Logger()
	{
		fs_.close();

#ifdef VOODOO_LOG_CONSOLE_ENABLED
		std::cout.rdbuf(cout_buffer_);
		std::cout.rdbuf(cerr_buffer_);
#endif
	}

	Logger& Logger::Get()
	{
		static Logger instance;
		return instance;
	}


	void Logger::Write(std::string s)
	{
		Get().fs_ << s << std::endl;

#ifdef VOODOO_LOG_CONSOLE_ENABLED
		WriteToConsole(s);
#endif
	}

#ifdef VOODOO_LOG_CONSOLE_ENABLED
	void Logger::WriteToConsole(std::string s)
	{
		std::cout << s << std::endl;
	}
#endif
}