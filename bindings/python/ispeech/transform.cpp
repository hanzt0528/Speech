#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/complex.h>
#include <pybind11/numpy.h>
#include "../../../src/ispeech/ispeech.h"
#include "../../../src/algorithms/karaoke/midi_parser.h"
#include "../../../src/algorithms/karaoke/karaoke.h"

#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

namespace py = pybind11;
using namespace py::literals;
using namespace vfx_ispeech;

class Transform {
public:
  Transform(const std::string &name) : name(name){ }
  void setName(const std::string &name_) { name = name_; }
  const std::string &getName() const {
    return name;
  }

  SpectrumType fft()
  {
    int N = 10;
    double* a = new double[2 * N];
    for (std::size_t i = 0; i < N; ++i)
      {
	a[2 * i] = 10;
	a[2 * i + 1] = 5.0;
      }

    // convert the array back to complex values and return as vector
    ComplexType* tmpPtr = reinterpret_cast<ComplexType*>(a);
    SpectrumType spectrum(tmpPtr, tmpPtr + N);
    delete [] a;

    return spectrum;

  }

  /*
  virtual const SampleType* toArray()
  {
    std::size_t size = 128;

    m_data.reserve(size);
    for(int i = 0; i < size; i++)
      {
	m_data[i] = i+5;
      }
    return &m_data[0];

  }
  */
  std::vector<SampleType> toArray()
  {
    /*
    std::size_t size = 128;

    m_data.reserve(size);
    for(int i = 0; i < size; i++)
      {
	m_data[i] = i+5;
      }
    return &m_data;
    */

    int N = 128;
    double* a = new double[N];
    for (std::size_t i = 0; i < N; ++i)
      {
	a[i] = i;
      }

    // convert the array back to complex values and return as vector
    double* tmpPtr = reinterpret_cast<double*>(a);
    std::vector<SampleType> spectrum(tmpPtr, tmpPtr + N);
    delete [] a;

    return spectrum;

  }

  SpectrumType fft2(std::vector<SampleType> x)
  {
    int N = 128;
    double* a = new double[2 * N];
    for (std::size_t i = 0; i < N; ++i)
      {
	a[2 * i] = x[i];
	fprintf(stderr,"x[%ld]= %f\n",i,x[i]);

	a[2 * i + 1] = 5.0;
      }


    // convert the array back to complex values and return as vector
    ComplexType* tmpPtr = reinterpret_cast<ComplexType*>(a);
    SpectrumType spectrum(tmpPtr, tmpPtr + N);
    delete [] a;

    return spectrum;
  }

  std::vector<SampleType> m_data;
  std::string name;
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


class PyFft : public Fft {
public:
    /* Inherit the constructors */
    using Fft::Fft;

    /* Trampoline (need one for each virtual function) */
    SpectrumType  fft(const SampleType x[]) override {
	PYBIND11_OVERLOAD_PURE(
	    SpectrumType, /* Return type */
	    Fft,      /* Parent class */
	    fft,          /* Name of function in C++ (must match Python name) */
	    (double*)x      /* Argument(s) */
	);
    }

