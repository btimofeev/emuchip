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

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGETH

#include <QWidget>

class DisplayWidget : public QWidget
{
	Q_OBJECT

public:
	DisplayWidget();
	~DisplayWidget();
	
	void setResolution(int r) { resolution = r; }
	int getResolution() { return resolution; }
	void setBgColor(QColor color) { bgColor = color; }
	void setFgColor(QColor color) { fgColor = color; }
	QColor getBgColor() { return bgColor; }
	QColor getFgColor() { return fgColor; }

	void setScreen(unsigned char [128][64]);
	void clear();

protected:
	void paintEvent(QPaintEvent *event);

private:
	int resolution;

	QColor bgColor;
	QColor fgColor;
	
	unsigned char screen[128][64];
};

#endif
