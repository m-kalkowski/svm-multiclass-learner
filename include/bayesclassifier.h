#ifndef BAYESCLASSIFER_H
#define BAYESCLASSIFER_H

#include <string>
#include "opencv2/ml.hpp"
#include "i-machine-learner.h"

class BayesClassifier: public IMachineLearner
{
public:
    BayesClassifier(std::string &filePath, std::string &dataPath);

    void predictVector (std::vector<size_t> &signalNumber, std::vector<double> &predictedLabels);
    uint8_t _predict(size_t signalToPredict);
    void normalize(std::vector<std::vector<double>>& samples, cv::Mat& normlizedMatrix);

private:
    cv::Ptr<cv::ml::NormalBayesClassifier> m_pBayesClassifier;

    cv::Mat m_normalizeData;
    void normalize(std::vector<std::vector<double>>& samples);

};

#endif // BAYESCLASSIFER_H
