#ifndef KLT_TRACKER_H
#define KLT_TRACKER_H

#include <opencv2/opencv.hpp>

#include "Tracker.hpp"

inline
cv::Point average_point(const std::vector<cv::Point>& points) {
    int sum_x = 0, 
        sum_y = 0;

    for (const cv::Point& p: points) {
        sum_x += p.x; 
        sum_y += p.y;
    }

    return cv::Point(sum_x / points.size(), sum_y / points.size());
}

inline
cv::Rect rect_around_point(const cv::Point& point, const cv::Size& size, const cv::Size& bounds) {
    cv::Rect rect(point.x - size.width/2, point.y - size.height/2, size.width, size.height);

    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.x + rect.width  > bounds.width ) rect.width  = bounds.width  - rect.x - 1;
    if (rect.y + rect.height > bounds.height) rect.height = bounds.height - rect.y - 1;

    return rect.width > 0 && rect.height > 0 ? rect : cv::Rect();
}

inline
cv::Rect rect_around_points(const std::vector<cv::Point>& points, const cv::Size& size, const cv::Size& bounds) {
    return rect_around_point(average_point(points), size, bounds);
}

namespace tracking {

class KltTrackerParams {
public:
    float capture_crop_factor = 0.1;
    int max_corners = 100;
    double quality_level = 0.0003;
    double min_distance = 3;
    int block_size = 7;

    cv::Size win_size = cv::Size(15, 15);
    int max_level = 2;

    float find_area = 0.7; 
    float fall_area = 1.5;

    int max_error = 550;

    const cv::TermCriteria term_criteria = cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.003);
};

const int KLT_CORNER_STATUS_OK = 1;
const int KLT_CORNER_STATUS_FALLED = 0;

class KltTracker : public BaseTracker {
public:
    explicit KltTracker(const TermCriteria& term_criteria): 
        BaseTracker(term_criteria) {
    } 

    void capture(const cv::Mat& frame, const cv::Rect& box) override;
    
private:
    KltTrackerParams tracking_params;

    cv::Mat old_frame;
    cv::Size object_size;
    cv::Size frame_size;
    cv::Point object_center;
    std::vector<cv::Point> corners;

    // Track methods //
    std::pair<float, cv::Rect> __track(const cv::Mat &frame) override;

    std::pair<std::vector<int>, std::vector<int>> __calc_optical_flow(const cv::Mat& frame, std::vector<cv::Point>& new_corners);
    std::vector<cv::Point> __update_corners(const cv::Mat& frame);
    void __fall_scattered_corners();
    void __find_corners(const cv::Mat& img, std::vector<cv::Point>& corners, const cv::Rect& roi_rect = cv::Rect());
    std::pair<cv::Mat, cv::Rect> __crop_capture_frame(const cv::Mat& frame, const cv::Rect& captureBox);
};

} // namespace tracking

#endif /* KLT_TRACKER_H */


