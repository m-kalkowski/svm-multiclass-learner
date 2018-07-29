#ifndef BAYESCLASSIFER_H
#define BAYESCLASSIFER_H

#include <string>
#include "opencv2/ml.hpp"
#include "opencv2/core/cvstd.hpp"
#include "i-machine-learner.h"


class BayesClassifer : public IMachineLearner
{
public:
    BayesClassifer();

    virtual void train(sample_type &trainSamples,
                       label_type &trainLabels,
                       std::string modelName) { /* do nothing */ }

    virtual void predict(sample_type &testSamples,
                         std::vector<double> &predictedLabels,
                         std::string modelName);


    void normalize(sample_type &samples, cv::Mat &normlizedMatrix);

private:
    cv::Ptr<cv::ml::NormalBayesClassifier> m_pBayesClassifier;
};

#endif // BAYESCLASSIFER_H
