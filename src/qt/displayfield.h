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

#ifndef DISPLAYFIELD_H
#define DISPLAYFIELD_H

#include <QWidget>

class DisplayField : public QWidget
{
	Q_OBJECT

public:
	DisplayField();
	
	void setResolution(int r) { resolution = r; }
	int getResolution() { return resolution; }
	void setBgColor(QColor color) { bgColor = color; }
	void setFgColor(QColor color) { fgColor = color; }
	QColor getBgColor() { return bgColor; }
	QColor getFgColor() { return fgColor; }

	void setScreen(unsigned char [64*32]);
	void displayClear();

protected:
	void paintEvent(QPaintEvent *event);

private:
	int resolution;

	QColor bgColor;
	QColor fgColor;
	
	unsigned char screen[64*32];
};

#endif
