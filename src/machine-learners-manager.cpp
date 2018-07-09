#include "machine-learners-manager.h"

#include <iostream>


MachineLearnersManager::MachineLearnersManager()
{

}

MachineLearnersManager::~MachineLearnersManager()
{

}

IMachineLearner * getMachineLearner(std::string machineLearnerName)
{
    auto result = m_machineLearners.find(machineLearnerName);
    if (result != m_machineLearners.end())
        return m_machineLearners[machineLearnerName];
    else
        return nullptr;
}

void registerMachineLearner(IMachineLearner *machineLearner, std::string machineLearnerName)
{
    auto result = m_machineLearners.find(machineLearnerName);
    if (result == m_machineLearners.end())
        m_machineLearners[machineLearnerName] = machineLearner;
    else
        std::cout << "Machine learner with name provided already exists." << std::endl
}
