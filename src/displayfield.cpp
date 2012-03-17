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

#include <QPainter>
#include "displayfield.h"

DisplayField::DisplayField()
{
	displayClear();
	repaint();
}

void DisplayField::displayClear()
{
	for (int i = 0; i < 2048; i++)
		screen[i] = 0;
}

void DisplayField::setScreen(unsigned char arr[64*32])
{
	for (int i = 0; i < 2048; i++)
		screen[i] = arr[i];
}

void DisplayField::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.scale ( resolution, resolution );
	//painter.setRenderHint(QPainter::Antialiasing, true);
	QBrush brush(/*Qt::black*/bgColor, Qt::SolidPattern);
	painter.fillRect(0, 0, 64, 32, brush);
	
	int t = 0;
	for (int i = 0; i < 32; i++)
		for (int j = 0; j < 64; j++)
		{
			if (screen[t++] == 1)
			{
				painter.setPen(/*Qt::white*/fgColor);
				painter.setBrush(/*Qt::white*/fgColor);
				if (resolution > 1) painter.drawRect ( j, i, 1, 1 );
				else painter.drawPoint(j, i);
			}
		}
}
