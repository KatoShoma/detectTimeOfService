//
//  print_result.hpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#ifndef print_result_hpp
#define print_result_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <numeric>
#include <string>
#include <valarray>
#include <fstream>

#include "record_timing.hpp"

class PrintResultTime{
private:
public:
    PrintResultTime();//コンストラクタ
    
    void printServiceTimimg(std::vector<Recoder> service_time_and_sum_move_pixels);//サーブ時間を印字するプログラム
};
#endif /* print_result_hpp */
