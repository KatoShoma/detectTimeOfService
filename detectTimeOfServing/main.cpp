//
//  main.cpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "select_roi.hpp"
#include "detect_service.hpp"

int main(int argc, const char * argv[]) {
    const std::string file_path = "/Users/x17032xx/Desktop/service/test_movie/test_movie02.mp4";//ファイルパスの指定
    //インストラクタ
    cv::Rect2d roi_size;
    
    //ROI選択のためのインストラクタ
    SelectRoi select_roi;
    
    //ROIの範囲保存
    roi_size = select_roi.returnRoiSize(file_path);
    
    
    std::chrono::system_clock::time_point start, end;//処理時間計測用変数
    start = std::chrono::system_clock::now();//処理開始時間の取得
    
    //サーブ時刻プログラムのインストラクタ
    DetectService detect_service;

    detect_service.calcDifferenceBetweenFrames(file_path, roi_size);
    
    end = std::chrono::system_clock::now();//終了時間の取得
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();
    std::cerr << elapsed << "s" <<std::endl;//実行時間の印字
    
    
    return 0;
}
