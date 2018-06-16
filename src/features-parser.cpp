#include "features-parser.h"


FeaturesParser::FeaturesParser()
{
}

FeaturesParser::~FeaturesParser()
{
}

void FeaturesParser::load(std::string fileName)
{
    clear();

    m_featuresFile.open(fileName);
 
    if (!m_featuresFile.is_open())
    {
        std::cout << "Unable to open file" << std::endl;
        return;
    }

    std::string line;
    int lineCount = 0;

    while (getline(m_featuresFile, line)) {

        std::istringstream lineStream(line);
        std::string value, colon;
        lineStream >>  value >> colon;
        if (lineCount < 1000)
            m_trainLabels.push_back(value);
        else
            m_testLabels.push_back(value);

        std::vector<double> frameFeatures;
        while (getline(lineStream, value, ',')) {
            frameFeatures.push_back(std::stod(value));
        }
        if (lineCount < 1000)
            m_trainSamples.push_back(frameFeatures);
        else
            m_testSamples.push_back(frameFeatures);
        lineCount++;
    }

    m_featuresFile.close();
}

void FeaturesParser::clear()
{
   m_trainSamples.clear();
   m_trainLabels.clear();
   m_testSamples.clear();
   m_testLabels.clear();
}
