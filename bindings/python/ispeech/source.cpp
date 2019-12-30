#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "../../../src/ispeech/ispeech.h"

namespace py = pybind11;
using namespace py::literals;
using namespace vfx_ispeech;

class Source {
public:
  Source(const std::string &name) : name(name) { }
  void setName(const std::string &name_) { name = name_; }
  const std::string &getName() const {
    return name;
  }
  std::string name;
};

class PyDog : public Dog {
public:
    /* Inherit the constructors */
    using Dog::Dog;

    /* Trampoline (need one for each virtual function) */
    void  generate(std::size_t samplesCount) override {
        PYBIND11_OVERLOAD_PURE(
            void, /* Return type */
            Dog,      /* Parent class */
            generate,          /* Name of function in C++ (must match Python name) */
            samplesCount      /* Argument(s) */
        );
    }
};

class PyGenerator : public Generator {
public:
    /* Inherit the constructors */
    using Generator::Generator;

    /* Trampoline (need one for each virtual function) */
    void  generate(std::size_t samplesCount) override {
        PYBIND11_OVERLOAD_PURE(
            void, /* Return type */
            Generator,      /* Parent class */
            generate,          /* Name of function in C++ (must match Python name) */
            samplesCount      /* Argument(s) */
        );
    }
};


PYBIND11_MODULE(source, m) {
  py::class_<Source>(m, "Source",py::dynamic_attr())
    .def(py::init<const std::string &>())
    .def("setName", &Source::setName)
    .def("getName", &Source::getName)
    .def_readwrite("name",&Source::name);

  py::class_<Pet>(m, "Pet")
    .def(py::init<vfx_ispeech::FrequencyType>())
    .def_readwrite("name", &Pet::name);

  // Method 1: template parameter:
  py::class_<Dog, Pet,PyDog>(m, "Dog")
    .def(py::init<vfx_ispeech::FrequencyType>())
    .def("bark", &Dog::bark)
    .def("generate", &Dog::generate)
    ;
 

  //SignalSource
  py::class_<SignalSource>(m, "SignalSource")
    .def(py::init<vfx_ispeech::FrequencyType>())
    .def(py::init<std::size_t>())
    .def(py::init<std::size_t>())
    .def("__getitem__", [](const SignalSource &s, size_t i) {
			  if (i >= s.size()) throw py::index_error();
			  return s.sample(i);
			})    
    .def("__len__", &SignalSource::size)
    .def("__iter__", [](const SignalSource &s) { return py::make_iterator(s.begin(), s.end()); },py::keep_alive<0, 1>() )
    .def("shiftLeft",&SignalSource::shiftLeft)
    .def("shiftRight",&SignalSource::shiftRight)
    .def("append",&SignalSource::append)
    .def("appendHead",&SignalSource::appendHead)
    .def("set",&SignalSource::set)
    ;


  //Generator
  py::class_<Generator,SignalSource,PyGenerator>(m, "Generator")
    .def(py::init<vfx_ispeech::FrequencyType>())
    .def("setFrequency", &Generator::setFrequency)
    .def("setAmplitude", &Generator::setAmplitude)
    .def("setPhase", &Generator::setPhase)
    .def("generate", &Generator::generate);

  
  py::class_<SineGenerator,Generator>(m, "SineGenerator")
    .def(py::init<double>())
    .def("generate", &SineGenerator::generate);

  //Window
  py::class_<HammingWindow,SignalSource>(m,"HammingWindow")
    .def(py::init<std::size_t>());

  py::class_<HannWindow,SignalSource>(m,"HannWindow")
    .def(py::init<std::size_t>());

  py::class_<GaussianWindow,SignalSource>(m,"GaussianWindow")
    .def(py::init<std::size_t,double>());

  py::class_<FlattopWindow,SignalSource>(m,"FlattopWindow")
    .def(py::init<std::size_t>());

  py::class_<BlackmanWindow,SignalSource>(m,"BlackmanWindow")
    .def(py::init<std::size_t>());

  py::class_<BarlettWindow,SignalSource>(m,"BarlettWindow")
    .def(py::init<std::size_t>());
  py::class_<TriangWindow,SignalSource>(m,"TriangWindow")
    .def(py::init<std::size_t>());

  
  py::class_<BlackmanharrisWindow,SignalSource>(m,"BlackmanharrisWindow")
    .def(py::init<std::size_t>());
  
  
}
