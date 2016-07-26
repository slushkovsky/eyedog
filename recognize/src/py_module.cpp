#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#include <boost/python.hpp>
#include <opencv2/opencv.hpp>

#define CPU_ONLY
#include <caffe/caffe.hpp>

#include "pyboost/opencv/include/pyboostcvconverter.hpp"
#include "pyboost/std/wrappers.hpp"

#include "Classifier.hpp"

namespace py = boost::python;

py::list Classifier_predict(Classifier &self, const cv::Mat &img) {
    return std_vector_to_py_list(self.predict(img));
}

static int init_ar(){
    Py_Initialize();
    import_array();
}

BOOST_PYTHON_MODULE(_classifier){
    init_ar();

    // Initialize converters //
    py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
    pbcvt::matFromNDArrayBoostConverter();

    // Wraped classes //
    py::class_<Classifier>("Classifier", py::init<std::string, std::string, std::string>())
        .def("predict", &Classifier_predict)
        ;
}
