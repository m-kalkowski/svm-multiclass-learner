#ifndef SVMNU_OVA_LEARNER_H
#define SVMNU_OVA_LEARNER_H

#include "i-machine-learner.h"


class SvmnuOvaLearner: public IMachineLearner {

public:
    SvmnuOvaLearner();

    virtual ~SvmnuOvaLearner();

    virtual void train(sample_type &trainSamples,
                       label_type &trainLabels,
                       std::string modelName);

    virtual void predict(sample_type &testSamples,
                         std::vector<double> &predictedLabels,
                         std::string modelName);
};

#endif // SVMNU_OVA_LEARNER_H

