#ifndef MACHINE_LEARNERS_MANAGER
#define MACHINE_LEARNERS_MANAGER

#include <map>
#include <string>


class IMachineLearner;

class MachineLearnersManager {

public:
    MachineLearnersManager();
    virtual ~MachineLearnersManager();

    IMachineLearner * getMachineLearner(std::string machineLearnerName);
    void registerMachineLearner(IMachineLearner *machineLearner, std::string machineLearnerName);

private:
    std::map<std::string, IMachineLearner *> m_machineLearners;
};

#endif // MACHINE_LEARNERS_MANAGER
