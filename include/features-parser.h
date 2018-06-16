#ifndef FEATURES_PARSER_H
#define DEATURES_PARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::vector<double>> sample_type;
typedef std::vector<std::string> label_type;


class FeaturesParser
{
public :
    FeaturesParser();
    virtual ~FeaturesParser();

    void load(std::string fileName);

    inline sample_type & getTrainSamples() { return m_trainSamples; }
    inline label_type  & getTrainLabels()  { return m_trainLabels;  }
    inline sample_type & getTestSamples()  { return m_testSamples;  }
    inline label_type  & getTestLabels()   { return m_testLabels;   }

private:
    void clear();

    std::ifstream m_featuresFile;
    
    sample_type m_trainSamples;
    label_type m_trainLabels;
    sample_type m_testSamples;
    label_type m_testLabels;
};

#endif // FEATURES_PARSER_H
