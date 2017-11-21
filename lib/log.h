#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <stdio.h>

/*
    To set a file to log to:
    FILE* logFile = fopen("<filename>", "a");
    Output2FILE::Stream() = logFile;
*/

inline std::string NowTime();

enum TLogLevel {ERROR, WARNING, INFO, DEBUG};

template <typename T>
class Log
{
public:
    Log();
    virtual ~Log();
    std::ostringstream& Get(TLogLevel level = INFO);
public:
    static std::string ToString(TLogLevel level);
protected:
    std::ostringstream os;
private:
    Log(const Log&);
    Log& operator=(const Log&);
};

template <typename T>
Log<T>::Log(){}

template <typename T>
std::ostringstream& Log<T>::Get(TLogLevel level)
{
    os << ToString(level) << ":";
    os << std::string(level == INFO ? " " : "");
    os << std::string(level != WARNING ? "   " : " ");
    os << NowTime() << " ";
    return os;
}

template <typename T>
Log<T>::~Log()
{
    os << std::endl;
    T::Output(os.str());
}

template <typename T>
std::string Log<T>::ToString(TLogLevel level)
{
	static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    return buffer[level];
}

class Output2FILE
{
public:
    static FILE*& Stream();
    static void Output(const std::string& msg);
};

inline FILE*& Output2FILE::Stream()
{
    static FILE* pStream = stderr;
    return pStream;
}

inline void Output2FILE::Output(const std::string& msg)
{
    FILE* pStream = Stream();
    if (!pStream)
        return;
    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}

#define FILELOG_DECLSPEC

class FILELOG_DECLSPEC FILELog : public Log<Output2FILE> {};
//typedef Log<Output2FILE> FILELog;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL DEBUG
#endif

#define LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (!Output2FILE::Stream()) ; \
    else FILELog().Get(level)

#include <sys/time.h>

inline std::string NowTime()
{
    char buffer[11];
    time_t t;
    time(&t);
    tm r = {0};
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct tm *tms = localtime(&t);
    char result[100] = {0};
    std::sprintf(result, "%d/%d/%d %s", tms->tm_year+1900, tms->tm_mon+1, tms->tm_mday, buffer);
    return result;
}

#endif //__LOG_H__
