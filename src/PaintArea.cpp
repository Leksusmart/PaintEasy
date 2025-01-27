#include "PaintArea.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QQueue>
#include <QScrollBar>
#include <QShortCut>

PaintArea::PaintArea(QScrollArea *scrollArea, QWidget *parent)
   : QWidget(parent)
   , scrollArea(scrollArea)
   , scaleFactor(1.0)
{
   image = QImage(200, 200, QImage::Format_ARGB32);
   image.fill(Qt::white);
   setFocusPolicy(Qt::StrongFocus);
   setFocus();
   update();
}
void PaintArea::stepBack()
{
   if (buffer.size() > 0) {
      QVector<QPair<QColor, QPoint>> array2 = buffer.back();
      buffer.pop_back();
      // Обновляем изображение
      for (int i = 0; i < array2.size(); i++) {
         QPoint point = array2[i].second;
         QColor color = array2[i].first;

         image.setPixel(point.x(), point.y(), color.rgba());
      }
      update(); // Обновляем виджет
   }
}
void PaintArea::setImage(QImage newImage, double scale)
{
   image = newImage;
   scaleFactor = scale;
   update();
}
QImage PaintArea::getImage()
{
   return image;
}
void PaintArea::paintEvent(QPaintEvent *event)
{
   QPainter painter(this);

   painter.translate(QPoint(margin, margin));
   painter.scale(scaleFactor, scaleFactor);
   painter.drawTiledPixmap(QRect(0, 0, image.width(), image.height()), QPixmap(":/image/Background_transparent.png"));
   painter.drawImage(0, 0, image);

   this->setFixedSize(image.size() * scaleFactor + QSize(margin * 2, margin * 2));
}
void PaintArea::wheelEvent(QWheelEvent *event)
{
   setFocus();
   if (ctrlHolded) {
      QPoint focusPoint = event->position().toPoint();
      scaleFactor *= (event->angleDelta().y() > 0) ? 1.1 : 1.0 / 1.1;

      update();
   }
   scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() - (event->angleDelta().y()));
}
void PaintArea::keyPressEvent(QKeyEvent *event)
{
   if (event->key() == Qt::Key_Control) {
      ctrlHolded = true;
   }
}
void PaintArea::keyReleaseEvent(QKeyEvent *event)
{
   if (event->key() == Qt::Key_Control) {
      ctrlHolded = false;
   }
}
void PaintArea::mousePressEvent(QMouseEvent *event)
{
   if (event->button() == Qt::LeftButton) {
      drawPixel(event);
   } else if (event->button() == Qt::RightButton) {
      drawPixel(event);
   }
   if (event->button() == Qt::MiddleButton) {
      setCursor(Qt::ClosedHandCursor);
      dragging = true;
   }
   lastMousePos = event->pos();
}
void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
   if (event->button() == Qt::MiddleButton) {
      unsetCursor();
      dragging = false;
   }
   if (!array.empty()) {
      buffer.push_back(array);
      array.clear();
   }
   lastMousePos = event->pos();
}
void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
   setFocus();

   if (event->buttons() & Qt::MiddleButton && dragging) {
      move = event->pos() - lastMousePos;
      scrollArea->horizontalScrollBar()->setValue(scrollArea->horizontalScrollBar()->value() - (move.x()));
      scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() - (move.y()));
      lastMousePos = event->pos() - move;
   } else {
      drawPixel(event);
      lastMousePos = event->pos();
   }
}
void PaintArea::drawPixel(QMouseEvent *event)
{
   QPoint CenterPixel = (event->pos() - QPoint(margin, margin)) / scaleFactor;

   if (CenterPixel.x() >= 0 && CenterPixel.x() < image.width() && CenterPixel.y() >= 0 && CenterPixel.y() < image.height()) {
      if (isEraserActive) {
         stateChanged = true;
         QImage background(":/image/Background_transparent.png");

         // Рисуем квадрат для стирания
         int halfSize = drawSize / 2;
         for (int x = -halfSize; x <= halfSize; ++x) {
            for (int y = -halfSize; y <= halfSize; ++y) {
               QPoint pointToErase = CenterPixel + QPoint(x, y);
               if (pointToErase.x() >= 0 && pointToErase.x() < image.width() && pointToErase.y() >= 0 && pointToErase.y() < image.height()) {
                  int backgrounx = pointToErase.x() % background.width();
                  int backgrouny = pointToErase.y() % background.height();
                  if (image.pixelColor(pointToErase) != background.pixel(backgrounx, backgrouny)) {
                     // Проверяем, содержится ли точка в массиве
                     bool pointExists = false;
                     for (const auto &pair : array) {
                        if (pair.second == pointToErase) {
                           pointExists = true;
                           break;
                        }
                     }
                     if (!pointExists) {
                        // Если точка не найдена, добавляем её в массив
                        QPair<QColor, QPoint> pair = qMakePair(image.pixelColor(pointToErase), pointToErase);
                        array.push_back(pair);
                     }
                     image.setPixel(pointToErase, background.pixel(backgrounx, backgrouny));
                  }
               }
            }
         }
         update();
      } else if (isFillActive) {
         QColor *color;
         if (event->buttons() & Qt::LeftButton) {
            color = &color1;
         } else if (event->buttons() & Qt::RightButton) {
            color = &color2;
         } else
            return;
         QColor targetColor = image.pixelColor(CenterPixel);
         if (targetColor != *color) {
            QQueue<QPoint> queue;
            queue.enqueue(CenterPixel);

            while (!queue.isEmpty()) {
               QPoint current = queue.dequeue();
               if (image.pixelColor(current) == targetColor) {
                  // Проверяем, содержится ли точка в массиве
                  QPair<QColor, QPoint> pair = qMakePair(image.pixelColor(current), current);
                  array.push_back(pair);

                  image.setPixel(current, color->rgba());
                  // Проверяем соседние пиксели
                  QPoint neighbors[4] = {
                     QPoint(current.x() + 1, current.y()), // Правый
                     QPoint(current.x() - 1, current.y()), // Левый
                     QPoint(current.x(), current.y() + 1), // Нижний
                     QPoint(current.x(), current.y() - 1)  // Верхний
                  };

                  for (const QPoint &neighbor : neighbors) {
                     if (neighbor.x() >= 0 && neighbor.x() < image.width() && neighbor.y() >= 0 && neighbor.y() < image.height() && image.pixelColor(neighbor) == targetColor) {
                        queue.enqueue(neighbor);
                     }
                  }
               }
            }
            update();
         }
      } else {
         QColor *color;
         if (event->buttons() & Qt::LeftButton) {
            color = &color1;
         } else if (event->buttons() & Qt::RightButton) {
            color = &color2;
         } else
            return;
         stateChanged = true;
         int halfSize = drawSize / 2;
         // Рисуем квадрат для кисти
         for (int x = -halfSize; x <= halfSize; ++x) {
            for (int y = -halfSize; y <= halfSize; ++y) {
               QPoint pointToDraw = CenterPixel + QPoint(x, y);
               if (pointToDraw.x() >= 0 && pointToDraw.x() < image.width() && pointToDraw.y() >= 0 && pointToDraw.y() < image.height()) {
                  if (image.pixelColor(pointToDraw) != *color) {
                     // Проверяем, содержится ли точка в массиве
                     bool pointExists = false;
                     for (const auto &pair : array) {
                        if (pair.second == pointToDraw) {
                           pointExists = true;
                           break;
                        }
                     }
                     if (!pointExists) {
                        // Если точка не найдена, добавляем её в массив
                        QPair<QColor, QPoint> pair = qMakePair(image.pixelColor(pointToDraw), pointToDraw);
                        array.push_back(pair);
                     }
                     image.setPixel(pointToDraw, color->rgba());
                  }
               }
            }
         }
         update();
      }
   }
}
