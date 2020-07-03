#include "weather.h"

WPart::WPart()
{
	x=y=dx=dy=0;
	c=r=0;
	t=0;
}

void WPart::render(BITMAP *bmp)
{
	if (t > 1) 
		return;

	if (t == 0)
		circlefill(bmp, (int)x, (int)y, r, c);
	else
	{
		drawing_mode(DRAW_MODE_TRANS, NULL,0,0); 
		line(bmp, (int)x, (int)y, (int)(x + dx*5), (int)(y + dy*5), c);
		drawing_mode(DRAW_MODE_SOLID, NULL,0,0);
	}
}



Weather::Weather()
{
}

Weather::~Weather()
{
}

void Weather::render(BITMAP *bmp)
{
	for (int i = 0; i < n; i++)
		part[i].render(bmp);
}

void Weather::update()
{
	for (int i = 0; i < n; i++)
	{
		part[i].x += part[i].dx;
		part[i].y += part[i].dy;

		if (part[i].x < 0)
			part[i].x += 640;

		if (part[i].x > 640)
			part[i].x -= 640;

		if (part[i].y > 480)
		{
			part[i].y = -rand()%15;
			part[i].x = rand()%640;
			part[i].dx = (rand()%150 / 100.0f) * w;
		}
	}
}




void Weather::generate(int t)
{
	n = 0;
	if (t > 1) 
		return;

	n = rand()%(maxP/2) + (maxP/2);
	w = rand()%3-1;
	for (int i = 0; i < n; i++)
	{
		part[i].x = rand()%640;
		part[i].y = rand()%480;

		part[i].dx = (rand()%150 / 100.0f) * w;

		part[i].dy = rand()%400 / 100.0f + 2;

		part[i].r = rand()%3+1;
		part[i].t = t;

		if (t == 0)
			part[i].c = makecol(255,255,255); // snow
		else
			part[i].c = makecol(0,128,200+rand()%55); // rain
	}
}

