#include "machine-learners-manager.h"

#include <iostream>


MachineLearnersManager::MachineLearnersManager()
{

}

MachineLearnersManager::~MachineLearnersManager()
{

}

std::shared_ptr<IMachineLearner> MachineLearnersManager::getMachineLearner(
        std::string machineLearnerName)
{
    auto result = m_machineLearners.find(machineLearnerName);
    if (result != m_machineLearners.end())
        return m_machineLearners[machineLearnerName];
    else
        return nullptr;
}

void MachineLearnersManager::registerMachineLearner(
        std::shared_ptr<IMachineLearner> machineLearner,
        std::string machineLearnerName)
{
    auto result = m_machineLearners.find(machineLearnerName);
    if (result == m_machineLearners.end())
        m_machineLearners[machineLearnerName] = machineLearner;
    else
        std::cout << "Machine learner with name provided already exists." << std::endl;
}
