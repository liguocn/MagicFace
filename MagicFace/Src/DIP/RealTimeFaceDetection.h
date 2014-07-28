#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "ImageLoader.h"

namespace MagicDIP
{
    struct HaarFeature
    {
        int sRow;
        int sCol;
        int lRow;
        int lCol;
        int type;
    };

    class HaarClassifier
    {
    public:
        HaarClassifier();
        HaarClassifier(const HaarFeature& feature);
        ~HaarClassifier();

        void SetFeature(const HaarFeature& feature);
        int Learn(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, const ImageLoader& nonFaceImgLoader,
            const std::vector<double>& nonFaceDataWeights, const std::vector<int>& nonFaceIndex, double* trainError);
        int Predict(const std::vector<unsigned int>& integralImg, int sRow, int sCol, float scale) const;
        int Predict(const ImageLoader& imgLoader, int dataId) const;
        void Save(std::ofstream& fout) const;
        void Load(std::ifstream& fin);

    private:
        int CalFeatureValue(const ImageLoader& imgLoader, int dataId) const;
        int ImgBoxValue(const ImageLoader& imgLoader, int dataId, int sRow, int sCol, int eRow, int eCol) const;

    private:
        HaarFeature mFeature;
        int mThreshold;
        bool mIsLess;
    };

    class AdaBoostFaceDetection
    {
    public:
        AdaBoostFaceDetection();
        ~AdaBoostFaceDetection();

        int Learn(const ImageLoader& faceImgLoader, const ImageLoader& nonFaceImgLoader, const std::vector<bool>& nonFaceValidFlag,
            int levelCount);
        int Predict(const std::vector<unsigned int>& integralImg, int sRow, int sCol, double scale) const;
        int Predict(const ImageLoader& imgLoader, int dataId) const;
        void Save(std::ofstream& fout) const;
        void Load(std::ifstream& fin);

    private:
        void GenerateClassifierCadidates(int baseImgSize);
        int TrainWeakClassifier(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, 
            const ImageLoader& nonFaceImgLoader, const std::vector<double>& nonFaceDataWeights, 
            const std::vector<int>& nonFaceIndex);
        void Reset(void);

    private:
        std::vector<HaarClassifier*> mClassifiers;
        std::vector<double> mClassifierWeights;
        double mThreshold;
        //Cache classifier candidates
        std::vector<HaarClassifier*> mClassifierCandidates;
    };

    class RealTimeFaceDetection
    {
    public:
        RealTimeFaceDetection();
        ~RealTimeFaceDetection();

        int Learn(const std::vector<std::string>& faceImages, const std::vector<std::string>& nonFaceImages, 
            const std::vector<int>& layerCounts);
        int Detect(const cv::Mat& img, std::vector<int>& faces) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);

    private:
        int DetectOneFace(const std::vector<unsigned int>& integralImg, int sRow, int sCol, double scale) const;
        void Reset(void);

    private:
        int mBaseImgSize;
        std::vector<AdaBoostFaceDetection* > mCascadedDetectors;
    };
}
