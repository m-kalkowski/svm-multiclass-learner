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

    while (getline(m_featuresFile, line)) {

        std::istringstream lineStream(line);
        std::string value, colon;
        lineStream >>  value >> colon;
         
        m_allLabels.push_back(value);

        std::vector<double> frameFeatures;
        while (getline(lineStream, value, ','))
            frameFeatures.push_back(std::stod(value));
        
        m_allSamples.push_back(frameFeatures);
    }

    sample_type::const_iterator firstSample = m_allSamples.begin();
    sample_type::const_iterator midSample  = m_allSamples.begin() + 1000;
    sample_type::const_iterator lastSample = m_allSamples.end();
    m_trainSamples.assign(firstSample, midSample);
    m_testSamples.assign(midSample, lastSample);

    label_type::const_iterator firstLabel = m_allLabels.begin();
    label_type::const_iterator midLabel  = m_allLabels.begin() + 1000;
    label_type::const_iterator lastLabel = m_allLabels.end();
    m_trainLabels.assign(firstLabel, midLabel);
    m_testLabels.assign(midLabel, lastLabel);

    setTestLabelsRanges();

    m_featuresFile.close();
}

void FeaturesParser::clear()
{
    m_allSamples.clear();
    m_allLabels.clear(); 
    m_trainSamples.clear();
    m_trainLabels.clear();
    m_testSamples.clear();
    m_testLabels.clear();
}

void FeaturesParser::setTestLabelsRanges()
{
    int first = 0;

    for (auto i=1; i<m_testLabels.size(); ++i)
    {
        if ( m_testLabels.at(i-1) != m_testLabels.at(i))
        {
            m_ranges.push_back(range{first, i});
            first = i + 1;
        }
    }

    m_ranges.push_back(range{first, m_testLabels.size()});
}
