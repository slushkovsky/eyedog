#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/opencv.hpp>

template<typename T>
T pointsDistance(const cv::Point_<T> &p1, const cv::Point_<T> &p2){
    return std::sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
}

template<typename T>
cv::Point_<T> operator/(const cv::Point_<T>& p, double divisor){
    return cv::Point_<T>(p.x / divisor, p.y / divisor);
}

inline cv::Rect operator*(const cv::Rect& rect, double mul) {
    return cv::Rect(rect.x, rect.y, rect.width*mul, rect.height*mul);
}

template<typename T>
cv::Point_<T>& operator/=(cv::Point_<T>& p, T divisor){
    p.x /= divisor;
    p.y /= divisor;
    return p;
}

inline cv::Rect& operator*=(cv::Rect& rect, double mul){
    rect.width  *= mul;
    rect.height *= mul;

    return rect;
}

inline cv::Size operator*(const cv::Size& size, double mul){
    return cv::Size(size.width*mul, size.height * mul);
}

inline void clip(cv::Rect &rect, const cv::Mat &img){ // TODO: Inline is not about this function
    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.x > img.cols) rect.x = img.cols - 1;
    if (rect.y > img.rows) rect.y = img.rows - 1;
    if (rect.x + rect.width  > img.cols) rect.width  = img.cols - rect.x;
    if (rect.y + rect.height > img.rows) rect.height = img.rows - rect.y;
}

inline void clip(cv::Point &point, const cv::Mat &img){
    if (point.x < 0) point.x = 0;
    if (point.y < 0) point.y = 0;
    if (point.x > img.cols) point.x = img.cols - 1;
    if (point.y > img.rows) point.y = img.rows - 1;
}

#endif
