#ifndef MAPBOXITEM_H
#define MAPBOXITEM_H

#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QMapboxGL>
#include <QSharedPointer>

class MapBoxRender;

class MapBoxItem : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    MapBoxItem(QQuickItem* parent = nullptr);

    virtual Renderer * createRenderer() const override;

    Q_INVOKABLE void zoomIn();

    Q_INVOKABLE void zoomOut();

    Q_INVOKABLE void setStyle(int index);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent* ev) override;

    void changeStyle();
private:
    QPointF m_lastPos;
    mutable MapBoxRender * m_render = nullptr;

    QSharedPointer<QMapboxGL> m_map;
};

#endif // MAPBOXITEM_H
