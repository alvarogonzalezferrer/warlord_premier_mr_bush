#include "baby.h"


BabyList::BabyList()
{
	n = 0;
}

BabyList::~BabyList()
{
	for(int i =0; i < n;i++)
		delete(baby[i]);
}

void BabyList::generate(int i)
{
	for(int z=0;z < n;z++) // first release old
		delete(baby[z]);


	if (i < 0 || i > maxBL-1)
		i = maxBL-1;
	n = i;

	for(int z=0; z < n;z++)
		baby[z] = new Baby();
	
}

int BabyList::shoot(int x, int y,ParticleManager *pm)
{
	int ret = 0;
	for(int i = 0; i < n;i++)
	{
		if (baby[i]->life > 0)
		{
			if (baby[i]->x < x && 
				baby[i]->y < y && 
				baby[i]->x + baby[i]->sprite->w  > x &&
				baby[i]->y + baby[i]->sprite->h  > y)
			{

				// blood
				for (int j =0;j < rand()%15+15; j++)
					pm->add(new Blood(x, y, (float)(rand()%200-100)/100.0f, (float)(rand()%300+100)/-100.0f, rand()%15+25));

				baby[i]->life -= rand()%55+20;

				play_sample(Baby::babySND, rand()%128+128, x*255/640, rand()%500+950, 0);

				if (baby[i]->life < 0)
					ret += rand()%60+30;
			
			break; // shoots don't kill more than 1 baby, no piercing bullets
			}
			
		}
	}
	return ret;
}

int BabyList::grenade(int x, int y, ParticleManager *pm)
{
	int rad = 55+rand()%15; // rad grenade
	int ret = 0;
	for(int i =0; i < n;i++)
	{
		if (baby[i]->life > 0)
		{
			
			baby[i]->sy = -(rand()%8+6); // all babies jump when a grenade explodes

			// someone inside the blast?
			if (baby[i]->x < x+rad && 
				baby[i]->y < y+rad && 
				baby[i]->x + baby[i]->sprite->w  > x-rad &&
				baby[i]->y + baby[i]->sprite->h  > y-rad)
			{

				// blood
				for (int j =0;j < rand()%35+35; j++)
					pm->add(new Blood(x, y, (float)(rand()%300-150)/100.0f, (float)(rand()%400+300)/-100.0f, rand()%60+45));


				
				baby[i]->life -= 75 + ABS(100-ABS(x - baby[i]->x)); // proportional to distance... lame hack

				play_sample(Baby::babySND, rand()%128+128, x*255/640, rand()%500+950, 0);

				if (baby[i]->life < 0)
				{
					ret += rand()%60+30;
					baby[i]->jibbed = true;
					// add jibs
					for(int j=0; j < 3; j++)
					pm->add(new RotationalSpriteParticle (x, 
														  y, 
														  (float)(rand()%300-150)/100.0f, 
														  (float)(rand()%400+200)/-100.0f, 
														  rand()%255, 
														  rand()%3+1, 
														  rand()%120+120, 
														  Baby::jib[j]));
				}
			}
		}
	}
	return ret;
}

int BabyList::update(Map *m)
{
	int ret=0;
	for(int i =0; i < n;i++)
	{
		baby[i]->update(m);
		if (baby[i]->life > 0)
			ret++;
	}
	return ret;
}

void BabyList::render(BITMAP *bmp)
{
	for(int i =0; i < n;i++)
		baby[i]->render(bmp);
}

// --------- baby
BITMAP *Baby::walk[3];
BITMAP *Baby::jib[3];
BITMAP *Baby::dead = NULL;
SAMPLE *Baby::babySND = NULL;
Datafile Baby::data;
int Baby::count=0;


Baby::Baby()
{
	if (count == 0)
	{
		data.load("baby.dat");
		walk[0] = (BITMAP *)data.getObject("WALK_0")->dat;
		walk[1] = (BITMAP *)data.getObject("WALK_1")->dat;
		walk[2] = (BITMAP *)data.getObject("WALK_2")->dat;
		jib[0] = (BITMAP *)data.getObject("JIB_0")->dat;
		jib[1] = (BITMAP *)data.getObject("JIB_1")->dat;
		jib[2] = (BITMAP *)data.getObject("JIB_2")->dat;
		dead = (BITMAP *)data.getObject("DEAD")->dat;
		babySND = (SAMPLE *)data.getObject("BABYSND")->dat;
	}

	sprite = walk[0];

	count++;

	y = Map::mapH*0.9;
	sy = 0;
	sx = rand()% sprite->w / 10 + sprite->w / 20;

	state = rand()%2;
	life = rand()%30+70;
	jibbed = false;

	anm = rand()%40;

	if (state == LEFT)
	{
		x = Map::mapW + sprite->w * (rand()%5 + 1);
		sx = -sx;
	}

	if (state == RIGHT)
	{
		x = -sprite->w * (rand()%5 + 1);
	}
}

Baby::~Baby()
{
	count--;
	if (count == 0)
	{
		data.unload();
	}
}

void Baby::update(Map *m)
{

	if (++anm > 40) 
		anm = 0;		

//	y = m->getHeight((int)(x + sprite->w/2)) - sprite->h *0.95; // fall to ground
	y += sy; // fall to ground

	if (y > m->getHeight((int)(x + sprite->w/2)) - sprite->h *0.95)
	{
		y = m->getHeight((int)(x + sprite->w/2)) - sprite->h *0.95;
		sy = 0;
	}
	else
		sy += 0.80; // gravity / another friction force? maybe :P

	if (life <= 0)
		return; // dead, we don't move anymore...

	// friction
	float f = 1;
	if (m->tm == Map::SNOW)
		f = 1.10;

	if (m->tm == Map::ROCK)
		f = 0.9;

	x += sx*f;
	

	// randomly turn around harder to aim!
	if (rand()%1000 < 5)
	{
		if (sx < 0)
		{
			sx = -sx;
			state = RIGHT;
		}
		else
		{
			sx = -sx;			
			state = LEFT;
		}
	}

	// jump? maybe... harder to aim!
	if (sy < 1 && rand()%1000<10)
		sy = -(rand()%4+4);

	// bounce on borders
	switch(state)
	{
		case LEFT:
			if (x < -sprite->w*2)
			{
				state = RIGHT;
				sx = rand()% sprite->w / 10 + sprite->w / 20;
				life *= 1.1; // heal
			}
		break;
	
		case RIGHT:
			if (x > Map::mapW + sprite->w*2)
			{
				state = LEFT;
				sx = -(rand()% sprite->w / 10 + sprite->w / 20);
				life *= 1.1; // heal
			}
		break;
	}

}

void Baby::render(BITMAP *bmp)
{
	if (jibbed)
		return; // dead

	sprite = walk[0];

	if (anm > 10)
		sprite = walk[1];
	if (anm > 20)
		sprite = walk[2];
	if (anm > 30)
		sprite = walk[1];

	if (life < 1)
		sprite = dead;

	switch(state)
	{
		case LEFT:
			draw_sprite_h_flip(bmp, sprite, (int)x, (int)y);
		break;
	
		case RIGHT:
		default:
			draw_sprite(bmp, sprite, (int)x, (int)y);
		break;
	}

	//textprintf_ex(bmp, font, (int)x, (int)y, makecol(255,255,255), -1, "%d", life);
}

