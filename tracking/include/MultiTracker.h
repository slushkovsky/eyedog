#ifndef MULTI_TRACKER_H
#define MULTI_TRACKER_H

#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>

#include "Tracker.hpp"
#include "KltTracker.h"
#include "TemplateTracker.h"

class MultiTracker 
{
public:
    MultiTracker();

    void capture(const cv::Mat &frame, const cv::Rect &capture_box) {
    };
    void track(const cv::Mat& frame) {

    }

private:
    // const float ACCURACY_BORDER_VALUE_ = 0.90;

    // std::vector<BaseTracker*> trackers_;
    // cv::Size face_size_;

    // std::pair<float, cv::Rect> __track(const cv::Mat &frame) override;

    // static std::pair<float, cv::Rect> accumulate_trackers(const std::vector<TrackerOut> &outs);
};

#endif /* MULTI_TRACKER_H */
