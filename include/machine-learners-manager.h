#ifndef MACHINE_LEARNERS_MANAGER
#define MACHINE_LEARNERS_MANAGER

#include <map>
#include <memory>
#include <string>


class IMachineLearner;

class MachineLearnersManager {

public:
    MachineLearnersManager();
    virtual ~MachineLearnersManager();

    std::shared_ptr<IMachineLearner> getMachineLearner(
            std::string machineLearnerName);

    void registerMachineLearner(
            std::shared_ptr<IMachineLearner> machineLearner,
            std::string machineLearnerName);

private:
    std::map<std::string, std::shared_ptr<IMachineLearner>> m_machineLearners;
};

#endif // MACHINE_LEARNERS_MANAGER
