/*
 *  emuChip - CHIP-8 emulator.
 *  Copyright (C) 2009  Boris Timofeev <mashin87@gmail.com>
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
#include "chip8_emu.h"
#include "displayfield.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	void readSettings();
	void writeSettings();

private slots:
	void openRom();
	void closeRom();
	void set1x();
	void set2x();
	void set4x();
	void set8x();
	void set16x();
	void ref();
	void bgColorDialog();
	void fgColorDialog();
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
	
	Chip8_Emu *emu;
	DisplayField *displayField;
	
	QMenu *fileMenu;
	QMenu *videoMenu;
	QMenu *resolutionMenu;
	QActionGroup *resolutionGroup;
	QMenu *helpMenu;
	
	QAction *openRomAction;
	QAction *closeRomAction;
	QAction *exitAction;
	QAction *set1xAction;
	QAction *set2xAction;
	QAction *set4xAction;
	QAction *set8xAction;
	QAction *set16xAction;
	QAction *bgColorDialogAction;
	QAction *fgColorDialogAction;
	QAction *aboutAction;

	QString lastDir;
};

#endif