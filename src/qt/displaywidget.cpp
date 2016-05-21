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

#include <QPainter>
#include "displaywidget.h"

DisplayWidget::DisplayWidget(unsigned char arr[128][64])
{
	screen = arr;
	clear();
	repaint();
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::clear()
{
	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 128; x++)
			screen[x][y] = 0;
}

void DisplayWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.scale ( resolution, resolution );
	QBrush brush(bgColor, Qt::SolidPattern);
	painter.fillRect(0, 0, 128, 64, brush);

	painter.setPen(fgColor);
	painter.setBrush(fgColor);

	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 128; x++)
			if (screen[x][y] == 1)
			{
				if (resolution > 1)
					painter.drawRect ( x, y, 1, 1 );
				else painter.drawPoint(x, y);
			}
}