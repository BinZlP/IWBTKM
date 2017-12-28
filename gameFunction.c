#include "gameFunc.h"

//void cleanBulletArray(Bullet* bullets) {
//	int length = sizeof(bullets) / sizeof(Bullet);
//	for (int i = 0; i < length; i++) {
//		bullets = (bullets + i);
//	}
//	bullets[length - 1].pos.x = 0;
//	bullets[length - 1].pos.y = 0;
//	bullets[length - 1].vel.x = 0;
//	bullets[length - 1].vel.y = 0;
//}

//const int laser_thickness = 15;

void initBullet(Bullet* bullets, int length) {
	for (int i = 0; i < length; i++) {
		(bullets + i)->pos.x = 0;
		(bullets + i)->pos.y = 0;
		(bullets + i)->isShot = 0;
	}
}

void shootingGun(Bullet* bullets, Player player, Vector mousePos, int length, bool isSkillBullet) {
	for (int i = 0; i < length; i++) {
		if (!bullets[i].isShot) {
			//printf("ADDING BULLET\n");
			Vector unitVector = findUnitVector(player, mousePos);
			(bullets+i)->pos.x = player.pos.x + 36;
			(bullets+i)->pos.y = player.pos.y + 14;
			if (!isSkillBullet) {
				(bullets + i)->pos.w = bulletSize;
				(bullets + i)->pos.h = bulletSize;
			}
			else {
				(bullets + i)->pos.w = bulletSize*3;
				(bullets + i)->pos.h = bulletSize*3;
			}
			(bullets+i)->vel.x = unitVector.x*bulletSpeed;
			(bullets+i)->vel.y = unitVector.y*bulletSpeed;
			(bullets+i)->isShot = true;
			i = length;
		}
	}
}

void bulletDestroy(Bullet* bullets, int index, bool isSkillBullet) {
	bullets[index].pos.x = 0;
	bullets[index].pos.y = 0;
	if (!isSkillBullet) {
	
		bullets[index].pos.w = bulletSize;
		bullets[index].pos.h = bulletSize;
	}
	else {
		bullets[index].pos.w = bulletSize*3;
		bullets[index].pos.h = bulletSize*3;
	}
	
	bullets[index].vel.x = 0;
	bullets[index].vel.y = 0;

	bullets[index].isShot = false;
}

double calcDistanceI(int x1, int y1, int x2, int y2) {
	//printf("%lf\n", sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1))));
	return sqrt((double)((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)));
}
