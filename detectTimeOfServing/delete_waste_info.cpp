//
//  delete_waste_info.cpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#include "delete_waste_info.hpp"

DeleteWasteInfo::DeleteWasteInfo(){
    
}

void DeleteWasteInfo::deleteMinimam(std::vector<Recoder> &service_timing){
    int count_same_time = 0;//同じ時間の総フレーム数を取得
    int prev_time = 0;//前の時間を記録
    Recoder prev_service_timing;
    
    std::vector<Recoder> new_service_timing;//出力結果
    
    for(Recoder i : service_timing){//iに値が入る
        
        if(i.service_timing == prev_time){//iが前の時間と一致
//            cv::imshow("result", i.service_timing_img);
//            cv::waitKey();
            count_same_time++;//同じ時間のものがいくつあるかをカウント
            
        }else{
            
            if(count_same_time > 1 && prev_time != 0){//2フレーム以上同じ時間のものが発見された場合
                
               new_service_timing.push_back(prev_service_timing);//サーブの時間として登録
                
            }
            prev_time = i.service_timing;//参照時間を更新
            prev_service_timing = i;
            count_same_time = 0;//カウントをリセット
            
        }
        
    }
    service_timing = new_service_timing;//処理結果をコピー
}

void DeleteWasteInfo::deleteAdjacentTimeFrames(std::vector<Recoder> &service_time){
    std::vector<Recoder> new_service_timing;//出力
    Recoder prev_service_timing;
    
    
    int prev_service_time = 0;//前時間の保存
    
    const int time_margin = 1;//間が1秒しかない場合は消す
    
    
    for(Recoder i : service_time){
        
        if(i.service_timing - prev_service_time > time_margin && prev_service_time != 0){
            new_service_timing.push_back(prev_service_timing);//登録
        }
        prev_service_time = i.service_timing;
        prev_service_timing = i;//前時間の更新
    }
    
    new_service_timing.push_back(service_time.back());
    service_time = new_service_timing;
    
}

void DeleteWasteInfo::deleteNearTimeFrames(std::vector<Recoder> &service_time){
    std::vector<Recoder> new_service_timing;//出力
    Recoder prev_service_timing;
    
    
    int prev_service_time = 0;//前時間の保存
    
    const int time_margin = 5;//間が5秒しかない場合は消す
    
    
    for(Recoder i : service_time){
        
        if(i.service_timing - prev_service_time > time_margin && prev_service_time != 0){
            new_service_timing.push_back(prev_service_timing);//登録
        }
        prev_service_time = i.service_timing;
        prev_service_timing = i;//前時間の更新
    }
    
    new_service_timing.push_back(service_time.back());
    service_time = new_service_timing;
}
