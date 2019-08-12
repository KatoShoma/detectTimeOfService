//
//  detect_service.cpp
//  detectTimeOfServing
//
//  Created by x17032xx on 2019/07/29.
//  Copyright © 2019年 AIT. All rights reserved.
//

#include "detect_service.hpp"

using namespace cv::superres;

//コンストラクタ
DetectService::DetectService(){
    std::cout << "サーブ時刻の探索開始" << std::endl;
}

void DetectService::calcDifferenceBetweenFrames(std::string file_path, cv::Rect2d select_rect){
    //ムービーファイルの読み込み
    cv::VideoCapture cap(file_path);
    
    //TV-L1アルゴリズムによるオプティカルフロー計算オブジェクトの生成
    cv::Ptr<DenseOpticalFlowExt> opticalFlow = cv::superres::createOptFlow_DualTVL1();
    
    //動画の総フレーム数を取得
    //int sum_frames = cap.get(cv::CAP_PROP_FRAME_COUNT);
    
    //エラー時
    if(!cap.isOpened()){
        std::cerr << "File is not found." << std::endl;
    }
    
    for(int frame_num=0; ; frame_num++){
        
        sec = frame_num / fps;//フレームが出現する時間(s)
        
        static bool first_processing = true;//最初の処理かを確認するboolian
        cap >> frame_img;//frame_num番目のフレームの読み込み
        
        //画像が空の場合
        if(frame_img.empty() == true){
            break;
        }
        
        cv::resize(frame_img, frame_img, cv::Size(), 640.0/frame_img.cols ,640.0/frame_img.cols);//LaboLiveの動画サイズに合わせてリサイズ
        
        //フレーム番号の差が3ではない時
        if(frame_num % margin_frames != 0){
            continue;
        }
        
        if(first_processing == true){//1フレーム目の処理
            
            prev_img = frame_img(select_rect);//ROIの大きさに切り抜き, 前フレームの画像として登録
            
            //ここで平均画像生成用にR,G,B値を保存する（関数にしたところ処理速度が落ちたためここに記述）
            for(int i=0; i<prev_img.rows; i++){
                for(int j=0; j<prev_img.cols; j++){
                    cv::Vec3b s = prev_img.at<cv::Vec3b>(i, j);
                    t_R.push_back(s[0]); t_G.push_back(s[1]); t_B.push_back(s[2]);
                }
                R.push_back(t_R); G.push_back(t_G); B.push_back(t_B);
            }
            
            ave_img = prev_img.clone();//1フレーム目を平均画像の最初として保存
            
            cv::resize(prev_img, prev_img, cv::Size(), reduction_rate, reduction_rate);//画像のリサイズ
            
            cv::cvtColor(prev_img, prev_gray, cv::COLOR_BGR2GRAY);//グレースケール画像を生成
            
            cv::medianBlur(prev_gray, prev_gray, 5);//ノイズ除去
            
            first_processing = false;//最初の処理の終了
            
        }else{
            //2フレーム目以降の処理
            curr_img = frame_img(select_rect);//画像の切り抜き
            
            tmp_recode_timing.service_timing_img = curr_img.clone();//現画像
            
            //ここで平均画像生成用にR,G,B値を計算する（関数にしたところ処理速度が落ちたためここに記述）
            for(int i=0; i<curr_img.rows; i++){
                for(int j=0; j<curr_img.cols; j++){
                    cv::Vec3b s = curr_img.at<cv::Vec3b>(i, j);
                    R[i][j] += s[0]; G[i][j] += s[1]; B[i][j] += s[2];
                    s[0] = R[i][j]/(unsigned int)(frame_num/3); s[1] = G[i][j]/(unsigned int)(frame_num/3); s[2] = B[i][j]/(unsigned int)(frame_num/3);
                    ave_img.at<cv::Vec3b>(i, j) = s;
                }
            }
            
            cv::resize(curr_img, curr_img, cv::Size(), reduction_rate, reduction_rate);//画像のリサイズ
            
            op_img = curr_img.clone();//オプティカルフロー用画像をクローン
            
            cv::cvtColor(curr_img, curr_gray, cv::COLOR_BGR2GRAY);//グレースケール化
            
            cv::medianBlur(curr_gray, curr_gray, 5);//ノイズの除去
            
            cv::goodFeaturesToTrack(prev_gray, priorFeature, 1000, 0.01, 10);//特徴点の抽出
            
            int opCnt = (int)priorFeature.size(); //特徴点数
            
            if (opCnt>0) { //特徴点が存在する場合
                //前フレームの特徴点"priorFeature"から，対応する現フレームの特徴点"presentFeature"を検出
                cv::calcOpticalFlowPyrLK(curr_gray, prev_gray, priorFeature, presentFeature,
                                         status, errors, cv::Size(10,10), 4, criteria);
                //オプティカルフロー描画
                for(int i=0; i<opCnt; i++){
                    cv::Point pt1 = cv::Point(priorFeature[i]); //前フレーム特徴点
                    cv::Point pt2 = cv::Point(presentFeature[i]); //現フレーム特徴点
                    
                    sum_dis += calcDistance(pt1, pt2);//特徴点の移動量の総和を計算
                    
                }
            }
            
            if(sum_dis == 0){
                
                tmp_recode_timing.service_timing = frame_num/fps;//換算された時間の保存
                
                record_timing.push_back(tmp_recode_timing);//サーブ時間とフレーム画像の保存
                
            }else{
                
                sum_dis = 0;//特徴点の移動量の初期化
                
            }
            
            prev_img = curr_img.clone();//現フレームを前フレームとして登録
            prev_gray = curr_gray.clone();//現フレームを前フレームとして登録
            
        }
        cv::destroyAllWindows();//ウィンドウの消去
    }
    
    DeleteWasteInfo detectWaste;//重複削除関数用インストラクタ
    PrintResultTime printResultTime;//時間印字関数用インストラクタ
    
    detectWaste.deleteMinimam(record_timing);//短い時間は除去する関数
    
    detectWaste.deleteAdjacentTimeFrames(record_timing);//近すぎる時間の排除
    
    std::cout << "記録処理の開始" << std::endl;
    
    //平均画像との差分の平均値を求める
    for(int i=0; i<record_timing.size(); i++){
        cv::Mat dst_img, gray_img, thresh_img;//画像のインスタンス
        cv::absdiff(record_timing[i].service_timing_img, ave_img, dst_img);//差分の絶対値を計算
        cv::cvtColor(dst_img, gray_img, cv::COLOR_BGR2GRAY);//グレースケール化
        cv::adaptiveThreshold(gray_img, thresh_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 21, 9);//適応二値化処理
        ave+=cv::countNonZero(thresh_img);//0以外の合計値をプラス
    }
    
    ave = ave/record_timing.size();//平均画像との差分の平均値を算出
    
    for(int i=0; i<record_timing.size(); i++){
        cv::Mat dst_img, gray_img, thresh_img;//画像のインスタンス
        cv::absdiff(record_timing[i].service_timing_img, ave_img, dst_img);//差分の絶対値を計算
        cv::cvtColor(dst_img, gray_img, cv::COLOR_BGR2GRAY);//グレースケール化
        cv::adaptiveThreshold(gray_img, thresh_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 21, 9);//適応二値化処理
        if(cv::countNonZero(thresh_img) > ave){//平均画像との差分は平均値以上の場合
            detect_time_out.push_back(record_timing[i]);//サーブ時間の保存
        }
    }
    record_timing = detect_time_out;//保存されたサーブ時間の更新
    
    detectWaste.deleteNearTimeFrames(record_timing);//近すぎる時間の排除
    
    printResultTime.printServiceTimimg(record_timing);//時間の印字
    
}

double DetectService::calcDistance(cv::Point2f pt1, cv::Point2f pt2){
    return (sqrt(pow((pt1.x - pt2.x), 2)+pow((pt1.y - pt2.y), 2)));//二点間の距離を返す
}

