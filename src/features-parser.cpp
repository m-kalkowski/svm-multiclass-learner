#include "features-parser.h"


FeaturesParser::FeaturesParser():
    m_numOfChannels(0)
{
}

FeaturesParser::~FeaturesParser()
{
}

bool FeaturesParser::load(std::string fileName)
{
    clear();

    m_featuresFile.open(fileName);

    if (!m_featuresFile.is_open())
        return false;

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

    m_numOfChannels = m_allSamples.back().size() / numOfFeatures;

    sample_type::const_iterator firstSample = m_allSamples.begin();
    sample_type::const_iterator midSample  = m_allSamples.begin() + trainVectorsCount;
    sample_type::const_iterator lastSample = m_allSamples.end();
    m_trainSamples.assign(firstSample, midSample);
    m_testSamples.assign(midSample, lastSample);

    label_type::const_iterator firstLabel = m_allLabels.begin();
    label_type::const_iterator midLabel  = m_allLabels.begin() + trainVectorsCount;
    label_type::const_iterator lastLabel = m_allLabels.end();
    m_trainLabels.assign(firstLabel, midLabel);
    m_testLabels.assign(midLabel, lastLabel);

    setTestLabelsRanges();

    m_featuresFile.close();

    return true;
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
    size_t first = 0;

    for (size_t i=1; i<m_testLabels.size(); ++i)
    {
        if ( m_testLabels.at(i-1) != m_testLabels.at(i))
        {
            m_ranges.push_back(range{first, i});
            first = i + 1;
        }
    }

    m_ranges.push_back(range{first, m_testLabels.size()});
}
