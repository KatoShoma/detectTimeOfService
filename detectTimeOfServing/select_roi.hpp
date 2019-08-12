//
//  select_roi.hpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#ifndef select_roi_hpp
#define select_roi_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <numeric>
#include <string>
#include <valarray>
#include <fstream>

class SelectRoi{
private:
    cv::Mat first_frame_img;
    
    cv::Mat roi_img;
public:
    
    //コンストラクタ
    SelectRoi();
    
    cv::Rect2d returnRoiSize(std::string file_path);
    
};
#endif /* select_roi_hpp */
