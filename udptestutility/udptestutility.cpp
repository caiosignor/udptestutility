#include <iostream>
#include "ConfigurationManager.h"
#include <list>
#include <vector>
#include "ThreadWorker.h"

int main(int argc, char** argv)
{
    std::string filename(argv[1]);

    ConfigurationManager::Instance().Initialize(filename);

    std::list<ConnectionConfig> configs = ConfigurationManager::Instance().GetConfiguration();

    std::vector<std::shared_ptr<ThreadWorker>> threads;

    for (ConnectionConfig config : configs)
    {
        threads.push_back(std::make_shared<ThreadWorker>(config));
    }

    while (true)
    {
        for (auto& thread : threads)
        {
            thread->RunLoop();
        }
    }
}