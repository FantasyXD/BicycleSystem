#include <stdio.h>
#define BIKES_COUNT 1024

typedef struct _bike {
	char* brand;
	int total;
	char* place;
	int useage_time;
	int usgae_day;
	float lichen;
	float total_lichen;
	int damage_unlock;
} Bike;

Bike bikes[BIKES_COUNT];

char *(brands[]) = { "捷安特", "永久", "美利达", "凤凰", "喜德盛" };
char *(places[]) = { "青州市图书馆", "山东省博物馆", "水立方鸟巢", "齐鲁软件园","山东省体育馆" };

void InitializeBikes();

void main()
{

}

void InitializeBikes()
{
	for (int i = 0; i<BIKES_COUNT; i++) {

	}
}