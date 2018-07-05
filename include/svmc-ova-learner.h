#ifndef SVMC_OVA_LEARNER_H
#define SVMC_OVA_LEARNER_H

#include "i-machine-learner.h"


class SvmcOvaLearner: public IMachineLearner {

public:
    SvmcOvaLearner();

    virtual ~SvmcOvaLearner();

    virtual void train(sample_type &trainSamples,
                       label_type &trainLabels,
                       std::string modelName);

    virtual void predict(sample_type &testSamples,
                         std::vector<double> &predictedLabels,
                         std::string modelName);
};

#endif // SVMC_OVA_LEARNER_H

