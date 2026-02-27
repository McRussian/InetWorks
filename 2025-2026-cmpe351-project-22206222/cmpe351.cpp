/* This is code developed by <Student Name and Surname> */
#include "cmpe351.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <map>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <climits>


// Функция для чтения входных данных
std::map<int, std::queue<ProcessInfo>> readInputFile(const std::string& filename) {
    std::map<int, std::queue<ProcessInfo>> queueMap;
    std::ifstream inputFile(filename);
    
    if (!inputFile.is_open()) {
        std::cerr << "Error: Cannot open input file: " << filename << std::endl;
        exit(1);
    }
    
    std::string line;
    int lineNumber = 1;
    
    while (std::getline(inputFile, line)) {
        // Пропускаем пустые строки
        if (line.empty()) {
            lineNumber++;
            continue;
        }
        
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        // Разделяем строку по символу ':'
        while (std::getline(ss, token, ':')) {
            tokens.push_back(token);
        }
        
        // Проверяем, что в строке ровно 4 значения
        if (tokens.size() != 4) {
            std::cerr << "Error: Invalid format at line " << lineNumber 
                      << ". Expected 4 values separated by ':', got " 
                      << tokens.size() << std::endl;
            exit(1);
        }
        
        try {
            // Создаем структуру ProcessInfo из первых трех значений
            ProcessInfo process;
            process.BurstTime = std::stoi(tokens[0]);
            process.Priority = std::stoi(tokens[1]);
            process.ArrivalTime = std::stoi(tokens[2]);
            
            // Получаем ID очереди из четвертого значения
            int queueID = std::stoi(tokens[3]);
            
            // Добавляем процесс в соответствующую очередь
            queueMap[queueID].push(process);
            
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid number format at line " << lineNumber << std::endl;
            exit(1);
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: Number out of range at line " << lineNumber << std::endl;
            exit(1);
        }
        
        lineNumber++;
    }
    
    inputFile.close();
    return queueMap;
}

// Вспомогательная функция для подсчета процессов в очереди
int countProcessesInQueue(const std::queue<ProcessInfo>& q) {
    return q.size();
}

// Вспомогательная функция для отладки - вывод информации про очереди
void showProcessQueue(std::map<int, std::queue<ProcessInfo>>& data) {
    // Выводим информацию о прочитанных данных (для отладки)
    std::cout << "Number of different queues: " << data.size() << std::endl;
    
    for (const auto& pair : data) {
        std::cout << "Queue ID " << pair.first << " has " 
                  << countProcessesInQueue(pair.second) << " process(es)" << std::endl;
        
        // Для демонстрации, можно также показать содержимое очереди
        std::cout << "  Processes in queue " << pair.first << ":" << std::endl;
        
        // Создаем копию очереди для обхода без изменения оригинала
        std::queue<ProcessInfo> tempQueue = pair.second;
        int processNum = 1;
        
        while (!tempQueue.empty()) {
            ProcessInfo p = tempQueue.front();
            tempQueue.pop();
            std::cout << "    Process " << processNum++ << ": "
                      << "Burst=" << p.BurstTime 
                      << ", Priority=" << p.Priority
                      << ", Arrival=" << p.ArrivalTime << std::endl;
        }
    }
}

// Функция для преобразования QueueScheduleInfo в строку
std::string queueScheduleInfoToString(const QueueScheduleInfo& info) {
    std::stringstream ss;
    
    // Добавляем времена ожидания из списка
    bool first = true;
    for (int waiting_time : info.waiting_times) {
        if (!first) {
            ss << ":";
        }
        ss << waiting_time;
        first = false;
    }
    
    // Добавляем среднее время ожидания
    ss << ":" << std::fixed << std::setprecision(2) << info.average_waiting_time;
    
    return ss.str();
}

// Вспомогательная функция для сравнения процессов по времени прибытия (FCFS)
bool compareByArrivalTime(const ProcessInfo& a, const ProcessInfo& b) {
    return a.ArrivalTime < b.ArrivalTime;
}

