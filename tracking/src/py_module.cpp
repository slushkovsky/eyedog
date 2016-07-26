#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#include <boost/python.hpp>
#include <opencv2/opencv.hpp>

#include "pyboost/opencv/include/pyboostcvconverter.hpp"
#include "TemplateTracker.h"

namespace py = boost::python;

void TmplTracker_capture(tracking::TemplateTracker &self, const cv::Mat &frame, py::tuple capture_box) {
    return self.capture(frame, pbcvt::py_tuple_to_cvRect(capture_box));
}

py::list TmplTracker_track(tracking::TemplateTracker &self, const cv::Mat &frame) {
    py::list l; 

    cv::Rect track_result = self.track(frame);

    l.append(track_result.x);
    l.append(track_result.y);
    l.append(track_result.width);
    l.append(track_result.height);

    return l;
}

static int init_ar(){
    Py_Initialize();
    import_array();
}

BOOST_PYTHON_MODULE(_tracking){
    //using namespace XM;
    init_ar();

    // Initialize converters //
    py::to_python_converter<cv::Mat,
    pbcvt::matToNDArrayBoostConverter>();
    pbcvt::matFromNDArrayBoostConverter();

    // Wraped classes //
    py::class_<tracking::TemplateTracker>("TemplateTracker", py::init<float, int, int>())
        .def("capture", &TmplTracker_capture)
        .def("track", &TmplTracker_track)
        ;
}
