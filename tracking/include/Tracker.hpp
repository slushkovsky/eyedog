#ifndef TRACKER_HPP
#define TRACKER_HPP

#include <tuple>
#include <opencv2/opencv.hpp>

namespace tracking {

class TermCriteria {
public:
	TermCriteria(float max_confidence_eps, const cv::Size &min_object_size): 
		max_confidence_eps(max_confidence_eps), 
		min_object_size   (min_object_size) {
	}

	bool is_term(float confidence_eps, const cv::Size &object_size) {
		return confidence_eps     > this->max_confidence_eps    || 
		       object_size.width  < this->min_object_size.width || 
		       object_size.height < this->min_object_size.height;
	}

private: 
	float max_confidence_eps; 
	cv::Size min_object_size;
};


class BaseTracker {
public:
	BaseTracker(const TermCriteria &term_criteria): 
		term_criteria(term_criteria) {
	}

    cv::Rect track(const cv::Mat &frame) {
    	std::pair<float, cv::Rect> tracking_result = this->__track(frame.clone()); 

    	float confidence_eps = std::abs(tracking_result.first - this->last_confidence) / float(tracking_result.first); 

    	bool is_termination = this->term_criteria.is_term(confidence_eps, tracking_result.second.size());

    	this->last_confidence = tracking_result.first;

    	return is_termination ? cv::Rect() : tracking_result.second; 
    }

    virtual void capture(const cv::Mat &frame, const cv::Rect &capture_box) = 0;

protected: 
	TermCriteria term_criteria;
	float last_confidence = 1.0; 

	virtual std::pair<float, cv::Rect> __track(const cv::Mat &frame) = 0;
};


} // namesapce tracking


#endif /* TRACKER_HPP */
