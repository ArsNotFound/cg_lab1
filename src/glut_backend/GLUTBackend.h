#ifndef CG_LAB1_GLUTBACKEND_H
#define CG_LAB1_GLUTBACKEND_H

#include <memory>
#include <string>

#include "ICallbacks.h"

namespace GLUTBackend {
extern void init(int argc, char **argv);

extern bool createWindow(
    unsigned int width, unsigned int height, bool isFullscreen, const std::string &title
);

extern void run(const std::shared_ptr<ICallbacks> &callbacks);
};  // namespace GLUTBackend

#endif  // CG_LAB1_GLUTBACKEND_H
