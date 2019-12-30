/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

namespace py = pybind11;
using namespace py::literals;

class Pet {
public:
  Pet(const std::string &name) : name(name) { }
  void setName(const std::string &name_) { name = name_; }
  const std::string &getName() const {
    return name;
  }
  std::string name;
};
class Sequence {
public:
  Sequence(size_t size) : m_size(size) {
    m_data = new float[size];
    memset(m_data, 0, sizeof(float) * size);
  }
  Sequence(const std::vector<float> &value) : m_size(value.size()) {
    m_data = new float[m_size];
    memcpy(m_data, &value[0], sizeof(float) * m_size);
  }
  Sequence(const Sequence &s) : m_size(s.m_size) {
    m_data = new float[m_size];
    memcpy(m_data, s.m_data, sizeof(float)*m_size);
  }
  Sequence(Sequence &&s) : m_size(s.m_size), m_data(s.m_data) {
    s.m_size = 0;
    s.m_data = nullptr;
  }

  ~Sequence()
  {
    delete[] m_data;
  }

  Sequence &operator=(const Sequence &s) {
    if (&s != this) {
      delete[] m_data;
      m_size = s.m_size;
      m_data = new float[m_size];
      memcpy(m_data, s.m_data, sizeof(float)*m_size);
    }
    return *this;
  }

  Sequence &operator=(Sequence &&s) {
    if (&s != this) {
      delete[] m_data;
      m_size = s.m_size;
      m_data = s.m_data;
      s.m_size = 0;
      s.m_data = nullptr;
    }
    return *this;
  }

  bool operator==(const Sequence &s) const {
    if (m_size != s.size()) return false;
    for (size_t i = 0; i < m_size; ++i)
      if (m_data[i] != s[i])
	return false;
    return true;
  }
  bool operator!=(const Sequence &s) const { return !operator==(s); }

  float operator[](size_t index) const { return m_data[index]; }
  float &operator[](size_t index) { return m_data[index]; }

  bool contains(float v) const {
    for (size_t i = 0; i < m_size; ++i)
      if (v == m_data[i])
	return true;
    return false;
  }

  Sequence reversed() const {
    Sequence result(m_size);
    for (size_t i = 0; i < m_size; ++i)
      result[m_size - i - 1] = m_data[i];
    return result;
  }

  size_t size() const { return m_size; }

  const float *begin() const { return m_data; }
  const float *end() const { return m_data+m_size; }

private:
  size_t m_size;
  float *m_data;
};


PYBIND11_MODULE(_common, m) {
  py::class_<Pet>(m, "Pet",py::dynamic_attr())
    .def(py::init<const std::string &>())
    .def("setName", &Pet::setName)
    .def("getName", &Pet::getName)
    .def_readwrite("name",&Pet::name);

  py::class_<Sequence>(m, "Sequence")
    .def(py::init<size_t>())
    .def(py::init<const std::vector<float>&>())
    /// Bare bones interface
    .def("__getitem__", [](const Sequence &s, size_t i) {
			  if (i >= s.size()) throw py::index_error();
			  return s[i];
			})
    .def("__setitem__", [](Sequence &s, size_t i, float v) {
			  if (i >= s.size()) throw py::index_error();
			  s[i] = v;
			})
    .def("__len__", &Sequence::size)
    /// Optional sequence protocol operations
    .def("__iter__", [](const Sequence &s) { return py::make_iterator(s.begin(), s.end()); },
	 py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
    .def("__contains__", [](const Sequence &s, float v) { return s.contains(v); })
    .def("__reversed__", [](const Sequence &s) -> Sequence { return s.reversed(); })
    /// Slicing protocol (optional)
    .def("__getitem__", [](const Sequence &s, py::slice slice) -> Sequence* {
			  size_t start, stop, step, slicelength;
			  if (!slice.compute(s.size(), &start, &stop, &step, &slicelength))
			    throw py::error_already_set();
			  Sequence *seq = new Sequence(slicelength);
			  for (size_t i = 0; i < slicelength; ++i) {
			    (*seq)[i] = s[start]; start += step;
			  }
			  return seq;
			})
    .def("__setitem__", [](Sequence &s, py::slice slice, const Sequence &value) {
			  size_t start, stop, step, slicelength;
			  if (!slice.compute(s.size(), &start, &stop, &step, &slicelength))
			    throw py::error_already_set();
			  if (slicelength != value.size())
			    throw std::runtime_error("Left and right hand size of slice assignment have different sizes!");
			  for (size_t i = 0; i < slicelength; ++i) {
			    s[start] = value[i]; start += step;
			  }
			})
    /// Comparisons
    .def(py::self == py::self)
    .def(py::self != py::self)
    // Could also define py::self + py::self for concatenation, etc.
    ;

}
