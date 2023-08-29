#include "mapboxitem.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QMapbox>
#include <QMapboxGL>
#include <QScopedPointer>
#include <QQuickWindow>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QThread>

MapBoxItem::MapBoxItem(QQuickItem *parent)
    :QQuickFramebufferObject(parent)
{
    setFlags(ItemIsFocusScope|ItemHasContents);
    setMirrorVertically(true);
    setFocus(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    QMapboxGLSettings settings;
    settings.setCacheDatabasePath("/tmp/mbgl-cache.db");
    settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);

    m_map.reset(new QMapboxGL(nullptr,settings,QSize(100,100),1));
    //m_map->moveToThread(QThread::currentThread());
    m_map->setCoordinateZoom(QMapbox::Coordinate(39.86793971,116.53284098), 14);

    setStyle(0);
}

class MapBoxRender : public QQuickFramebufferObject::Renderer
{
public:
    MapBoxRender(const QQuickFramebufferObject* item){
    }

    virtual void render() override{
        m_map->render();
    }

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override{
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        auto fbo = new QOpenGLFramebufferObject(size, format);
        m_map->resize(size);
        m_map->setFramebufferObject(fbo->handle(),size);
        return fbo;
    }

    QSharedPointer<QMapboxGL> m_map;

};


QQuickFramebufferObject::Renderer *MapBoxItem::createRenderer() const
{
    m_render = new MapBoxRender(this);
    m_render->m_map = m_map;
    QObject::connect(m_map.data(),&QMapboxGL::needsRendering,this,&QQuickItem::update);
    return m_render;
}

void MapBoxItem::zoomIn()
{
    m_map->setZoom(m_map->zoom()+1);
}

void MapBoxItem::zoomOut()
{
    m_map->setZoom(m_map->zoom()-1);
}

void MapBoxItem::setStyle(int index)
{
    auto& styles = QMapbox::defaultStyles();
    if(index <0 || index >= styles.size())
        return;
    m_map->setStyleUrl(styles[index].first);
}

void MapBoxItem::changeStyle(){
    static uint8_t currentStyleIndex = 1;
    auto& styles = QMapbox::defaultStyles();
    m_map->setStyleUrl(styles[currentStyleIndex].first);
    if (++currentStyleIndex == styles.size()) {
        currentStyleIndex = 0;
    }
}

void MapBoxItem::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "sss";
    switch (event->key()) {
    case Qt::Key_S:
        qDebug() << "sss";
        break;
    default:
        break;
    }
}

void MapBoxItem::mousePressEvent(QMouseEvent *ev)
{
    m_lastPos = ev->position();

    if (ev->type() == QEvent::MouseButtonPress) {
        if (ev->buttons() == (Qt::LeftButton | Qt::RightButton)) {
//            changeStyle();
//            m_map->toggle3DExtrusions(true);
        }
    }

    ev->accept();
}

void MapBoxItem::mouseMoveEvent(QMouseEvent *ev)
{
    QPointF delta = ev->position() - m_lastPos;

    if (!delta.isNull()) {
        if (ev->buttons() == Qt::LeftButton && ev->modifiers() & Qt::ShiftModifier) {
            m_render->m_map->pitchBy(delta.y());
        } else if (ev->buttons() == Qt::LeftButton) {
            m_render->m_map->moveBy(delta);
        } else if (ev->buttons() == Qt::RightButton) {
            m_render->m_map->rotateBy(m_lastPos, ev->position());
        }
    }

    m_lastPos = ev->position();
    ev->accept();
}

void MapBoxItem::mouseReleaseEvent(QMouseEvent *event)
{

}

void MapBoxItem::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonDblClick) {
        if (ev->buttons() == Qt::LeftButton) {
            m_render->m_map->scaleBy(2.0, m_lastPos);
        } else if (ev->buttons() == Qt::RightButton) {
            m_render->m_map->scaleBy(0.5, m_lastPos);
        }
    }
    ev->accept();
}

void MapBoxItem::wheelEvent(QWheelEvent *ev)
{
    if (ev->angleDelta().x() >0) {
        return;
    }

    float factor = ev->angleDelta().y() / 1200.;
    if (ev->angleDelta().y() < 0) {
        factor = factor > -1 ? factor : 1 / factor;
    }

    m_render->m_map->scaleBy(1 + factor, ev->position());
    ev->accept();
}
