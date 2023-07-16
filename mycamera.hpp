#include <thread>
#include <functional>

#include <opencv2/opencv.hpp>

class Mycamera {
public:
    Mycamera() {
        search_device();

        cap_.open(1);
        if(!cap_.isOpened())//カメラデバイスが正常にオープンしたか確認．
        {
            is_camera_opend = false;
            //読み込みに失敗したときの処理
            std::cout << "camera error" << std::endl;
        }

        fps_ = cap_.get(cv::CAP_PROP_FPS);
        contrast_ = cap_.get(cv::CAP_PROP_CONTRAST);
        saturation_ = cap_.get(cv::CAP_PROP_SATURATION);
        gamma_ = cap_.get(cv::CAP_PROP_GAMMA);
        width_ = cap_.get(cv::CAP_PROP_FRAME_WIDTH);
        height_ = cap_.get(cv::CAP_PROP_FRAME_HEIGHT);

        std::cout << "fps:" << fps_ << " contrast_:" << contrast_ << " saturation_:" << saturation_ << " gamma_:" << gamma_ << std::endl;
        std::cout << "width_:" << width_ << " height_:" << height_ << std::endl;

        is_camera_opend = true;
        do_update_ = false;
    }
    ~Mycamera() {
        close_thread_ = true;
    }

    bool open(int device_id) {
        cap_.open(device_id);
        if(!cap_.isOpened())//カメラデバイスが正常にオープンしたか確認．
        {
            is_camera_opend = false;
            //読み込みに失敗したときの処理
            std::cout << "camera error" << std::endl;
        }

        is_camera_opend = true;
        do_update_ = false;
    }

    bool do_update() {
        return do_update_;
    }

    bool get_current_image(cv::Mat& image) {
        if (!do_update_) return false;

        if (count_ % 2 == 0) {
            image = pon_image_;
        }
        else {
            image = pin_image_;
        }
        do_update_ = false;

        return true; 
    }

    bool start() {
        // スレッド終了フラグのクリア
        close_thread_ = false;
        read_thread_ = std::thread(std::bind(&Mycamera::read_thread, this));

        return true;
    }

    bool stop() {
        close_thread_ = true;
        if (read_thread_.joinable()) {
            read_thread_.join();
        }

        return false;
    };

private:
    void search_device() {
        for(int i = 0; i < 10; i++) {
            cap_.open(i);
            if(!cap_.isOpened())//カメラデバイスが正常にオープンしたか確認．
            {
                //読み込みに失敗したときの処理
                std::cout << "camera can't opened" << std::endl;
                device_num_ = i;
                cap_.release();
                break;
            }
            cap_.release();
        }

        std::cout << "device_num_" << device_num_ << std::endl;
    }

    void read_thread() {
        count_ = 0;
        while(!close_thread_) {
            if (count_ % 2 == 0) {
                cap_.read(pin_image_);
            }
            else {
                cap_.read(pon_image_);
            }

            do_update_ = true;
            count_++;
        }

        std::cout << count_ << std::endl;
        cap_.release();
    }

    cv::Mat pin_image_;
    cv::Mat pon_image_;
    cv::VideoCapture cap_;
    bool do_update_;
    bool is_camera_opend;
    bool close_thread_;
    unsigned long count_;

    int fps_;
    int contrast_;
    int saturation_;
    int gamma_;
    int width_;
    int height_;

    int device_num_;

    /** readスレッドのスレッド変数 */
    std::thread read_thread_;
};