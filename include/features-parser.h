#ifndef FEATURES_PARSER_H
#define FEATURES_PARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


typedef std::vector<std::vector<double>> sample_type;
typedef std::vector<std::string> label_type;

struct range
{
    size_t first;
    size_t last;
};

class FeaturesParser
{
public :
    static constexpr auto trainVectorsCount = 1000;
    static constexpr auto numOfFeatures = 319;

    FeaturesParser();
    virtual ~FeaturesParser();

    bool load(std::string fileName);

    inline sample_type & getAllSamples()   { return m_allSamples;   }
    inline label_type  & getAllLabels()    { return m_allLabels;    }
    inline sample_type & getTrainSamples() { return m_trainSamples; }
    inline label_type  & getTrainLabels()  { return m_trainLabels;  }
    inline sample_type & getTestSamples()  { return m_testSamples;  }
    inline label_type  & getTestLabels()   { return m_testLabels;   }

    inline std::vector<range> & getRanges() { return m_ranges; }

    inline int channels() { return m_numOfChannels; }

private:
    void clear();
    void setTestLabelsRanges();

    std::ifstream m_featuresFile;
    int m_numOfChannels;
    std::vector<range> m_ranges;

    sample_type m_allSamples;
    label_type m_allLabels;
    sample_type m_trainSamples;
    label_type m_trainLabels;
    sample_type m_testSamples;
    label_type m_testLabels;
};

#endif // FEATURES_PARSER_H
