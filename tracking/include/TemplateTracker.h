#ifndef TEMPLATE_TRACKER_H
#define TEMPLATE_TRACKER_H

#include <tuple>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Tracker.hpp"

namespace tracking {

class TemplateTracker : public BaseTracker {
public:
    // explicit TemplateTracker(const TermCriteria& term_criteria) : 
        // BaseTracker(term_criteria){
    // }

    TemplateTracker(float max_confidence_eps, int obj_min_width, int obj_min_height): 
        BaseTracker(TermCriteria(max_confidence_eps, cv::Size(obj_min_width, obj_min_height))) {
    }

    void capture(const cv::Mat &frame, const cv::Rect &rect) override;
    
private:
    const float TRACK_AREA_SCALE_FACTOR_ = 1.33;
    const int TEMPLATE_METHOD_ = cv::TM_SQDIFF_NORMED;
    const float TEMPL_ADJUST_COEFF_ = 0.025;
    const std::vector<float> INCREASE_SCALES_ = { 0.05 };
    cv::Mat obj_templ_;
    cv::Rect curr_area_;

    std::pair<float, cv::Rect> __track(const cv::Mat &frame) override;

    void increase(cv::Rect &rect);
    void adjust_object_template(const cv::Mat &frame);
    std::tuple<cv::Mat, cv::Point> get_current_detect_area(const cv::Mat &frame);
};
 
} // namespace tracking

#endif /* TEMPLATE_TRACKER_H */
