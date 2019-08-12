//
//  select_roi.cpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#include "select_roi.hpp"

using namespace cv::superres;//selectROI関数使用に必要

SelectRoi::SelectRoi(){
    std::cout << "注目領域を選択してください" << std::endl;
}

cv::Rect2d SelectRoi::returnRoiSize(std::string file_path){
    
    cv::Rect2d select_rect;//
    
    //ムービーの読み込み
    cv::VideoCapture cap(file_path);
    
    //エラー時
    if(!cap.isOpened()){
        std::cerr << "File is not found." << std::endl;
    }
    
    while(1){
        cap >> first_frame_img;//1フレーム目を読み込み
        
        //画像が空の場合
        if(first_frame_img.empty() == true){
            break;
        }
        
        cv::resize(first_frame_img, first_frame_img, cv::Size(), 640.0/first_frame_img.cols ,640.0/first_frame_img.cols);
        
        select_rect = selectROI(first_frame_img);//double型でROIの指定
        
        std::cerr << "x=" << select_rect.x << " y=" << select_rect.y << std::endl;
        std::cerr << "width=" << select_rect.width << " height=" << select_rect.height << std::endl;
        
        break;
        
    }
    
    return (select_rect);
}
