#pragma once
#include "stdio.h"
#include "mutex"
#include "map"
#include "ctime"
enum LogPriority
{
    TracePriority,
    DebugPriority,
    InfoPriority,
    WarnPriority,
    ErrorPriority,
    CriticalPriority
};
class Logger
{
private:
    static LogPriority priority;
    static std::mutex log_mutex;
    static const std::map<LogPriority, const char *> log_priority_map;
    static FILE *file;
    static const char *path_file;
    static const char *default_path;

public:
    static void EnableFileOutput()
    {
        path_file = Logger::default_path;
        enable_file_output();
    }

    static void EnableFileOutput(const char *path)
    {
        path_file = path;
        enable_file_output();
    }
    static void CloseFileOutput()
    {
        free_file();
    }

    static void SetPriority(LogPriority new_priority)
    {
        priority = new_priority;
    }
    template <typename... Args>
    static void Trace(const char *msg, Args... args)
    {
        log(LogPriority::TracePriority, msg, args...);
    }
    template <typename... Args>
    static void Debug(const char *msg, Args... args)
    {
        log(LogPriority::DebugPriority, msg, args...);
    }
    template <typename... Args>
    static void Info(const char *msg, Args... args)
    {
        log(LogPriority::InfoPriority, msg, args...);
    }
    template <typename... Args>
    static void Warn(const char *msg, Args... args)
    {
        log(LogPriority::WarnPriority, msg, args...);
    }
    template <typename... Args>
    static void Error(const char *msg, Args... args)
    {
        log(LogPriority::ErrorPriority, msg, args...);
    }
    template <typename... Args>
    static void Critical(const char *msg, Args... args)
    {
        log(LogPriority::CriticalPriority, msg, args...);
    }

private:
    template <typename... Args>
    static void log(LogPriority curr_priority, const char *msg, Args... args)
    {
        if (priority <= curr_priority)
        {
            std::time_t curr_time = std::time(0);
            std::tm *timestamp = std::localtime(&curr_time);
            char buff[80];
            strftime(buff, 80, "%D %X", timestamp);

            std::scoped_lock lock(log_mutex);
            const char *priority_str = log_priority_map.find(curr_priority)->second;
            printf("%s\t", buff);
            printf("|-<%s>-|\t", priority_str);
            printf(msg, args...);
            printf("\n");
            if (file)
            {
                fprintf(file, "%s\t", buff);
                fprintf(file, "|-<%s>-|\t", priority_str);
                fprintf(file, msg, args...);
                fprintf(file, "\n");
            }
        }
    }
    static void enable_file_output()
    {
        if (file != 0)
        {
            free_file();
        }
        file = fopen(path_file, "a");
        if (file == 0)
        {
            printf("Logger: Cannot open the output file");
        }
    }
    static void free_file()
    {
        fclose(file);
        file = 0;
    }
};

LogPriority Logger::priority = LogPriority::TracePriority;
std::mutex Logger::log_mutex;
const std::map<LogPriority, const char *> Logger::log_priority_map = {
    {TracePriority, "Trace"},
    {DebugPriority, "Debug"},
    {InfoPriority, "Info"},
    {WarnPriority, "Warn"},
    {ErrorPriority, "Error"},
    {CriticalPriority, "!Critical!"}};
FILE *Logger::file = 0;
const char *Logger::default_path = "log.txt";
const char *Logger::path_file = Logger::default_path;