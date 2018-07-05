#ifndef SVMNU_OVO_LEARNER_H
#define SVMNU_OVO_LEARNER_H

#include "i-machine-learner.h"


class SvmnuOvoLearner: public IMachineLearner {

public:
    SvmnuOvoLearner();

    virtual ~SvmnuOvoLearner();

    virtual void train(sample_type &trainSamples,
                       label_type &trainLabels,
                       std::string modelName);

    virtual void predict(sample_type &testSamples,
                         std::vector<double> &predictedLabels,
                         std::string modelName);
};

#endif // SVMNU_OVO_LEARNER_H

