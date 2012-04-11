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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include "../chip_emu.h"
#include "displaywidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	void readSettings();
	void writeSettings();
	void loadGame();

private slots:
	void openRom();
	void closeRom();
	void set1x();
	void set2x();
	void set4x();
	void set8x();
	void bgColorDialog();
	void fgColorDialog();
	void reset();
	void about();
	
protected:
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);

private:
	void createActions();
	void createMenu();
	void emulation();
	
	bool emuStart;
	int opcode_count;

	QString fileName;
	
	ChipEmu *emu;
	DisplayWidget *display;
	
	QMenu *fileMenu;
	QMenu *videoMenu;
	QMenu *resolutionMenu;
	QActionGroup *resolutionGroup;
	QMenu *emulationMenu;
	QMenu *helpMenu;
	
	QAction *openRomAction;
	QAction *closeRomAction;
	QAction *exitAction;
	QAction *set1xAction;
	QAction *set2xAction;
	QAction *set4xAction;
	QAction *set8xAction;
	QAction *bgColorDialogAction;
	QAction *fgColorDialogAction;
	QAction *resetEmulation;
	QAction *aboutAction;

	QString lastDir;
};

#endif