    void  ifft(SpectrumType spectrum, double x[]) override {
	PYBIND11_OVERLOAD_PURE(
	    void, /* Return type */
	    Fft,      /* Parent class */
	    ifft,          /* Name of function in C++ (must match Python name) */
	    spectrum,      /* Argument(s) */
	    x
	);
    }


};


PYBIND11_MODULE(transform, m) {
  py::class_<Transform>(m, "Transform",py::dynamic_attr())
    .def(py::init<const std::string &>())
    .def("setName", &Transform::setName)
    .def("getName", &Transform::getName)
    .def_readwrite("name",&Transform::name)
    .def("fft",&Transform::fft)
    .def("toArray",&Transform::toArray)
    .def("fft2",&Transform::fft2,py::return_value_policy::reference);

  py::class_<Fft,PyFft>(m, "Fft")
    .def(py::init<std::size_t>());

  py::class_<OouraFft,Fft>(m,"OouraFft")
    .def(py::init<std::size_t>())
    .def("fft",&OouraFft::fft2)
    .def("ifft",&OouraFft::ifft2,py::return_value_policy::reference);

  py::class_<Dft,Fft>(m,"Dft")
    .def(py::init<std::size_t>())
    .def("fft",&Dft::fft2)
    .def("ifft",&Dft::ifft2,py::return_value_policy::reference);


  py::class_<AnalysisType>(m, "AnalysisType")
    .def_readwrite("mX", &AnalysisType::mX,py::return_value_policy::reference)
    .def_readwrite("pX", &AnalysisType::pX,py::return_value_policy::reference);

  py::class_<SineTrackingType>(m, "SineTrackingType")
    .def_readwrite("tfreqn", &SineTrackingType::tfreqn,py::return_value_policy::reference)
    .def_readwrite("tmagn", &SineTrackingType::tmagn,py::return_value_policy::reference)
    .def_readwrite("tphasen", &SineTrackingType::tphasen,py::return_value_policy::reference);

  py::class_<SoundDescriptorType>(m, "SoundDescriptorType")
    .def_readwrite("pitch", &SoundDescriptorType::pitch)
    .def_readwrite("energy", &SoundDescriptorType::energy)
    .def_readwrite("rms", &SoundDescriptorType::rms)
    .def_readwrite("power", &SoundDescriptorType::power)
    .def_readwrite("start", &SoundDescriptorType::start)
    .def_readwrite("end", &SoundDescriptorType::end)
    ;

  py::class_<MIDIDescriptorType>(m, "MIDIDescriptorType")
    .def_readwrite("note", &MIDIDescriptorType::note)
    .def_readwrite("start", &MIDIDescriptorType::start)
    .def_readwrite("end", &MIDIDescriptorType::end)
    ;

  py::class_<ModelDFT>(m,"ModelDFT")
    .def(py::init())
    .def("dftModel",&ModelDFT::dftModel,py::return_value_policy::reference)
    .def("Analysis",&ModelDFT::Analysis,py::return_value_policy::reference)
    .def("Synthesis",&ModelDFT::Synthesis,py::return_value_policy::reference)
    ;

  py::class_<ModelSine>(m,"ModelSine")
    .def(py::init())
    .def("sineModel",&ModelSine::sineModel,py::return_value_policy::reference)
    .def("Analysis",&ModelSine::Analysis,py::return_value_policy::reference)
    .def("Synthesis",&ModelSine::Synthesis,py::return_value_policy::reference)
    .def("GetAnalysisTypeCount",&ModelSine::GetAnalysisTypeCount)
    .def("GetAnalysisTypeByIndex",&ModelSine::GetAnalysisTypeByIndex,py::return_value_policy::reference)
    .def("AnalysisAndSynthesis",&ModelSine::AnalysisAndSynthesis,py::return_value_policy::reference)
    ;

  py::class_<ModelSTFT>(m,"ModelSTFT")
    .def(py::init())
    .def("Analysis",&ModelSTFT::Analysis,py::return_value_policy::reference)
    .def("Synthesis",&ModelSTFT::Synthesis,py::return_value_policy::reference)
    .def("GetAnalysisTypeCount",&ModelSTFT::GetAnalysisTypeCount)
    .def("GetAnalysisTypeByIndex",&ModelSTFT::GetAnalysisTypeByIndex,py::return_value_policy::reference)
    .def("AnalysisAndSynthesis",&ModelSTFT::AnalysisAndSynthesis,py::return_value_policy::reference)
    ;

  py::class_<midi_parser>(m,"midi_parser")
    .def(py::init())
    .def("parser2",&midi_parser::parser2,py::return_value_policy::reference)
    ;

  py::class_<PitchDetection>(m,"PitchDetection")
    .def(py::init<std::size_t>())
    .def("Detect2",&PitchDetection::Detect2)
    ;

  py::class_<karaoke>(m,"karaoke")
    .def(py::init())
    .def("set",&karaoke::set)
    .def("getwav",&karaoke::getwav,py::return_value_policy::reference)
    .def("getmidi",&karaoke::getmidi,py::return_value_policy::reference)
    .def("getscore",&karaoke::getscore)
    ;

  py::class_<HarmonicsType>(m, "HarmonicsType")
    .def_readwrite("hfreq", &HarmonicsType::hfreq,py::return_value_policy::reference)
    .def_readwrite("hmag", &HarmonicsType::hmag,py::return_value_policy::reference)
    .def_readwrite("hphase", &HarmonicsType::hphase,py::return_value_policy::reference);


  py::class_<ModelHarmonic>(m,"ModelHarmonic")
    .def(py::init())
    .def("f0Detection",&ModelHarmonic::f0Detection,py::return_value_policy::reference)
    .def("Analysis",&ModelHarmonic::Analysis,py::return_value_policy::reference)
    .def("GetAnalysisTypeCount",&ModelHarmonic::GetAnalysisTypeCount)
    .def("GetAnalysisTypeByIndex",&ModelHarmonic::GetAnalysisTypeByIndex,py::return_value_policy::reference)
    ;
}
