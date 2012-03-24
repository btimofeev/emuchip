/*
 *  emuChip - CHIP-8 emulator.
 *  Copyright (C) 2009-2012  Boris Timofeev <mashin87@gmail.com> <http://www.emunix.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow()
{

	createActions();
	createMenu();

	emu = new ChipEmu();
	display = new DisplayWidget();
	setCentralWidget(display);
	
	emuStart = false;
	readSettings();
}

void MainWindow::openRom()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), lastDir, tr("All files (*)"));
	lastDir = QFileInfo(fileName).absolutePath();
	
	if (fileName != "")
	{
		closeRom();
		emu->loadGame(fileName.toLocal8Bit().data());
		emuStart = true;
		emulation();
	}
}

void MainWindow::closeRom()
{
	emuStart = false;
	emu->init();
	display->clear();
	display->repaint();
}

void MainWindow::emulation()
{
	QElapsedTimer et;
	et.start();
	
	while (emuStart)
	{
		QCoreApplication::processEvents ( QEventLoop::AllEvents );

		if (opcode_count < 12)	// execute 720 opcodes per sec
		{
			emu->executeNextOpcode();
			opcode_count++;
		}
			
		//decrease timers every 1/60sec and redraw screen
		if (et.hasExpired(1000/60))
		{
			emu->decreaseTimers();
			et.restart();
			
			display->setScreen(emu->screen);
			display->repaint();
			
			opcode_count = 0;
		}
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{	
	switch(event->key())
	{
		case Qt::Key_1: emu->key[1] = 1; break;
		case Qt::Key_2: emu->key[2] = 1; break;
		case Qt::Key_3: emu->key[3] = 1; break;
		case Qt::Key_4: emu->key[0xC] = 1; break;
		case Qt::Key_Q: emu->key[4] = 1; break;
		case Qt::Key_W: emu->key[5] = 1; break;
		case Qt::Key_E: emu->key[6] = 1; break;
		case Qt::Key_R: emu->key[0xD] = 1; break;
		case Qt::Key_A: emu->key[7] = 1; break;
		case Qt::Key_S: emu->key[8] = 1; break;
		case Qt::Key_D: emu->key[9] = 1; break;
		case Qt::Key_F: emu->key[0xE] = 1; break;
		case Qt::Key_Z: emu->key[0xA] = 1; break;
		case Qt::Key_X: emu->key[0] = 1; break;
		case Qt::Key_C: emu->key[0xB] = 1; break;
		case Qt::Key_V: emu->key[0xF] = 1; break;
		//default: QWidget::keyPressEvent(event);
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	switch(event->key())
	{
		case Qt::Key_1: emu->key[1] = 0; break;
		case Qt::Key_2: emu->key[2] = 0; break;
		case Qt::Key_3: emu->key[3] = 0; break;
		case Qt::Key_4: emu->key[0xC] = 0; break;
		case Qt::Key_Q: emu->key[4] = 0; break;
		case Qt::Key_W: emu->key[5] = 0; break;
		case Qt::Key_E: emu->key[6] = 0; break;
		case Qt::Key_R: emu->key[0xD] = 0; break;
		case Qt::Key_A: emu->key[7] = 0; break;
		case Qt::Key_S: emu->key[8] = 0; break;
		case Qt::Key_D: emu->key[9] = 0; break;
		case Qt::Key_F: emu->key[0xE] = 0; break;
		case Qt::Key_Z: emu->key[0xA] = 0; break;
		case Qt::Key_X: emu->key[0] = 0; break;
		case Qt::Key_C: emu->key[0xB] = 0; break;
		case Qt::Key_V: emu->key[0xF] = 0; break;
		//default: QWidget::keyPressEvent(event);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	emuStart = false;
	writeSettings();
	delete display;
	delete emu;
	event->accept();
}

void MainWindow::createActions()
{
	openRomAction = new QAction(tr("&Open ROM.."), this);
	openRomAction->setShortcut(tr("Ctrl+O"));
	connect(openRomAction, SIGNAL(triggered()), this, SLOT(openRom()));

	closeRomAction = new QAction(tr("&Close ROM.."), this);
	closeRomAction->setShortcut(tr("Ctrl+C"));
	connect(closeRomAction, SIGNAL(triggered()), this, SLOT(closeRom()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Alt+F4"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	set1xAction = new QAction(tr("64x32"), this);
	set1xAction->setCheckable(true);
	connect(set1xAction, SIGNAL(triggered()), this, SLOT(set1x()));
	set2xAction = new QAction(tr("128x64"), this);
	set2xAction->setCheckable(true);
	connect(set2xAction, SIGNAL(triggered()), this, SLOT(set2x()));
	set4xAction = new QAction(tr("256x128"), this);
	set4xAction->setCheckable(true);
	connect(set4xAction, SIGNAL(triggered()), this, SLOT(set4x()));
	set8xAction = new QAction(tr("512x256"), this);
	set8xAction->setCheckable(true);
	connect(set8xAction, SIGNAL(triggered()), this, SLOT(set8x()));
	set16xAction = new QAction(tr("1024x512"), this);
	set16xAction->setCheckable(true);
	connect(set16xAction, SIGNAL(triggered()), this, SLOT(set16x()));

	resolutionGroup = new QActionGroup(this);
	resolutionGroup->addAction(set1xAction);
	resolutionGroup->addAction(set2xAction);
	resolutionGroup->addAction(set4xAction);
	resolutionGroup->addAction(set8xAction);
	resolutionGroup->addAction(set16xAction);

	bgColorDialogAction = new QAction(tr("&Background color.."), this);
	connect(bgColorDialogAction, SIGNAL(triggered()), this, SLOT(bgColorDialog()));
	fgColorDialogAction = new QAction(tr("&Foreground color.."), this);
	connect(fgColorDialogAction, SIGNAL(triggered()), this, SLOT(fgColorDialog()));

	aboutAction = new QAction(tr("&About.."), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openRomAction);
	fileMenu->addAction(closeRomAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	videoMenu = menuBar()->addMenu(tr("&Video"));
	resolutionMenu = videoMenu->addMenu(tr("&Window size"));
	resolutionMenu->addAction(set1xAction);
	resolutionMenu->addAction(set2xAction);
	resolutionMenu->addAction(set4xAction);
	resolutionMenu->addAction(set8xAction);
	resolutionMenu->addAction(set16xAction);
	videoMenu->addSeparator();
	videoMenu->addAction(bgColorDialogAction);
	videoMenu->addAction(fgColorDialogAction);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
}

void MainWindow::set1x()
{
	display->setResolution(1);
	display->repaint();
	setFixedSize (64, 32 + menuBar()->height());
}

void MainWindow::set2x()
{
	display->setResolution(2);
	display->repaint();
	setFixedSize (128, 64 + menuBar()->height());
}

void MainWindow::set4x()
{
	display->setResolution(4);
	display->repaint();
	setFixedSize (256, 128 + menuBar()->height());
}

void MainWindow::set8x()
{
	display->setResolution(8);
	display->repaint();
	setFixedSize (512, 256 + menuBar()->height());
}

void MainWindow::set16x()
{
	display->setResolution(16);
	display->repaint();
	setFixedSize (1024, 512 + menuBar()->height());
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About"), "<center><h3>emuChip v"+QCoreApplication::applicationVersion()+"</h3></center>"+tr("<center><p>emuChip is cross-platform CHIP-8 emulator.</p><b>Homepage:  </b> <a href=\"http://emuchip.googlecode.com\">http://emuchip.googlecode.com</a>.<small><p>Copyright &copy; 2009-2012 Boris Timofeev (<a href=\"mailto:mashin87@gmail.com\">mashin87@gmail.com</a>).</p></small></center>"));
}

void MainWindow::writeSettings()
{
	QSettings settings("emuChip", "emuChip");
	settings.setValue("displayResolution", display->getResolution());
	settings.setValue("lastDir", lastDir);
	QVariant color =  display->getBgColor();
	settings.setValue("bgColor", color);
	color =  display->getFgColor();
	settings.setValue("fgColor", color);
}

void MainWindow::readSettings()
{
	QSettings settings("emuChip", "emuChip");
	int r = settings.value("displayResolution", 8).toInt();
	switch(r)
	{
		case 1: set1xAction->setChecked(true); set1x(); break;
		case 2: set2xAction->setChecked(true); set2x(); break;
		case 4: set4xAction->setChecked(true); set4x(); break;
		case 8: set8xAction->setChecked(true); set8x(); break;
		case 16: set16xAction->setChecked(true); set16x(); break;
	}
	lastDir = settings.value("lastDir", ".").toString();

	QVariant v = settings.value("bgColor", Qt::black);
	QColor color = v.value<QColor>();
	display->setBgColor(color);

	v = settings.value("fgColor", Qt::white);
	color = v.value<QColor>();
	display->setFgColor(color);
}

void MainWindow::bgColorDialog()
{
	display->setBgColor(QColorDialog::getColor (Qt::black));
	display->repaint();
}

void MainWindow::fgColorDialog()
{
	display->setFgColor(QColorDialog::getColor (Qt::white));
	display->repaint();
}
