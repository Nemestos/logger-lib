#include "logger.h"
#include "thread"
void log_test(int nbr)
{
    Logger::Info("%d", nbr);
}

int main()
{
    Logger::EnableFileOutput();
    std::thread threads[10];
    for (size_t i = 0; i < 10; i++)
    {
        threads[i] = std::thread(log_test, i);
    }
    for (size_t i = 0; i < 10; i++)
    {
        threads[i].join();
    }
    Logger::CloseFileOutput();
    return 1;
}