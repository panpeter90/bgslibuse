/*
This file is part of BGSLibrary.

BGSLibrary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

BGSLibrary is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BGSLibrary.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "package_bgs/FrameDifferenceBGS.h"
#include "package_bgs/StaticFrameDifferenceBGS.h"
#include "package_bgs/WeightedMovingMeanBGS.h"
#include "package_bgs/WeightedMovingVarianceBGS.h"
#include "package_bgs/MixtureOfGaussianV1BGS.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_bgs/AdaptiveBackgroundLearning.h"
#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >= 4 && CV_SUBMINOR_VERSION >= 3
#include "package_bgs/GMG.h"
#endif

#include "package_bgs/dp/DPAdaptiveMedianBGS.h"
#include "package_bgs/dp/DPGrimsonGMMBGS.h"
#include "package_bgs/dp/DPZivkovicAGMMBGS.h"
#include "package_bgs/dp/DPMeanBGS.h"
#include "package_bgs/dp/DPWrenGABGS.h"
#include "package_bgs/dp/DPPratiMediodBGS.h"
#include "package_bgs/dp/DPEigenbackgroundBGS.h"
#include "package_bgs/dp/DPTextureBGS.h"

#include "package_bgs/tb/T2FGMM_UM.h"
#include "package_bgs/tb/T2FGMM_UV.h"
#include "package_bgs/tb/T2FMRF_UM.h"
#include "package_bgs/tb/T2FMRF_UV.h"
#include "package_bgs/tb/FuzzySugenoIntegral.h"
#include "package_bgs/tb/FuzzyChoquetIntegral.h"

#include "package_bgs/lb/LBSimpleGaussian.h"
#include "package_bgs/lb/LBFuzzyGaussian.h"
#include "package_bgs/lb/LBMixtureOfGaussians.h"
#include "package_bgs/lb/LBAdaptiveSOM.h"
#include "package_bgs/lb/LBFuzzyAdaptiveSOM.h"

#if !defined(_WIN32)
// Currently this method works only on Linux platform.
#include "package_bgs/ck/LbpMrf.h"
#endif

#include "package_bgs/jmo/MultiLayerBGS.h"
#include "package_bgs/pt/PixelBasedAdaptiveSegmenter.h"
#include "package_bgs/av/VuMeter.h"
#include "package_bgs/ae/KDE.h"
#include "package_bgs/db/IndependentMultimodalBGS.h"
#include <ctime>
int main(int argc, char **argv)
{
  CvCapture *capture = 0;
  int resize_factor = 100;

    std::cout << "Opening: " << ""<< std::endl;
    capture = cvCaptureFromAVI("video.avi");

  
  IplImage *frame_aux = cvQueryFrame(capture);
  IplImage *frame = cvCreateImage(cvSize((int)((frame_aux->width*resize_factor)/100) , (int)((frame_aux->height*resize_factor)/100)), frame_aux->depth, frame_aux->nChannels);
  cvResize(frame_aux, frame);

  /* Background Subtraction Methods */
  IBGS *bgs;

  /*** Default Package ***/
  //bgs = new FrameDifferenceBGS; //0.001 s
  bgs = new StaticFrameDifferenceBGS; //0.002s
  //bgs = new WeightedMovingMeanBGS; // 0.011 s
  //bgs = new WeightedMovingVarianceBGS; //0.025
  //bgs = new MixtureOfGaussianV1BGS; //sratch
  //bgs = new MixtureOfGaussianV2BGS; //0.025
  //bgs = new AdaptiveBackgroundLearning; //0.012 bad
  //bgs = new GMG; //sratch
  
  /*** DP Package (thanks to Donovan Parks) ***/
  //bgs = new DPAdaptiveMedianBGS; //0.031 bad
  //bgs = new DPGrimsonGMMBGS; //0.07 bad
  //bgs = new DPZivkovicAGMMBGS;
  //bgs = new DPMeanBGS;
  //bgs = new DPWrenGABGS;
  //bgs = new DPPratiMediodBGS;
  //bgs = new DPEigenbackgroundBGS;
  //bgs = new DPTextureBGS;

  /*** TB Package (thanks to Thierry Bouwmans, Fida EL BAF and Zhenjie Zhao) ***/
  //bgs = new T2FGMM_UM; //0.08 fairy
 // bgs = new T2FGMM_UV; //bad
  //bgs = new T2FMRF_UM; //0.152 bad
 // bgs = new T2FMRF_UV; //0.16 bad//
	//bgs = new FuzzySugenoIntegral; // 0.123 bad
 // bgs = new FuzzyChoquetIntegral; //0.25 bad

  /*** JMO Package (thanks to Jean-Marc Odobez) ***/
 // bgs = new MultiLayerBGS; //0.187 good

  /*** PT Package (thanks to Martin Hofmann, Philipp Tiefenbacher and Gerhard Rigoll) ***/
  //bgs = new PixelBasedAdaptiveSegmenter; //0.5s OHMYGOD

  /*** LB Package (thanks to Laurence Bender) ***/
  //bgs = new LBSimpleGaussian; //0.031 bad
  //bgs = new LBFuzzyGaussian; //0.037 bad
 // bgs = new LBMixtureOfGaussians; //0.04s
  //bgs = new LBAdaptiveSOM; //0.041 bad
  //bgs = new LBFuzzyAdaptiveSOM; //0.045 bad

  /*** LBP-MRF Package (thanks to Csaba Kertész) ***/
  //bgs = new LbpMrf;

  /*** AV Package (thanks to Lionel Robinault and Antoine Vacavant) ***/
  //bgs = new VuMeter; //0.021 bad

  /*** EG Package (thanks to Ahmed Elgammal) ***/
  //bgs = new KDE; //0.021 bad
  
  /*** DB Package (thanks to Domenico Daniele Bloisi) ***/
  //bgs = new IndependentMultimodalBGS; //0.146 bad

  int key = 0;
  while(key != 'q')
  {
    frame_aux = cvQueryFrame(capture);
    if(!frame_aux) break;

    cvResize(frame_aux, frame);
    
    cv::Mat img_input(frame);
    cv::imshow("input", img_input);
    cv::Mat img_mask;
    cv::Mat img_bkgmodel;
	clock_t begin = clock();
    bgs->process(img_input, img_mask, img_bkgmodel); // automatically shows the foreground mask image
    std::cout << "Take time: " << float(clock() - begin)/CLOCKS_PER_SEC << "\n"<< std::endl;
    //if(!img_mask.empty())
    //  do something
    key = cvWaitKey(50);
  }
  system("pause");
  delete bgs;

  cvDestroyAllWindows();
  cvReleaseCapture(&capture);
  
  return 0;
}
