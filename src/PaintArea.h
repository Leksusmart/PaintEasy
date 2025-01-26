#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QImage>
#include <QMouseEvent>
#include <QPoint>
#include <QScrollArea>
#include <QWheelEvent>
#include <QWidget>

class PaintArea : public QWidget
{
   Q_OBJECT

public:
   explicit PaintArea(QScrollArea *scrollArea, QWidget *parent = nullptr);
   void setImage(QImage image, double scale);
   QImage getImage();

   void mouseMoveEvent(QMouseEvent *event) override; // Обработка перемещения мыши
   QPoint move = QPoint(0, 0);
   QColor color1;
   QColor color2;
   bool isEraserActive = false;
   bool isFillActive = false;
   bool stateChanged = false;
   unsigned int drawSize = 1;
   QVector<QPair<QColor, QPoint>> array;
   QVector<QVector<QPair<QColor, QPoint>>> buffer;
   void stepBack();
   const int margin = 50;
   void wheelEvent(QWheelEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override;
   void keyReleaseEvent(QKeyEvent *event) override;
   void mousePressEvent(QMouseEvent *event) override;
   void mouseReleaseEvent(QMouseEvent *event) override;

protected:
   void paintEvent(QPaintEvent *event) override;
   void drawPixel(QMouseEvent *event);

private:
   QImage image;      // Изображение для рисования
   qreal scaleFactor; // Коэффициент масштаба
   bool ctrlHolded = false;
   QPoint lastMousePos;
   QScrollArea *scrollArea;
   bool dragging = false;
};

#endif // PAINTAREA_H
