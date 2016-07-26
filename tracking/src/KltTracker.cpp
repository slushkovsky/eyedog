#include "KltTracker.h"

#include "utils.hpp"

namespace tracking {

void KltTracker::capture(const cv::Mat& frame, const cv::Rect& box) {
    if (frame.type() == CV_8U)
        this->old_frame = frame.clone();
    else
        cv::cvtColor(frame, this->old_frame, CV_BGR2GRAY);

    this->frame_size = frame.size();

    this->object_center = cv::Point(box.x + box.width/2, box.y + box.height/2);

    cv::Mat object_image; 
    cv::Rect object_rect;

    std::tie(object_image, object_rect) = this->__crop_capture_frame(this->old_frame, box);
    this->object_size = object_rect.size();
    
    this->__find_corners(object_image, this->corners, object_rect);
}

std::pair<float, cv::Rect> KltTracker::__track(const cv::Mat &orig_frame) {
    cv::Mat frame; 

    if (frame.type() != CV_8U)
        cv::cvtColor(orig_frame, frame, CV_BGR2GRAY);
    else 
        frame = orig_frame.clone();

    std::vector<cv::Point> new_corners = this->__update_corners(frame);
    
    std::vector<int> status, 
                     error;

    std::tie(status, error) = this->__calc_optical_flow(frame, new_corners);

    cv::Point flow;
    int active_corners = 0; 

    for (uint i = 0; i < status.size(); ++i)
        if (status[i] != KLT_CORNER_STATUS_FALLED || error[i] < this->tracking_params.max_error) {  
            flow += new_corners[i] - this->corners[i];
            ++active_corners;
        }

    this->object_center += flow / active_corners;

    this->old_frame = frame; 
    this->corners = new_corners;

    this->__fall_scattered_corners();

    return std::make_pair(1.0, rect_around_point(this->object_center, this->object_size, frame.size()));
}

void KltTracker::__find_corners(const cv::Mat& img, std::vector<cv::Point>& corners, const cv::Rect& roi_rect) {
    goodFeaturesToTrack(img, corners, this->tracking_params.max_corners, this->tracking_params.quality_level, 
                                      this->tracking_params.min_distance, cv::Mat(), this->tracking_params.block_size, true);

    if (roi_rect != cv::Rect())
        for (cv::Point& corner: corners)
            corner += roi_rect.tl();
}

std::vector<cv::Point> KltTracker::__update_corners(const cv::Mat& frame) {
    cv::Rect find_area = rect_around_points(this->corners, object_size * this->tracking_params.find_area, this->frame_size);

    if (find_area == cv::Rect())
        return std::vector<cv::Point>();

    std::vector<cv::Point> new_corners;

    this->__find_corners(frame(find_area), new_corners, find_area);

    // std::move(new_corners.begin(), new_corners.end(), std::back_inserter(this->corners))

    return new_corners;
}

void KltTracker::__fall_scattered_corners() {
    cv::Rect true_area = rect_around_points(this->corners, object_size * this->tracking_params.fall_area, this->frame_size);

    for (auto it = this->corners.begin(); it != this->corners.end();)
        it = true_area.contains((*it)) ? ++it : this->corners.erase(it);
}

std::pair<std::vector<int>, std::vector<int>> KltTracker::__calc_optical_flow(const cv::Mat& frame, std::vector<cv::Point>& new_corners) {
    std::vector<int> status, 
                     error;

    calcOpticalFlowPyrLK(this->old_frame, frame, this->corners, new_corners, status, error, this->tracking_params.win_size, 
                         this->tracking_params.max_level, this->tracking_params.term_criteria);

    return std::make_pair(status, error);
}

std::pair<cv::Mat, cv::Rect> KltTracker::__crop_capture_frame(const cv::Mat& frame, const cv::Rect& captureBox) {
    cv::Rect crop_rect((1 + this->tracking_params.capture_crop_factor) * captureBox.x, 
                       (1 + this->tracking_params.capture_crop_factor) * captureBox.y,
                       (1 - 2 * this->tracking_params.capture_crop_factor) * captureBox.width, 
                       (1 - 2 * this->tracking_params.capture_crop_factor) * captureBox.height);

    if (crop_rect.x + crop_rect.width > frame.cols)
        crop_rect.width = frame.cols - crop_rect.x;

    if (crop_rect.y + crop_rect.height > frame.rows)
        crop_rect.height = frame.rows - crop_rect.y;

    return std::make_pair(frame(crop_rect), crop_rect);
}

} // namespace tracking