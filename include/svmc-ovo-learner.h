#ifndef SVMC_OVO_LEARNER_H
#define SVMC_OVO_LEARNER_H

#include "i-machine-learner.h"


class SvmcOvoLearner: public IMachineLearner {

public:
    SvmcOvoLearner();

    virtual ~SvmcOvoLearner();

    virtual void train(sample_type &trainSamples,
                       label_type &trainLabels,
                       std::string modelName);

    virtual void predict(sample_type &testSamples,
                         std::vector<double> &predictedLabels,
                         std::string modelName);
};

#endif // SVMC_OVO_LEARNER_H

