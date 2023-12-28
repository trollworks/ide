#include <fmt/format.h>

#include <trollworks-editor/app.hpp>

int main(int argc, char** argv) {
  try {
    auto app = tw::editor::app();
    return app.run(argc, argv);
  }
  catch (const std::exception& err) {
    fmt::print(stderr, "Uncaught exception: {}\n", err.what());
    return 1;
  }
}
