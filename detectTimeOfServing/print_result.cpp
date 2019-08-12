//
//  print_result.cpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#include "print_result.hpp"

PrintResultTime::PrintResultTime(){
    
}

void PrintResultTime::printServiceTimimg(std::vector<Recoder> service_timing){
    int m = 0 , s = 0;//分、秒
    for(auto record_time : service_timing){
        std::ostringstream min, sec;
        m = record_time.service_timing/60;//分の計算
        s = record_time.service_timing%60;//秒の計算
        min << std::setw(2) << std::setfill('0') << m ;//2桁に設定(分)
        sec << std::setw(2) << std::setfill('0') << s ;//2桁に設定(秒)
        std::string mm(min.str());//string型に変更
        std::string ss(sec.str());//string型に変更
        std::cout << record_time.service_timing << " " << "00:" << mm << ":" << ss <<std::endl;//時間の印字
    }
}
