#include "MultiTracker.h"

#include "utils.hpp"

MultiTracker::MultiTracker() {
    // trackers_.push_back(new TemplateTracker());
    // trackers_.push_back(new KltTracker());
}

// void MultiTracker::capture(const cv::Mat &frame, const cv::Rect &capture_box){
    // face_size_ = capture_box.size();

    // for (BaseTracker *tracker : trackers_)
        // tracker->capture(frame, capture_box);
// }

// std::pair<float, cv::Rect> MultiTracker::__track(const cv::Mat &frame){
    // std::vector<std::tuple<Tracker*, TrackerOut>> results;

    // for (Tracker *tracker : trackers_)
    //     results.push_back(std::make_tuple(tracker, tracker->track(frame)));

    // std::vector<Tracker*> for_recapture;
    // std::vector<TrackerOut> ok_trackers;

    // for (const std::tuple<Tracker*, TrackerOut> &out: results) {
    //     if (std::get<1>(out).confidence < 0.8) // TODO: -> const
    //         for_recapture.push_back(std::get<0>(out));
    //     else
    //         ok_trackers.push_back(std::get<1>(out));
    // }

    // if (ok_trackers.size() == 0)
    // return std::make_pair(0.0, cv::Rect());

    // TrackerOut accumulated = this->accumulate_trackers(ok_trackers);

    // for (Tracker *tracker: for_recapture)
    //     tracker->capture(frame, accumulated.rect);

    // return accumulated;
// }

// TrackerOut MultiTracker::accumulate_trackers(const std::vector<TrackerOut> &outs) {
//     double confidence = 0.0;
//     cv::Point tl;
//     int width = 0;
//     int height = 0;

//     for (const TrackerOut &out: outs) {
//         confidence += out.confidence;
//         tl += out.rect.tl();
//         width += out.rect.width;
//         height += out.rect.height;
//     }

//     confidence /= float(outs.size());
//     tl /= float(outs.size());
//     width /= outs.size();
//     height /= outs.size();

//     return TrackerOut(cv::Rect(tl.x, tl.y, width, height), confidence);
// }

