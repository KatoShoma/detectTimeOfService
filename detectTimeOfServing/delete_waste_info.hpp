//
//  delete_waste_info.hpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#ifndef delete_waste_info_hpp
#define delete_waste_info_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "record_timing.hpp"

class DeleteWasteInfo{
private:
public:
    DeleteWasteInfo(); //コンストラクタ
    
    void deleteMinimam(std::vector<Recoder> &service_timing);//少ない値を除去
    
    void deleteAdjacentTimeFrames(std::vector<Recoder> &service_time);//近い時間のものを除去
    
    void deleteNearTimeFrames(std::vector<Recoder> &service_time);
};
#endif /* delete_waste_info_hpp */
