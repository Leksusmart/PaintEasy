#include "MainWindow.h"
#include "ExitDialog.h"
#include "src/ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   setWindowTitle("Безымянный - PaintEasy");
   paintArea = new PaintArea(ui->PaintArea, this);
   ui->PaintArea->setWidget(paintArea);
   ui->PaintArea->setWidgetResizable(true);
   ui->PaintArea->setFocusPolicy(Qt::StrongFocus);
   ui->PaintArea->setFocus();

   QShortcut *redo = new QShortcut(QKeySequence("Ctrl+Z"), this);
   connect(redo, &QShortcut::activated, [this]() { paintArea->stepBack(); });

   QShortcut *quickSave = new QShortcut(QKeySequence("Ctrl+S"), this);
   connect(quickSave, &QShortcut::activated, [this]() { SaveFile(); });
   connect(ui->action_Open, &QAction::triggered, this, &MainWindow::OpenFile);
   connect(ui->action_Save, &QAction::triggered, this, &MainWindow::SaveFile);
   connect(ui->action_SaveAs, &QAction::triggered, [this]() { SaveFile(true); });

   connect(ui->button_pencil, &QPushButton::clicked, [this]() {
      paintArea->isEraserActive = false;
      paintArea->isFillActive = false;
      ui->button_pencil->setStyleSheet("border:2px solid blue;border-radius: 3px;background-color: rgb(126, 126, 126);");
      ui->button_eraser->setStyleSheet("border:1px solid black;border-radius: 3px;background-color: rgb(126, 126, 126);");
      ui->button_fill->setStyleSheet("border:1px solid black;border-radius: 3px;background-color: rgb(126, 126, 126);");
   });
   connect(ui->button_eraser, &QPushButton::clicked, [this]() {
      paintArea->isEraserActive = true;
      paintArea->isFillActive = false;
      ui->button_pencil->setStyleSheet("border:1px solid black;border-radius: 3px;background-color: rgb(126, 126, 126);");
      ui->button_eraser->setStyleSheet("border:2px solid blue;border-radius: 3px;background-color: rgb(126, 126, 126);");
      ui->button_fill->setStyleSheet("border:1px solid black;border-radius: 3px;background-color: rgb(126, 126, 126);");
   });
   connect(ui->button_fill, &QPushButton::clicked, [this]() {
      paintArea->isEraserActive = false;
      paintArea->isFillActive = true;
      ui->button_pencil->setStyleSheet("border:1px solid black;border-radius: 3px;background-color: rgb(126, 126, 126);");
      ui->button_eraser->setStyleSheet("border:1px solid black;border-radius: 3px;background-color: rgb(126, 126, 126);");
      ui->button_fill->setStyleSheet("border:2px solid blue;border-radius: 3px;background-color: rgb(126, 126, 126);");
   });
   connect(ui->Slider_DrawSize, &QSlider::valueChanged, [this]() {
      paintArea->drawSize = ui->Slider_DrawSize->value();
      ui->label_DrawSize->setText("Размер кисти: " + QString::number(ui->Slider_DrawSize->value()) + "px");
   });
   updateColors();

   ui->FirstColor->setStyleSheet(QString("background-color: %1;border:2px solid blue;").arg(activeColor.name()));
   ui->SecondColor->setStyleSheet(QString("background-color: %1;border:2px solid black;").arg(secondColor.name()));

   connect(ui->SecondColor, &QPushButton::clicked, [this]() {
      if (ui->SecondColor->styleSheet() == QString("background-color: %1;border:2px solid black;").arg(secondColor.name())) {
         QColor temp = secondColor;
         secondColor = activeColor;
         activeColor = temp;
         updateColors();
         ui->SecondColor->setStyleSheet(QString("background-color: %1;border:2px solid blue;").arg(activeColor.name()));
         ui->FirstColor->setStyleSheet(QString("background-color: %1;border:2px solid black;").arg(secondColor.name()));
      }
   });
   connect(ui->FirstColor, &QPushButton::clicked, [this]() {
      if (ui->FirstColor->styleSheet() == QString("background-color: %1;border:2px solid black;").arg(secondColor.name())) {
         QColor temp = secondColor;
         secondColor = activeColor;
         activeColor = temp;
         updateColors();
         ui->FirstColor->setStyleSheet(QString("background-color: %1;border:2px solid blue;").arg(activeColor.name()));
         ui->SecondColor->setStyleSheet(QString("background-color: %1;border:2px solid black;").arg(secondColor.name()));
      }
   });

   connect(ui->Color_1, &QPushButton::clicked, [this]() { updateColors(ui->Color_1, activeColor); });
   connect(ui->Color_2, &QPushButton::clicked, [this]() { updateColors(ui->Color_2, activeColor); });
   connect(ui->Color_3, &QPushButton::clicked, [this]() { updateColors(ui->Color_3, activeColor); });
   connect(ui->Color_4, &QPushButton::clicked, [this]() { updateColors(ui->Color_4, activeColor); });
   connect(ui->Color_5, &QPushButton::clicked, [this]() { updateColors(ui->Color_5, activeColor); });
   connect(ui->Color_6, &QPushButton::clicked, [this]() { updateColors(ui->Color_6, activeColor); });
   connect(ui->Color_7, &QPushButton::clicked, [this]() { updateColors(ui->Color_7, activeColor); });
   connect(ui->Color_8, &QPushButton::clicked, [this]() { updateColors(ui->Color_8, activeColor); });
   connect(ui->Color_9, &QPushButton::clicked, [this]() { updateColors(ui->Color_9, activeColor); });
   connect(ui->Color_10, &QPushButton::clicked, [this]() { updateColors(ui->Color_10, activeColor); });
   connect(ui->Color_11, &QPushButton::clicked, [this]() { updateColors(ui->Color_11, activeColor); });
   connect(ui->Color_12, &QPushButton::clicked, [this]() { updateColors(ui->Color_12, activeColor); });
   connect(ui->Color_13, &QPushButton::clicked, [this]() { updateColors(ui->Color_13, activeColor); });
   connect(ui->Color_14, &QPushButton::clicked, [this]() { updateColors(ui->Color_14, activeColor); });
   connect(ui->Color_15, &QPushButton::clicked, [this]() { updateColors(ui->Color_15, activeColor); });
   connect(ui->Color_16, &QPushButton::clicked, [this]() { updateColors(ui->Color_16, activeColor); });
   connect(ui->Color_17, &QPushButton::clicked, [this]() { updateColors(ui->Color_17, activeColor); });
   connect(ui->Color_18, &QPushButton::clicked, [this]() { updateColors(ui->Color_18, activeColor); });
   connect(ui->Color_19, &QPushButton::clicked, [this]() { updateColors(ui->Color_19, activeColor); });
   connect(ui->Color_20, &QPushButton::clicked, [this]() { updateColors(ui->Color_20, activeColor); });

   connect(ui->Color_1, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_1, secondColor); });
   connect(ui->Color_2, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_2, secondColor); });
   connect(ui->Color_3, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_3, secondColor); });
   connect(ui->Color_4, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_4, secondColor); });
   connect(ui->Color_5, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_5, secondColor); });
   connect(ui->Color_6, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_6, secondColor); });
   connect(ui->Color_7, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_7, secondColor); });
   connect(ui->Color_8, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_8, secondColor); });
   connect(ui->Color_9, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_9, secondColor); });
   connect(ui->Color_10, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_10, secondColor); });
   connect(ui->Color_11, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_11, secondColor); });
   connect(ui->Color_12, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_12, secondColor); });
   connect(ui->Color_13, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_13, secondColor); });
   connect(ui->Color_14, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_14, secondColor); });
   connect(ui->Color_15, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_15, secondColor); });
   connect(ui->Color_16, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_16, secondColor); });
   connect(ui->Color_17, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_17, secondColor); });
   connect(ui->Color_18, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_18, secondColor); });
   connect(ui->Color_19, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_19, secondColor); });
   connect(ui->Color_20, &QPushButton::customContextMenuRequested, [this]() { updateColors(ui->Color_20, secondColor); });
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
   paintArea->keyPressEvent(event);
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
   paintArea->keyReleaseEvent(event);
}
void MainWindow::wheelEvent(QWheelEvent *event)
{
   paintArea->wheelEvent(event);
}
void MainWindow::updateColors(QPushButton *Color, QColor &currentColor)
{
   bool match = false;
   QString hex = "";
   for (int i = 0; i < Color->styleSheet().size() - 6; i++) {
      if (Color->styleSheet()[i] == '#' || match)
         match = true;
      else
         continue;
      hex += Color->styleSheet()[i];
      if (hex.size() == 7)
         break;
   }
   if (QColor::isValidColorName(hex)) {
      currentColor = QColor(hex);
      updateColors();
   }
}
void MainWindow::updateColors()
{
   paintArea->color1 = activeColor;
   paintArea->color2 = secondColor;
   if (ui->FirstColor->styleSheet().contains("blue")) {
      ui->FirstColor->setStyleSheet(QString("background-color: %1;border:2px solid blue;").arg(activeColor.name()));
      ui->SecondColor->setStyleSheet(QString("background-color: %1;border:2px solid black;").arg(secondColor.name()));
   } else if (ui->SecondColor->styleSheet().contains("blue")) {
      ui->SecondColor->setStyleSheet(QString("background-color: %1;border:2px solid blue;").arg(activeColor.name()));
      ui->FirstColor->setStyleSheet(QString("background-color: %1;border:2px solid black;").arg(secondColor.name()));
   }
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
   if (event->button() == Qt::MiddleButton) {
      setCursor(Qt::ClosedHandCursor);
      dragging = true;
   }
   lastMousePos = event->pos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
   if (dragging) {
      QPoint currentMousePos = event->pos();
      QPoint offset = currentMousePos - lastMousePos;

      // Изменяем положение ползунков
      ui->PaintArea->horizontalScrollBar()->setValue(ui->PaintArea->horizontalScrollBar()->value() - offset.x());
      ui->PaintArea->verticalScrollBar()->setValue(ui->PaintArea->verticalScrollBar()->value() - offset.y());

      lastMousePos = currentMousePos;
   }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
   if (event->button() == Qt::MiddleButton) {
      unsetCursor();
      dragging = false;
   }
   paintArea->buffer.push_back(paintArea->array);
   paintArea->array.clear();
   lastMousePos = event->pos();
}
void MainWindow::OpenFile()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Изображения (*.png *.jpg *.bmp)"));
   if (!fileName.isEmpty()) {
      if (!image.load(fileName)) {
         QMessageBox::warning(this, tr("Open Image"), tr("Could not open image file."));
      } else {
         QFileInfo fileInfo(fileName);
         setWindowTitle(fileInfo.fileName() + " - PaintEasy"); // Получаем только имя файла
         // Рассчитываем масштаб, чтобы изображение полностью помещалось в окно
         double widthScale = (double) (ui->PaintArea->width() - paintArea->margin * 2) / image.width();
         double heightScale = (double) (ui->PaintArea->height() - paintArea->margin * 2) / image.height();
         double scale = qMin(widthScale, heightScale);
         paintArea->setImage(image, scale);
         update();
      }
   }
}
void MainWindow::SaveFile(bool chooseLocation)
{
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "Безымянный", tr("Изображения (*.png *.jpg *.bmp)"));
   if (!fileName.isEmpty()) {
      if (!paintArea->getImage().save(fileName)) {
         QMessageBox::warning(this, tr("Save Image"), tr("Could not save image file."));
      } else {
         paintArea->stateChanged = false;
      }
   }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
   if (paintArea->stateChanged) {
      ExitDialog dialog(this);
      dialog.exec();
      unsigned short int answer = dialog.status;
      if (answer == 1) {
         SaveFile();
      } else if (answer == 0) {
         event->ignore();
         return;
      }
   }
   event->accept();
}
MainWindow::~MainWindow()
{
   delete ui;
}
