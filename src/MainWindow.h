#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PaintArea.h"

#include <QImage>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

   void updateColors();
   void updateColors(QPushButton *Color, QColor &currentColor);
private slots:
   void closeEvent(QCloseEvent *event) override;
   void OpenFile();
   void SaveFile(bool chooseLocation = false);
   void mousePressEvent(QMouseEvent *event) override;   // Обработка нажатия мыши
   void mouseMoveEvent(QMouseEvent *event) override;    // Обработка перемещения мыши
   void mouseReleaseEvent(QMouseEvent *event) override; // Обработка отпускания мыши
   void wheelEvent(QWheelEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override;
   void keyReleaseEvent(QKeyEvent *event) override;

private:
   Ui::MainWindow *ui;
   PaintArea *paintArea;
   QImage image; // Изображение для рисования
   bool dragging = false;
   QPoint lastMousePos;

   QColor activeColor = qRgb(0, 0, 0);
   QColor secondColor = qRgb(200, 0, 0);
};

#endif // MAINWINDOW_H
