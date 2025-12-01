#include <iostream>
#include "ConfigurationManager.h"
#include <list>
#include <vector>
#include "ThreadWorker.h"

int main(int argc, char** argv)
{
    std::string filename("");
    if (argc > 1)
    {
        filename = argv[1];
    }

    bool initialized = ConfigurationManager::Instance().Initialize(filename);

    if (!initialized)
    {
        std::cout << "Error on loading configuration!" << std::endl;
        return -1;
    }

    const std::list<ConnectionConfig> configs = ConfigurationManager::Instance().GetConfiguration();

    std::vector<std::shared_ptr<ThreadWorker>> threads;

    for (const ConnectionConfig& config : configs)
    {
        threads.push_back(std::make_shared<ThreadWorker>(config));
    }

    for (auto& thread : threads)
    {
        thread->Join();
    }
}