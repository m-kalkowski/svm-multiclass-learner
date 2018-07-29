#include "bayesclassifer.h"

/*
 *Zeby to cos uzyc trzeba zaladowac plik z klasyfikatorem potem wowalac normalize na wszytkihc probkach
 * nastepnie dajessz mu kolejne rownanie
 * normalizowania kazdego po kolei nie dziala
*/
BayesClassifer::BayesClassifer()
{

}

void BayesClassifer::predict(sample_type &testSamples,
                             std::vector<double> &predictedLabels,
                             std::string modelName)
{
    m_pBayesClassifier = cv::ml::NormalBayesClassifier::load(modelName);

    cv::Mat normalizedSamples;
    cv::Mat result;

    //normalize(testSamples, normalizedSamples);
    cv::Mat samplesMat;
    for (unsigned int i = 0; i < testSamples.size(); ++i)
    {
      // Make a temporary cv::Mat row and add to NewSamples _without_ data copy
        cv::Mat singleSignal(1, testSamples[0].size(), CV_32F, testSamples[i].data());

        samplesMat.push_back(singleSignal);
    }



    for (size_t i=0; i<testSamples.size(); ++i) {
        cv::Mat currentRow = samplesMat.row(i);
        m_pBayesClassifier->predict(currentRow, result, 2);
        predictedLabels.push_back(result.at<uint8_t>(0));
    }
}

void BayesClassifer::normalize(sample_type &samples, cv::Mat &normlizedMatrix)
{
    cv::Mat samplesMat;
    for (unsigned int i = 0; i < samples.size(); ++i)
    {
      // Make a temporary cv::Mat row and add to NewSamples _without_ data copy
        cv::Mat singleSignal(1, samples[0].size(), CV_64F, samples[i].data());

        samplesMat.push_back(singleSignal);
    }
    cv::Mat normalizeTest;
    cv::normalize(samplesMat,normalizeTest);
    normalizeTest.convertTo(normlizedMatrix,CV_32F);
}
//WYKORZYSTANIE ZEBY SPRAWDZIC WSZYSTKO WYSZLO MI 1181
/*

BayesClassifer bayesNC(bayesClassifierPath);
cv::Mat outt;
bayesNC.normalize(testingSamples,outt);
int hits=0;
for (uint64_t i=0;i<testingSamples.size(); i++)
{
    //float prediction = bayesClassifier->predict(normalizeTest2.at(i),bayesTestLabels);
    cv::Mat currentRow = outt.row(i);
    uint8_t result = bayesNC.predict(currentRow);
    std::cout << result << std::endl;
    if (result == intLabels[i])
        hits++;
}
std::cout << "NUMBER OF HITS " << hits << std::endl;
*/
