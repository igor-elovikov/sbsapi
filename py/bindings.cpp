#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "sbsar/Context.h"
#include "sbsar/Package.h"

namespace py = pybind11;

using namespace py::literals;

PYBIND11_MODULE(pysbsar, m)
{
	spdlog::set_level(spdlog::level::debug);

	py::enum_<sbsar::ParameterType>(m, "ParameterType")
	  .value("STRING", sbsar::ParameterType::STRING)
	  .value("FLOAT", sbsar::ParameterType::FLOAT)
	  .value("FLOAT2", sbsar::ParameterType::FLOAT2)
	  .value("FLOAT3", sbsar::ParameterType::FLOAT3)
	  .value("FLOAT4", sbsar::ParameterType::FLOAT4)
	  .value("INTEGER", sbsar::ParameterType::INTEGER)
	  .value("INTEGER2", sbsar::ParameterType::INTEGER2)
	  .value("INTEGER3", sbsar::ParameterType::INTEGER3)
	  .value("INTEGER4", sbsar::ParameterType::INTEGER4);

	py::enum_<sbsar::ParameterWidget>(m, "ParameterWidget")
	  .value("NONE", sbsar::ParameterWidget::NONE)
	  .value("SLIDER", sbsar::ParameterWidget::SLIDER)
	  .value("ANGLE", sbsar::ParameterWidget::ANGLE)
	  .value("COLOR", sbsar::ParameterWidget::COLOR)
	  .value("TOGGLE", sbsar::ParameterWidget::TOGGLE)
	  .value("COMBOBOX", sbsar::ParameterWidget::COMBOBOX)
	  .value("IMAGE", sbsar::ParameterWidget::IMAGE)
	  .value("POSITION", sbsar::ParameterWidget::POSITION);

	py::enum_<sbsar::Precision>(m, "Precision")
	  .value("B8", sbsar::Precision::B8)
	  .value("B16", sbsar::Precision::B16)
	  .value("B32", sbsar::Precision::B32);

	py::enum_<sbsar::DataFormat>(m, "DataFormat")
	  .value("RGB", sbsar::DataFormat::RGB)
	  .value("RGBA", sbsar::DataFormat::RGBA)
	  .value("RGBX", sbsar::DataFormat::RGBX)
	  .value("GRAYSCALE", sbsar::DataFormat::GRAYSCALE);

	py::enum_<sbsar::DataType>(m, "DataType")
	  .value("INTEGER", sbsar::DataType::INTEGER)
	  .value("FLOAT", sbsar::DataType::FLOAT);

	py::class_<sbsar::ImageFormat>(m, "ImageFormat")
	  .def_readwrite("depth", &sbsar::ImageFormat::precision)
	  .def_readwrite("format", &sbsar::ImageFormat::format)
	  .def_readwrite("dtype", &sbsar::ImageFormat::dtype)
	  .def_readwrite("num_channels", &sbsar::ImageFormat::num_channels);

	py::class_<sbs::Vec2Int>(m, "Vec2Int")
	  .def(py::init<int, int>(), "x"_a, "y"_a)
	  .def_readwrite("x", &sbs::Vec2Int::x)
	  .def_readwrite("y", &sbs::Vec2Int::y)
	  .def("__repr__", [](const sbs::Vec2Int& v) { return fmt::format("{}", v); });

	py::class_<sbs::Vec3Int>(m, "Vec3Int")
	  .def(py::init<int, int, int>(), "x"_a, "y"_a, "z"_a)
	  .def_readwrite("x", &sbs::Vec3Int::x)
	  .def_readwrite("y", &sbs::Vec3Int::y)
	  .def_readwrite("z", &sbs::Vec3Int::z)
	  .def("__repr__", [](const sbs::Vec3Int& v) { return fmt::format("{}", v); });

	py::class_<sbs::Vec4Int>(m, "Vec4Int")
	  .def(py::init<int, int, int, int>(), "x"_a, "y"_a, "z"_a, "w"_a)
	  .def_readwrite("x", &sbs::Vec4Int::x)
	  .def_readwrite("y", &sbs::Vec4Int::y)
	  .def_readwrite("z", &sbs::Vec4Int::z)
	  .def_readwrite("w", &sbs::Vec4Int::w)
	  .def("__repr__", [](const sbs::Vec4Int& v) { return fmt::format("{}", v); });

	py::class_<sbs::Vec2Float>(m, "Vec2Float")
	  .def(py::init<float, float>(), "x"_a, "y"_a)
	  .def_readwrite("x", &sbs::Vec2Float::x)
	  .def_readwrite("y", &sbs::Vec2Float::y)
	  .def("__repr__", [](const sbs::Vec2Float& v) { return fmt::format("{}", v); });

	py::class_<sbs::Vec3Float>(m, "Vec3Float")
	  .def(py::init<float, float, float>(), "x"_a, "y"_a, "z"_a)
	  .def_readwrite("x", &sbs::Vec3Float::x)
	  .def_readwrite("y", &sbs::Vec3Float::y)
	  .def_readwrite("z", &sbs::Vec3Float::z)
	  .def("__repr__", [](const sbs::Vec3Float& v) { return fmt::format("{}", v); });

	py::class_<sbs::Vec4Float>(m, "Vec4Float")
	  .def(py::init<float, float, float, float>(), "x"_a, "y"_a, "z"_a, "w"_a)
	  .def_readwrite("x", &sbs::Vec4Float::x)
	  .def_readwrite("y", &sbs::Vec4Float::y)
	  .def_readwrite("z", &sbs::Vec4Float::z)
	  .def_readwrite("w", &sbs::Vec4Float::w)
	  .def("__repr__", [](const sbs::Vec4Float& v) { return fmt::format("{}", v); });

	auto parm_class = py::class_<sbsar::Parameter>(m, "Parameter")
	  .def("type", &sbsar::Parameter::type)
	  .def("label", &sbsar::Parameter::label)
	  .def("description", &sbsar::Parameter::description)
	  .def("group", &sbsar::Parameter::group)
	  .def("usages", &sbsar::Parameter::usages)
	  .def("visible_condition", &sbsar::Parameter::visible_condition)
	  .def("widget", &sbsar::Parameter::widget)
	  .def("user_tag", &sbsar::Parameter::user_tag)
	  .def("component_labels", &sbsar::Parameter::component_labels)
	  .def("is_visible", &sbsar::Parameter::is_visible)
	  .def("slider_step", &sbsar::Parameter::slider_step)
	  .def("slider_clamp", &sbsar::Parameter::slider_clamp)
	  .def("label_true", &sbsar::Parameter::label_true)
	  .def("label_false", &sbsar::Parameter::label_false)
	  .def("get", &sbsar::Parameter::get)
	  .def("default_value", &sbsar::Parameter::default_value)
	  .def("max_value", &sbsar::Parameter::max_value)
	  .def("min_value", &sbsar::Parameter::min_value)
	  .def("choices", &sbsar::Parameter::choices)
	  .def("set", &sbsar::Parameter::set<std::string>, "value"_a);

	hana::for_each(sbs_parm_types, [&](auto& t) {
		auto builtin_type = get_builtin_type(t);
		using builtin_t = typename decltype(builtin_type)::type;
		parm_class.def("set", &sbsar::Parameter::set<builtin_t>, "value"_a);
	});

	py::class_<sbsar::Output>(m, "Output", py::buffer_protocol())
	  .def("save", &sbsar::Output::save, "filename"_a)
	  .def_readonly("format", &sbsar::Output::format)
	  .def_buffer([](sbsar::Output& output) -> py::buffer_info {

		  auto info = py::buffer_info{};
		  auto& format = output.get_data_format();

		  info.ptr = output.get_raw_data();
		  info.ndim = 2;
		  auto [width, height] = output.get_data_size();
		  if (format.num_channels == 1) {
			  info.shape = {width, height};
		  }
		  else {
			  info.shape = {width, height, format.num_channels};
		  }

		  switch (format.dtype) {
			  case sbsar::DataType::INTEGER:
				  switch (format.precision) {
					  case sbsar::Precision::B8:
						  info.format = py::format_descriptor<unsigned char>::format();
						  info.itemsize = sizeof(unsigned char);
						  break;
					  case sbsar::Precision::B16:
						  info.format = py::format_descriptor<uint16_t>::format();
						  info.itemsize = sizeof(uint16_t);
						  break;
					  case sbsar::Precision::B32:
						  spdlog::warn("Python Buffer Object: Unsupported format Int32 for output!");
						  break;
				  }
				  break;
			  case sbsar::DataType::FLOAT:
				  switch (format.precision) {
					  case sbsar::Precision::B8:
						  spdlog::warn("Python Buffer Object: Float8 is not a valid format!");
						  break;
					  case sbsar::Precision::B16:
						  info.format = "e";  // TODO: "e"? is it portable?
						  info.itemsize = sizeof(uint16_t);
						  break;
					  case sbsar::Precision::B32:
						  info.format = py::format_descriptor<float>::format();
						  info.itemsize = sizeof(float );
						  break;
				  }
				  break;
		  }

		  info.strides = { info.itemsize * height * format.num_channels, info.itemsize * format.num_channels };

		  return info;
	  });

	py::class_<sbsar::Graph>(m, "Graph")
	  .def("render", &sbsar::Graph::render, "grab_results"_a = true)
	  .def("parm", &sbsar::Graph::parm, py::return_value_policy::reference)
	  .def("output", &sbsar::Graph::output, py::return_value_policy::reference)
	  .def("parms", &sbsar::Graph::parms)
	  .def("outputs", &sbsar::Graph::outputs)
	  .def("inputs", &sbsar::Graph::inputs);

	py::class_<sbsar::Package>(m, "Package")
	  .def("graph", &sbsar::Package::graph, py::return_value_policy::reference);

	py::class_<sbsar::Context>(m, "Context")
	  .def(py::init<>())
	  .def("load_package", &sbsar::Context::load_package, "filename"_a, "instantiate"_a = true,
		py::return_value_policy::reference);

}

