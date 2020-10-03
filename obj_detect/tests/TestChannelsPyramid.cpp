
#include <pyramid/ChannelsPyramid.h>
#include <pyramid/ChannelsPyramidComputeAllStrategy.h>
#include <pyramid/ChannelsPyramidComputeAllParallelStrategy.h>
#include <pyramid/ChannelsPyramidApproximatedStrategy.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "gtest/gtest.h"

class TestChannelsPyramid: public testing::Test
{
public:
  ChannelsPyramid* pChnsPyramid;
  virtual void SetUp()
    {
      pChnsPyramid = nullptr;
    }

  virtual void TearDown()
    {
      if (pChnsPyramid)
      {
        delete pChnsPyramid;
      }
    }
};

TEST_F(TestChannelsPyramid, TestGetScales)
{
  int nPerOct = 8;
  int nOctUp = 1;
  int shrink = 4;
  cv::Size size;
  size.height = 19;
  size.width = 22;
  cv::Size minDS;
  size.height = 16;
  size.width = 16;
  std::vector<double> scales;
  std::vector<cv::Size2d> scaleshw;

  if (pChnsPyramid)
  {
    delete pChnsPyramid;
  }
  pChnsPyramid = dynamic_cast<ChannelsPyramid*>( new ChannelsPyramidComputeAllStrategy() );

  pChnsPyramid->getScales(nPerOct, nOctUp, minDS, shrink, size, scales, scaleshw);
  std::vector<float> check = {2.1463, 1.8537, 1.6589, 1.4632, 1.2684, 1.0737, 0.8779};

  for(uint i = 0; i < scales.size(); i++){
    ASSERT_TRUE(abs(scales[i]-check[i])<1.e-3f);
  }
}

TEST_F(TestChannelsPyramid, TestGetScalesChangeVals)
{
  int nPerOct = 7;
  int nOctUp = 0;
  int shrink = 4;
  cv::Size size;
  size.height = 30;
  size.width = 30;
  cv::Size minDS;
  size.height = 16;
  size.width = 16;
  std::vector<double> scales;
  std::vector<cv::Size2d> scaleshw;

  if (pChnsPyramid)
  {
    delete pChnsPyramid;
  }
  pChnsPyramid = dynamic_cast<ChannelsPyramid*>( new ChannelsPyramidComputeAllStrategy() );

  pChnsPyramid->getScales(nPerOct, nOctUp, minDS, shrink, size, scales, scaleshw);
  pChnsPyramid->getScales(nPerOct, nOctUp, minDS, shrink, size, scales, scaleshw);
  std::vector<float> check = {1.0667, 0.9333, 0.8000, 0.6667, 0.5333};

  for(uint i = 0; i < scales.size(); i++){
    ASSERT_TRUE(abs(scales[i]-check[i])<1.e-3f);
  }
}

TEST_F(TestChannelsPyramid, channelsPyramidComputeAllStrategy)
{
  cv::Mat image = cv::imread("images/index.jpeg", cv::IMREAD_COLOR);

  std::string nameOpts = "yaml/pPyramid.yml";

  if (pChnsPyramid)
  {
    delete pChnsPyramid;
  }
  pChnsPyramid = dynamic_cast<ChannelsPyramid*>( new ChannelsPyramidComputeAllStrategy() );

  bool loadOk = pChnsPyramid->load(nameOpts.c_str());
  ASSERT_TRUE(loadOk);
  std::vector<cv::Mat> filters; // empty filters is ACF pyramid.
  std::vector<double> scales;
  std::vector<cv::Size2d> scaleshw;
  std::vector<std::vector<cv::Mat>> pyramid = pChnsPyramid->compute(image, filters, scales, scaleshw);
  //ASSERT_TRUE(pyramid.size()==28);
}

TEST_F(TestChannelsPyramid, channelsPyramidComputeAllParallelStrategy)
{
  cv::Mat image = cv::imread("images/index.jpeg", cv::IMREAD_COLOR);

  std::string nameOpts = "yaml/pPyramid.yml";

  if (pChnsPyramid)
  {
    delete pChnsPyramid;
  }
  pChnsPyramid = dynamic_cast<ChannelsPyramid*>( new ChannelsPyramidComputeAllParallelStrategy() );

  bool loadOk = pChnsPyramid->load(nameOpts.c_str());
  ASSERT_TRUE(loadOk);
  std::vector<cv::Mat> filters; // empty filters is ACF pyramid.
  std::vector<double> scales;
  std::vector<cv::Size2d> scaleshw;
  std::vector<std::vector<cv::Mat>> pyramid = pChnsPyramid->compute(image, filters, scales, scaleshw);
  //ASSERT_TRUE(pyramid.size()==28);
}

TEST_F(TestChannelsPyramid, channelsPyramidApproximatedStrategy)
{
  cv::Mat image = cv::imread("images/index.jpeg", cv::IMREAD_COLOR);

  std::string nameOpts = "yaml/pPyramid.yml";

  if (pChnsPyramid)
  {
    delete pChnsPyramid;
  }
  pChnsPyramid = dynamic_cast<ChannelsPyramid*>( new ChannelsPyramidApproximatedStrategy() );

  bool loadOk = pChnsPyramid->load(nameOpts.c_str());
  ASSERT_TRUE(loadOk);
  std::vector<cv::Mat> filters; // empty filters is ACF pyramid.
  std::vector<double> scales;
  std::vector<cv::Size2d> scaleshw;
  auto start = std::chrono::system_clock::now(); 
  std::vector<std::vector<cv::Mat>> pyramid = pChnsPyramid->compute(image, filters, scales, scaleshw);
  //ASSERT_TRUE(pyramid.size()==28);
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<float,std::milli> duration = end - start;
  std::cout << duration.count() << "ms" << std::endl;
}