// Функция для вычисления FCFS (First Come First Serve)
QueueScheduleInfo FCFS(std::queue<ProcessInfo> processes) {
    QueueScheduleInfo result;
    
    if (processes.empty()) {
        result.average_waiting_time = 0.0;
        return result;
    }
    
    // Копируем процессы в список для сортировки
    std::list<ProcessInfo> processList;
    while (!processes.empty()) {
        processList.push_back(processes.front());
        processes.pop();
    }
    
    // Сортируем процессы по времени прибытия (ArrivalTime)
    processList.sort(compareByArrivalTime);
    
    int currentTime = 0;
    int totalWaitingTime = 0;
    int processCount = 0;
    
    // Обрабатываем процессы в порядке прибытия
    for (const ProcessInfo& process : processList) {
        processCount++;
        
        // Если текущее время меньше времени прибытия процесса
        if (currentTime < process.ArrivalTime) {
            currentTime = process.ArrivalTime;
        }
        
        // Время ожидания = текущее время - время прибытия
        int waitingTime = currentTime - process.ArrivalTime;
        
        // Сохраняем время ожидания в список
        result.waiting_times.push_back(waitingTime);
        totalWaitingTime += waitingTime;
        
        // Обновляем текущее время (добавляем время выполнения процесса)
        currentTime += process.BurstTime;
    }
    
    // Вычисляем среднее время ожидания
    if (processCount > 0) {
        result.average_waiting_time = static_cast<double>(totalWaitingTime) / processCount;
    } else {
        result.average_waiting_time = 0.0;
    }
    
    return result;
}


// Функция для вычисления SJF (Shortest Job First, non-preemptive)
QueueScheduleInfo SJF(std::queue<ProcessInfo> processes) {
    QueueScheduleInfo result;
    
    if (processes.empty()) {
        result.average_waiting_time = 0.0;
        return result;
    }
    
    // Копируем в вектор для простоты сортировки (если разрешено)
    std::vector<ProcessInfo> processVec;
    while (!processes.empty()) {
        processVec.push_back(processes.front());
        processes.pop();
    }
    
    // Сортируем по времени прибытия
    std::sort(processVec.begin(), processVec.end(), 
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.ArrivalTime < b.ArrivalTime;
              });
    
    int n = processVec.size();
    std::vector<int> completionTime(n, 0);
    std::vector<bool> completed(n, false);
    int currentTime = 0;
    int completedCount = 0;
    
    while (completedCount < n) {
        int idx = -1;
        int minBurst = INT_MAX;
        
        // Находим процесс с наименьшим временем выполнения среди прибывших
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processVec[i].ArrivalTime <= currentTime) {
                if (processVec[i].BurstTime < minBurst) {
                    minBurst = processVec[i].BurstTime;
                    idx = i;
                }
            }
        }
        
        // Если не нашли, переходим к следующему времени прибытия
        if (idx == -1) {
            int minArrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!completed[i] && processVec[i].ArrivalTime < minArrival) {
                    minArrival = processVec[i].ArrivalTime;
                }
            }
            currentTime = minArrival;
            continue;
        }
        
        // Выполняем найденный процесс
        completionTime[idx] = currentTime + processVec[idx].BurstTime;
        completed[idx] = true;
        currentTime = completionTime[idx];
        completedCount++;
    }
    
    // Вычисляем времена ожидания
    int totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        int waiting = completionTime[i] - processVec[i].ArrivalTime - processVec[i].BurstTime;
        result.waiting_times.push_back(waiting);
        totalWaiting += waiting;
    }
    
    result.average_waiting_time = static_cast<double>(totalWaiting) / n;
    return result;
}

