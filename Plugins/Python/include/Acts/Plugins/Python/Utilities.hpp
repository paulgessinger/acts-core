#pragma once

#include <string>
#include <unordered_map>

#include <pybind11/pybind11.h>

namespace Acts::Python {

struct Context {
  std::unordered_map<std::string, pybind11::module_*> modules;

  pybind11::module_& get(const std::string& name) { return *modules.at(name); }

  // template <typename... Args,
  //           typename = std::enable_if_t<sizeof...(Args) >= 2> >
  // std::array<std::reference_wrapper<pybind11::module_>, sizeof...(Args)> get(
  //     Args&&... args) {
  //   return {(*modules.at(args))...};
  // }
  template <typename... Args,
            typename = std::enable_if_t<sizeof...(Args) >= 2> >
  auto get(Args&&... args) {
    return std::make_tuple((*modules.at(args))...);
  }
};

template <typename T, typename Ur, typename Ut>
void pythonRangeProperty(T& obj, const std::string& name, Ur Ut::*begin,
                         Ur Ut::*end) {
  obj.def_property(
      name.c_str(),
      [=](Ut& self) {
        return std::pair{self.*begin, self.*end};
      },
      [=](Ut& self, std::pair<Ur, Ur> p) {
        self.*begin = p.first;
        self.*end = p.second;
      });
}

inline void patchClassesWithConfig(pybind11::module_& m) {
  pybind11::module::import("acts._adapter").attr("_patch_config")(m);
}

template <typename T>
void patchKwargsConstructor(T& c) {
  pybind11::module::import("acts._adapter").attr("_patchKwargsConstructor")(c);
}

inline pybind11::module_ getMainModule() {
  return pybind11::module::import("acts.ActsPythonBindings");
}

#define ACTS_PYTHON_MEMBER(name) \
  _binding_instance.def_readwrite(#name, &_struct_type::name)

#define ACTS_PYTHON_STRUCT_BEGIN(obj, cls) \
  {                                        \
    auto& _binding_instance = obj;         \
    using _struct_type = cls;              \
    do {                                   \
    } while (0)

#define ACTS_PYTHON_STRUCT_END() \
  }                              \
  do {                           \
  } while (0)

}  // namespace Acts::Python