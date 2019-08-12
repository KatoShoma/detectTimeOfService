//
//  detect_service.hpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#ifndef detect_service_hpp
#define detect_service_hpp

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <numeric>
#include <string>
#include <valarray>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>

#include "delete_waste_info.hpp"
#include "print_result.hpp"
#include "record_timing.hpp"

class DetectService{
private:
    cv::Mat frame_img;//注目フレームのオリジナル画像
    cv::Mat prev_img, curr_img, op_img, prev_gray, curr_gray, ave_img;//前フレーム画像と現フレーム画像
    
    
    double sum_light_dis = 0;//ROI右側移動量
    
    double sum_left_dis = 0;//ROI左側移動量
    
    double sum_dis = 0;//特徴点の移動量
    
    const double reduction_rate = 1.0/3.0;//縮小サイズ
    
    const int fps = 30;//フレームレート
    
    const int margin_frames = 3;//差分を計算するフレームのフレーム番号の差

    double calcDistance(cv::Point2f pt1, cv::Point2f pt2);//二点間距離を計算
    
    int sec = 0, prev_sec = 0;//フレームの時間(s)と前の時間(s-1)を記録
    
    bool detect_person_flag = false;//人の発見があったかのフラグ
    
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 20, 0.05);  //反復アルゴリズム停止基準
    std::vector<cv::Point2f> priorFeature, presentFeature;  //前フレームおよび現フレーム特徴点
    
    std::vector<unsigned char> status;  //作業用
    
    std::vector<float> errors;  //作業用
    
    Recoder tmp_recode_timing;
    
    std::vector<Recoder> record_timing;//時間の記録
    
    std::vector<int> detect_person;//実時間内で人の発見があったかを記録
    
    void calcColorAve(cv::Mat src_img, cv::Mat average_img, int frame_num, std::vector<std::vector<int>> &R, std::vector<std::vector<int>> &G, std::vector<std::vector<int>> &B);
    
    std::vector<std::vector<int>>R; std::vector<std::vector<int>>G; std::vector<std::vector<int>>B;
    
    std::vector<int>t_R; std::vector<int>t_G; std::vector<int>t_B;//一時登録用
    
    std::vector<Recoder> detect_time_out;
    
    int ave = 0;
    
public:
    DetectService();//コンストラクタ
    
    void calcDifferenceBetweenFrames(std::string file_path, cv::Rect2d select_rect);//全体を担う関数
};
#endif /* detect_service_hpp */
