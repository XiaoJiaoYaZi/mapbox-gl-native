#include <mbgl/gl/headless_backend.hpp>
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QGLWidget>
#else
#include <QOpenGLWidget>
#endif
#include <QOpenGLContext>

#include <cassert>

namespace mbgl {
namespace gl {

class QtBackendImpl final : public HeadlessBackend::Impl {
public:
    ~QtBackendImpl() = default;

    gl::ProcAddress getExtensionFunctionPointer(const char* name) {
        QOpenGLContext* thisContext = QOpenGLContext::currentContext();
        return thisContext->getProcAddress(name);
    }

    void activateContext() {
        widget.makeCurrent();
    }

    void deactivateContext() {
        widget.doneCurrent();
    }

private:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QGLWidget widget;
#else
    QOpenGLWidget widget;
#endif
};

void HeadlessBackend::createImpl() {
    assert(!impl);
    impl = std::make_unique<QtBackendImpl>();
}

} // namespace gl
} // namespace mbgl
