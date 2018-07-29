#include "bayesclassifier.h"
#include "features-parser.h"

//stoper jak w matlabie
auto m_startTime = std::chrono::steady_clock::now();

void tic()
{
    m_startTime = std::chrono::steady_clock::now();
}


double toc()
{
    const auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> timeDifference =
                   endTime - m_startTime;
    std::cout << "Prediciton time is " << timeDifference.count() << std::endl;
    return timeDifference.count();
}


//Dajesz dwi sciezki do bayesa i druga do danych
//potem dajesz predictVectors
BayesClassifier::BayesClassifier(std::string& filePath, std::string& dataPath)
{
    FeaturesParser parser;
    parser.load(dataPath);
    m_pBayesClassifier = cv::ml::NormalBayesClassifier::load(cv::String(filePath));
    sample_type testingSamples = parser.getTestSamples();
    normalize(testingSamples);
}

uint8_t BayesClassifier::_predict(size_t signalToPredict)
{
    cv::Mat result;
    tic();
    m_pBayesClassifier->predict(m_normalizeData.row(signalToPredict),result);
    toc();
    return result.at<uint8_t>(0);
}

void BayesClassifier::predictVector(std::vector<size_t>& signalNumber, std::vector<double>& predictedLabels)
{
    assert(signalNumber.size() > 0);
    predictedLabels.clear();
    predictedLabels.resize(signalNumber.size());
    for (size_t i=0; i<signalNumber.size(); i++)
        predictedLabels[i] = _predict(signalNumber[i]);
}


void BayesClassifier::normalize(std::vector<std::vector<double>>& samples, cv::Mat& normlizedMatrix)
{
    cv::Mat samplesMat;
    for (unsigned int i = 0; i < samples.size(); ++i)
    {
        cv::Mat singleSignal(1, samples[0].size(), CV_64F, samples[i].data());
        samplesMat.push_back(singleSignal);
    }
    cv::Mat normalizeTest;
    cv::normalize(samplesMat,normalizeTest);
    normalizeTest.convertTo(normlizedMatrix,CV_32F);
    m_normalizeData = normlizedMatrix.clone();
}

void BayesClassifier::normalize(std::vector<std::vector<double>>& samples)
{
    cv::Mat samplesMat;
    for (unsigned int i = 0; i < samples.size(); ++i)
    {
        cv::Mat singleSignal(1, samples[0].size(), CV_64F, samples[i].data());
        samplesMat.push_back(singleSignal);
    }
    cv::Mat normalizeTest;
    cv::normalize(samplesMat,normalizeTest);
    normalizeTest.convertTo(m_normalizeData,CV_32F);
}


