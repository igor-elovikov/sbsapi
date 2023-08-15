#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "sbsar/context.h"

namespace py = pybind11;

using namespace py::literals;

PYBIND11_MODULE(pysbsar, m) {
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

	py::enum_<sbsar::OutputSize>(m, "OutputSize")
	  .value("x1", sbsar::OutputSize::x1)
	  .value("x2", sbsar::OutputSize::x2)
	  .value("x4", sbsar::OutputSize::x4)
	  .value("x8", sbsar::OutputSize::x8)
	  .value("x16", sbsar::OutputSize::x16)
	  .value("x32", sbsar::OutputSize::x32)
	  .value("x64", sbsar::OutputSize::x64)
	  .value("x128", sbsar::OutputSize::x128)
	  .value("x256", sbsar::OutputSize::x256)
	  .value("x512", sbsar::OutputSize::x512)
	  .value("x1024", sbsar::OutputSize::x1024)
	  .value("x2048", sbsar::OutputSize::x2048)
	  .value("x4096", sbsar::OutputSize::x4096)
	  .value("x8192", sbsar::OutputSize::x8192)
	  .value("NONE", sbsar::OutputSize::NONE);

	py::class_<sbsar::OutputFormatOverride>(m, "OutputFormatOverride")
	  .def_readwrite("format", &sbsar::OutputFormatOverride::format)
	  .def_readwrite("resolution", &sbsar::OutputFormatOverride::resolution);

	py::class_<sbsar::OutputResolution>(m, "OutputResolution")
	  .def_readwrite("width", &sbsar::OutputResolution::width)
	  .def_readwrite("height", &sbsar::OutputResolution::height);

	py::class_<sbsar::PixelFormat>(m, "PixelFormat")
	  .def_readwrite("depth", &sbsar::PixelFormat::precision)
	  .def_readwrite("format", &sbsar::PixelFormat::format)
	  .def_readwrite("dtype", &sbsar::PixelFormat::dtype)
	  .def_readwrite("num_channels", &sbsar::PixelFormat::num_channels);

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
	  .def_property_readonly("type", &sbsar::Parameter::type)
	  .def_property_readonly("id", &sbsar::Parameter::id)
	  .def_property_readonly("label", &sbsar::Parameter::label)
	  .def_property_readonly("description", &sbsar::Parameter::description)
	  .def_property_readonly("group", &sbsar::Parameter::group)
	  .def_property_readonly("usages", &sbsar::Parameter::usages)
	  .def_property_readonly("visible_condition", &sbsar::Parameter::visible_condition)
	  .def_property_readonly("widget", &sbsar::Parameter::widget)
	  .def_property_readonly("user_tag", &sbsar::Parameter::user_tag)
	  .def_property_readonly("component_labels", &sbsar::Parameter::component_labels)
	  .def_property_readonly("is_visible", &sbsar::Parameter::is_visible)
	  .def_property_readonly("slider_step", &sbsar::Parameter::slider_step)
	  .def_property_readonly("slider_clamp", &sbsar::Parameter::slider_clamp)
	  .def_property_readonly("label_true", &sbsar::Parameter::label_true)
	  .def_property_readonly("label_false", &sbsar::Parameter::label_false)
	  .def_property("value", &sbsar::Parameter::get, [](sbsar::Parameter& parm, const sbsar::Value& v) {
		  parm.set(v);
	  })
	  .def_property_readonly("default_value", &sbsar::Parameter::default_value)
	  .def_property_readonly("max_value", &sbsar::Parameter::max_value)
	  .def_property_readonly("min_value", &sbsar::Parameter::min_value)
	  .def_property_readonly("choices", &sbsar::Parameter::choices)
	  .def("set", [](sbsar::Parameter& parm, const sbsar::Value& v) { parm.set(v); }, "value"_a);

	py::class_<sbsar::Output>(m, "Output", py::buffer_protocol())
	  .def("save", &sbsar::Output::save, "filename"_a)
	  .def_readonly("format", &sbsar::Output::format)
	  .def_property_readonly("id", &sbsar::Output::id)
	  .def_property_readonly("label", &sbsar::Output::label)
	  .def_property_readonly("group", &sbsar::Output::group)
	  .def_property_readonly("usages", &sbsar::Output::usages)
	  .def_property_readonly("visible_condition", &sbsar::Output::visible_condition)
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
						  info.format = "e";  // NOTE: "e"? is it portable?
						  info.itemsize = sizeof(uint16_t);
						  break;
					  case sbsar::Precision::B32:
						  info.format = py::format_descriptor<float>::format();
						  info.itemsize = sizeof(float);
						  break;
				  }
				  break;
		  }

		  info.strides = {info.itemsize * height * format.num_channels, info.itemsize * format.num_channels};

		  return info;
	  });

	py::class_<sbsar::Input>(m, "Input", py::buffer_protocol())
	  .def_property_readonly("id", &sbsar::Input::id)
	  .def_property_readonly("label", &sbsar::Input::label)
	  .def_property_readonly("description", &sbsar::Input::description)
	  .def_property_readonly("visible_condition", &sbsar::Input::visible_condition)
	  .def_property_readonly("is_visible", &sbsar::Input::is_visible)
	  .def_property_readonly("is_color", &sbsar::Input::is_color)
	  .def_property_readonly("is_floating_point", &sbsar::Input::is_floating_point)
	  .def_property_readonly("usages", &sbsar::Input::usages)
	  .def("load_from_file", &sbsar::Input::load_from_file, "filename"_a)
	  .def("load_from_array", [](sbsar::Input& input, py::buffer& buffer) -> void {
		  auto info = buffer.request();
		  auto shape = info.shape;
		  if (shape.size() < 2) return;

		  auto format = sbsar::PixelFormat{};
		  format.num_channels = shape.size() > 2 ? static_cast<int>(shape[2]) : 1;

		  if (info.format == py::format_descriptor<unsigned char>::format()) {
			  format.dtype = sbsar::DataType::INTEGER;
			  format.precision = sbsar::Precision::B8;
		  }
		  else if (info.format == py::format_descriptor<unsigned short>::format()) {
			  format.dtype = sbsar::DataType::INTEGER;
			  format.precision = sbsar::Precision::B16;
		  }
		  else if (info.format == "e") {
			  format.dtype = sbsar::DataType::FLOAT;
			  format.precision = sbsar::Precision::B16;
		  }
		  else if (info.format == py::format_descriptor<float>::format()) {
			  format.dtype = sbsar::DataType::FLOAT;
			  format.precision = sbsar::Precision::B32;
		  }
		  else {
			  spdlog::warn("Python Buffer Object: Uploading failed, format is not supported");
			  return;
		  }

		  switch (format.num_channels) {
			  case 1:
				  format.format = sbsar::DataFormat::GRAYSCALE;
				  break;
			  case 3:
				  format.format = sbsar::DataFormat::RGB;
				  break;
			  case 4:
				  format.format = sbsar::DataFormat::RGBA;
				  break;
		  }

		  input.load_from_buffer(info.ptr, static_cast<int>(shape[0]), static_cast<int>(shape[1]), format);
	  }, "data"_a);

	py::class_<sbsar::Graph>(m, "Graph")
	  .def("render", &sbsar::Graph::render, "grab_results"_a = true)
	  .def("parm", &sbsar::Graph::parm, py::return_value_policy::reference)
	  .def("output", &sbsar::Graph::output, py::return_value_policy::reference)
	  .def("input", &sbsar::Graph::input, py::return_value_policy::reference)
	  .def("parms", &sbsar::Graph::parms, py::return_value_policy::reference)
	  .def("outputs", &sbsar::Graph::outputs, py::return_value_policy::reference)
	  .def("inputs", &sbsar::Graph::inputs, py::return_value_policy::reference);

	py::class_<sbsar::Package>(m, "Package")
	  .def("graph", &sbsar::Package::graph, "id"_a, py::return_value_policy::reference)
	  .def("graphs", &sbsar::Package::graphs, py::return_value_policy::reference);

	py::class_<sbsar::Context>(m, "Context")
	  .def(py::init<bool>(), "enable_renderer"_a = true)
	  .def(py::init<size_t>(), "memory_budget_mbytes"_a)
	  .def("load_package", [](sbsar::Context& ctx, const char* filename, bool instantiate) -> sbsar::Package* {
		  return ctx.load_package(filename, instantiate).expect();
		  },
		"filename"_a, "instantiate"_a = true,
		py::return_value_policy::reference);

}

