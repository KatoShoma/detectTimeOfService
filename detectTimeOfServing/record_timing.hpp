//
//  record_timing.hpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/08/08.
//  Copyright © 2019年 AIT. All rights reserved.
//

#ifndef record_timing_hpp
#define record_timing_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <numeric>
#include <string>
#include <valarray>
#include <fstream>
#include <opencv2/opencv.hpp>

class Recoder{
private:
public:
    Recoder();
    int service_timing;//サーブのタイング
    cv::Mat service_timing_img;//サーブのタイミングの画像
};
#endif /* record_timing_hpp */
