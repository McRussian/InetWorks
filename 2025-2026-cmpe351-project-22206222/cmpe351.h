#include <unistd.h>
#include <string>
#include <queue>
#include <map>
#include <list>

#if !defined(_POSIX_VERSION)
    #error "Non-POSIX compliant OS environment detected. Please compile your code in Linux,MacOS or BSD enviroment"
#endif

// Структура для хранения информации о процессе
struct ProcessInfo {
    int BurstTime;
    int Priority;
    int ArrivalTime;
    int ProcessID; // Добавляем ID процесса для удобства
};

// Структура для хранения результатов планирования очереди
struct QueueScheduleInfo {
    std::list<int> waiting_times; // Времена ожидания для каждого процесса
    double average_waiting_time;    // Среднее время ожидания
};

std::map<int, std::queue<ProcessInfo>> readInputFile(const std::string& filename);

void showProcessQueue(std::map<int, std::queue<ProcessInfo>>& data);

QueueScheduleInfo FCFS(std::queue<ProcessInfo> processes);
QueueScheduleInfo SJF(std::queue<ProcessInfo> processes);
QueueScheduleInfo Priority(std::queue<ProcessInfo> processes);

std::string queueScheduleInfoToString(const QueueScheduleInfo& info);

void writeOutputFile(const std::map<int, std::queue<ProcessInfo>>& queueMap, const std::string& outputFilename);
