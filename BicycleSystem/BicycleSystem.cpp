#include <stdio.h>
#include "stdafx.h"
#define BIKES_COUNT 1024
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef enum _BikeStatus {
	//未知状态
	unknown,
	//备用状态
	sparing,
	//开锁状态
	unlocking,
	//骑行状态
	riding,
	//上锁状态
	locking,
	//损坏状态
	damaging,
	//维修状态
	maintaining,
} BikeStatus;

typedef struct _bike {
	//单车品牌
	const char* brand;
	//投放地点
	const char* place_original;
	//单车id
	const char* id;
	//被使用的次数
	int used_count = 0;
	//开锁失败的次数
	int unlock_failed = 0;
	//单日累计的骑行里程
	int mileage = 0;
	//单车目前所在地
	const char* place;
	//单车状态
	BikeStatus status = BikeStatus::sparing;
	//当前骑行的用户
	User* user = nullptr;
} Bike;

typedef struct _user {
	//用户名
	char* name;
	//手机号码
	char phone[11];
	//累计骑行里程
	float mileage_total = 0;
	//当日骑行里程
	float mileage = 0;
	//当前骑行的自行车
	Bike* bike = nullptr;
} User;

typedef struct _bike_commercial {
	//单车品牌
	const char* brand;
	//投放量
	int count = 1000;
	//免费时长
	int free = 60;
	//收费标准
	float price = 1;
	//优惠开始时长
	float preferential_start = 3;
	//优惠价格
	float preferential_price = 0.5;
	//受欢迎程度
	int popular = 0;
	//日营业额
	float turnover = 0;
	//当日使用数量
	int usage_count = 0;
	//目前使用数量
	int usage_count_current = 0;
}BikeCommercial;

Bike bikes[BIKES_COUNT];

char *(brands[]) = { "捷安特", "永久", "美利达", "凤凰", "喜德盛" };
char *(places[]) = { "青州市图书馆", "山东省博物馆", "水立方鸟巢", "齐鲁软件园","山东省体育馆" };

void InitializeBikes();
int randint(int m, int n);
const char* newGUID(void);

#pragma region 用户模块

#pragma endregion


void main()
{
	_gettch();
}

void InitializeBikes()
{
	int brand=0, place = 0, total = 0;
	for (int i = 0; i<BIKES_COUNT; i++) 
	{
		brand = randint(0, 5);
		bikes[i].brand = brands[brand];
		place = randint(0, 5);
		bikes[i].place_original = places[place];
		bikes[i].id = newGUID();
	}
}

int randint(int m, int n)
{
	return rand() % (n - m + 1) + m;
}

const char* newGUID(void)
{
	srand(time(NULL));
	static char buf[64] = { 0 };
	snprintf(buf, sizeof(buf),
		"{%08X-%04X-%04X-%04X-%04X%04X%04X}",
		rand() & 0xffffffff,
		rand() & 0xffff,
		rand() & 0xffff,
		rand() & 0xffff,
		rand() & 0xffff, rand() & 0xffff, rand() & 0xffff
	);
	return (const char*)buf;
}