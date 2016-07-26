#include "TemplateTracker.h"

#include <utility>

#include "utils.hpp"

namespace tracking {

void TemplateTracker::capture(const cv::Mat &frame, const cv::Rect &rect){
    curr_area_ = rect;
    obj_templ_ = frame(rect);
}

std::pair<float, cv::Rect> TemplateTracker::__track(const cv::Mat &frame){
    cv::Mat tracking_area_frame;
    cv::Point track_area_tl;

    std::tie(tracking_area_frame, track_area_tl) = get_current_detect_area(frame);

    cv::Rect track_area(track_area_tl.x, track_area_tl.y, tracking_area_frame.rows, tracking_area_frame.cols); //TODO : check if with and height on the right places

    std::vector<cv::Mat> object_frames = {obj_templ_};

    for (float scale : INCREASE_SCALES_){
        cv::Mat tmp;

        cv::resize(obj_templ_, tmp, cv::Size(0, 0), 1.0 + scale, 1.0 + scale);
        object_frames.push_back(tmp);
        cv::resize(obj_templ_, tmp, cv::Size(0, 0), 1.0 - scale, 1.0 - scale);
        object_frames.push_back(tmp);
    }

    std::vector <cv::Mat> match_results;

    for (auto obj_frame : object_frames){
        cv::Mat result;
        cv::matchTemplate(tracking_area_frame, obj_frame, result, TEMPLATE_METHOD_);
        match_results.push_back(result);
    }

    std::vector<double> min_vals;
    std::vector<std::tuple<double, cv::Point>> min_max_values;

    for (cv::Mat match: match_results){
        double    minVal;
        double    maxVal;
        cv::Point minLoc;
        cv::Point maxLoc;

        cv::minMaxLoc(match, &minVal, &maxVal, &minLoc, &maxLoc);

        min_max_values.push_back(std::make_tuple(minVal, minLoc));
        min_vals.push_back(minVal);
    }

    int best_area_index = find(min_vals.begin(), min_vals.end(), *(std::minmax_element(min_vals.begin(), min_vals.end()).first)) - min_vals.begin();

    cv::Point best_pos;
    double best_val;

    std::tie(best_val, best_pos) = min_max_values[best_area_index];

    obj_templ_ = object_frames[best_area_index];
    curr_area_ = cv::Rect(track_area.x + best_pos.x, track_area.y + best_pos.y, obj_templ_.cols, obj_templ_.rows);

    adjust_object_template(frame);

    return std::make_pair(1.0 - best_val, curr_area_);
}

void TemplateTracker::increase(cv::Rect &rect) {
    int newWidth = rect.width * TRACK_AREA_SCALE_FACTOR_;
    int newHeight = rect.height * TRACK_AREA_SCALE_FACTOR_;

    rect.x -= (newWidth - rect.width) / 2;
    rect.y -= (newHeight - rect.height) / 2;

    rect.width = newWidth;
    rect.height = newHeight;
}

std::tuple<cv::Mat, cv::Point> TemplateTracker::get_current_detect_area(const cv::Mat &frame){
    cv::Rect search_area(curr_area_);
    increase(search_area);
    clip(search_area, frame);

    return std::make_tuple(frame(search_area), cv::Point(search_area.x, search_area.y));
}

void TemplateTracker::adjust_object_template(const cv::Mat &frame){
    obj_templ_ = (1.0 - TEMPL_ADJUST_COEFF_) * obj_templ_ + TEMPL_ADJUST_COEFF_ * frame(curr_area_);
}

} // namespace tracking