// Функция для вычисления Priority Scheduling (non-preemptive)
QueueScheduleInfo Priority(std::queue<ProcessInfo> processes) {
    QueueScheduleInfo result;
    
    if (processes.empty()) {
        result.average_waiting_time = 0.0;
        return result;
    }
    
    // Копируем процессы в вектор для простоты
    std::vector<ProcessInfo> procVec;
    while (!processes.empty()) {
        procVec.push_back(processes.front());
        processes.pop();
    }
    
    int n = procVec.size();
    std::vector<int> startTime(n, 0);
    std::vector<bool> completed(n, false);
    int currentTime = 0;
    int finished = 0;
    
    // Храним индексы процессов в исходном порядке
    std::vector<int> originalIndices;
    for (int i = 0; i < n; i++) {
        originalIndices.push_back(i);
    }
    
    while (finished < n) {
        int selectedIdx = -1;
        int minPriority = INT_MAX; // Ищем минимальное значение Priority
        
        // Ищем процесс с наивысшим приоритетом (минимальным числом) среди прибывших
        for (int i = 0; i < n; i++) {
            if (!completed[i] && procVec[i].ArrivalTime <= currentTime) {
                if (procVec[i].Priority < minPriority) {
                    minPriority = procVec[i].Priority;
                    selectedIdx = i;
                }
                // Если приоритеты равны, выбираем раньше прибывший
                else if (procVec[i].Priority == minPriority && selectedIdx != -1) {
                    if (procVec[i].ArrivalTime < procVec[selectedIdx].ArrivalTime) {
                        selectedIdx = i;
                    }
                }
            }
        }
        
        // Если не нашли готовых процессов
        if (selectedIdx == -1) {
            // Находим минимальное время прибытия среди оставшихся
            int minArrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!completed[i] && procVec[i].ArrivalTime < minArrival) {
                    minArrival = procVec[i].ArrivalTime;
                }
            }
            currentTime = minArrival;
            continue;
        }
        
        // Сохраняем время начала выполнения
        startTime[selectedIdx] = currentTime;
        
        // Выполняем процесс
        currentTime += procVec[selectedIdx].BurstTime;
        completed[selectedIdx] = true;
        finished++;
    }
    
    // Вычисляем времена ожидания в исходном порядке
    int totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        int waiting = startTime[i] - procVec[i].ArrivalTime;
        result.waiting_times.push_back(waiting);
        totalWaiting += waiting;
    }
    
    result.average_waiting_time = static_cast<double>(totalWaiting) / n;
    return result;
}

// Функция для записи результатов в выходной файл
void writeOutputFile(const std::map<int, std::queue<ProcessInfo>>& queueMap, 
                     const std::string& outputFilename) {
    
    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Cannot open output file: " << outputFilename << std::endl;
        exit(1);
    }
    
    // Перебираем все очереди
    for (const auto& queuePair : queueMap) {
        int queueID = queuePair.first;
        std::queue<ProcessInfo> processes = queuePair.second;
        
        // Создаем копию очереди для алгоритма FCFS
        std::queue<ProcessInfo> processesCopy = processes;
        
        // Выполняем FCFS (Algorithm 1)
        QueueScheduleInfo fcfsResult = FCFS(processesCopy);
        
        // Формируем и записываем строку результата
        std::string resultStr = std::to_string(queueID) + ":1:" + 
                                queueScheduleInfoToString(fcfsResult);
        outputFile << resultStr << std::endl;
        std::cout << resultStr << std::endl;
                
        // Для SJF (Algorithm 2)
        processesCopy = processes; // создаем новую копию
        QueueScheduleInfo sjfResult = SJF(processesCopy);
        resultStr = std::to_string(queueID) + ":2:" + 
                   queueScheduleInfoToString(sjfResult);
        outputFile << resultStr << std::endl;
        std::cout << resultStr << std::endl;

        // Для Priority (Algorithm 3)
        processesCopy = processes; // создаем новую копию
        QueueScheduleInfo priorityResult = Priority(processesCopy);
        resultStr = std::to_string(queueID) + ":3:" + 
                   queueScheduleInfoToString(priorityResult);
        outputFile << resultStr << std::endl;
        std::cout << resultStr << std::endl;
    }
    
    outputFile.close();
}

// Главная функция программы
int main(int argc, char* argv[]) {
    // Проверяем количество аргументов командной строки
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " input_file.txt output_file.txt" << std::endl;
        return 1;
    }
    
    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];
    
    // Читаем входные данные и создаем map с очередями процессов
    std::map<int, std::queue<ProcessInfo>> data = readInputFile(inputFilename);

    writeOutputFile(data, outputFilename);
    
    return 0;
}
