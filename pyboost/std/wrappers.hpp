#include <boost/python.hpp>
#include <vector>

template <class T>
boost::python::list std_vector_to_py_list(const std::vector<T>& vec) {
    boost::python::list l;

    for (auto value : vec)
    	l.append(value);
    
    return l;
}