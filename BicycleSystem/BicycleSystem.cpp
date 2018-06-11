#include <stdio.h>
#include "stdafx.h"
#define BIKES_COUNT 1024
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

//链表元素
typedef struct _LinkedListNode {
	//元素地址
	void* Value;
	//下一个元素
	struct _LinkedListNode* Next;
	//上一个元素
	struct _LinkedListNode* Prev;
	//元素大小
	int Size;
	//元素id
	const char* id;
	//元素的索引
	int index = 0;
} LinkedListNode;

//链表结构
typedef struct _LinkedList {
	//开始元素
	LinkedListNode* Start = nullptr;
	//结束元素
	LinkedListNode* End = nullptr;
	//元素个数
	int count = 0;
	//当前的元素
	LinkedListNode* Current = nullptr;
	//当前的索引
	int index = -1;
}LinkedList;
//单车状态
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
//单车实例
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
	struct _user *user = nullptr;
} Bike;
//用户实例
typedef struct _user {
	//用户名
	const char* name;
	//手机号码
	const char phone[11];
	//累计骑行里程
	float mileage_total = 0;
	//当日骑行里程
	float mileage = 0;
	//当前骑行的自行车
	Bike* bike = nullptr;
	//用户所在位置
	const char* location = nullptr;
} User;
//品牌信息
typedef struct _bike_commercial {
	//单车品牌
	const char* brand;
	//投放量（台）
	int count = 1000;
	//免费时长（分钟）
	int free = 60;
	//免费之后的起步价（元）
	float price_basic = 2;
	//收费标准（元/时）
	float price = 1;
	//优惠开始时长（分钟）
	int preferential_start = 180;
	//优惠价格（元/时）
	float preferential_price = 0.5;
	//受欢迎程度
	int popular = 0;
	//日营业额（元）
	float turnover = 0;
	//当日使用数量（台）
	int usage_count = 0;
	//目前使用数量（台）
	int usage_count_current = 0;
}BikeCommercial;

LinkedList Users, Bikes, Brands;

//预制品牌和地点
char *(brands[]) = { "捷安特", "永久", "美利达", "凤凰", "喜德盛" };
char *(places[]) = { "青州市图书馆", "山东省博物馆", "水立方鸟巢", "齐鲁软件园","山东省体育馆" };

void InitializeBikes();
int randint(int m, int n);
const char* newGUID(void);

#pragma region 链表模块
//增
LinkedListNode* Add(LinkedList* list, void* node, int size);
//删
LinkedListNode* Remove(LinkedList* list, void* node);
//改
LinkedListNode* Update(LinkedList* list, void* node_src, void* node_dest);
//查
LinkedListNode* Query(LinkedList* list, void* node);
//排序
LinkedList* Sort(LinkedList* list, int direction, 
	int (*comparer)(LinkedListNode* node, LinkedListNode* next));
//交换
LinkedList* Swap(LinkedList* list, void* node1, void* node2);
//交换头尾
LinkedList* SwapStartEnd(LinkedList* list);
#pragma endregion


#pragma region 用户模块
//用户注册
User* Register(const char* name, const char* pwd, const char* phone);
//通过名称登录
User* LoginByName(const char* name, const char* pwd);
//通过手机号登录
User* LoginByPhone(const char* phohne, const char* pwd);
//注销用户
void Logout(User* user);
//用户充值
void Recharge(User* user, float money);
//使用单车
Bike* BeginRide(User* user, const char* id);
//归还单车并返回扣款金额
float EndRide(User* user);
//用户骑着自行车到某一地，返回骑行里程
float RideTo(User* user, const char* place);
//用户将自行车骑回投放地，返回骑行里程
float RideBackToOriginal(User* user);
//随机选择某个品牌的可用车
Bike* ChooseSepcBikeRandomly(const char* brand);
//随机选择可用车
Bike* ChooseBikeRandomly();
//用户报修车
void RepairBike(const char* id);
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

#pragma region 用户模块_实现
User* Register(const char* name, const char* pwd, const char* phone)
{
	User* user = nullptr;
	return user;
}
#pragma endregion


#pragma region 链表模块_实现
LinkedListNode* Add(LinkedList* list, void* node, int size)
{
	LinkedListNode* ele = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	ele->id = newGUID();
	ele->index = list->count;
	ele->Size = size;
	ele->Value = node;
	if (list->count == 0) 
	{
		list->Start = ele;
	}
	else 
	{
		ele->Prev = list->End;
		list->End->Next = ele;
	}
	list->End = ele;
	list->Current = ele;
	list->count++;
	return ele;
}

LinkedListNode* Query(LinkedList* list, void* node)
{
	LinkedListNode* ele = list->Start;
	while (ele!=nullptr) 
	{
		if (node == ele->Value) 
		{
			break;
		}
		ele = ele->Next;
	}
	return ele;
}

LinkedListNode* Remove(LinkedList* list, void* node)
{
	LinkedListNode *ele = Query(list, node);
	if (ele!=nullptr)
	{
		ele->Prev->Next = ele->Next;
		ele->Next->Prev = ele->Prev;
		ele = nullptr;
		list->count--;
	}
	return ele;
}

LinkedListNode* Update(LinkedList* list, void* node_src, void* node_dest)
{
	LinkedListNode *ele = Query(list, node_src);
	if (ele != nullptr) 
	{
		ele->Value = node_src;
	}
	return ele;
}

LinkedList* Sort(LinkedList* list, int direction,
	int(*comparer)(LinkedListNode* node, LinkedListNode* next))
{
	for (LinkedListNode* i = list->Start; i != list->End; i=i->Next)
	{
		LinkedListNode* minmax = i;
		for (LinkedListNode* j = i->Next; j!=nullptr;j=j->Next) 
		{
			if (comparer!=nullptr)
			{
				switch (comparer(j, minmax))
				{
				case 0:
					break;
				case 1:
					if (direction == -1) 
					{
						minmax = j;
					}
					break;
				case -1:
					if (direction == 1) 
					{
						minmax = j;
					}
					break;
				default:
					break;
				}
			}
		}

	}
	return list;
}

LinkedList* Swap(LinkedList* list, void* node1, void* node2)
{
	LinkedListNode* n1 = Query(list, node1);
	LinkedListNode* n2 = Query(list, node2);
	{
		if (n1->Next!=n2 && n2->Prev!=n1)
		{
			n1->Prev->Next = n2;
			n2->Next->Prev = n1;
		}
		LinkedListNode* tn = nullptr;
		tn = n1->Next;
		n1->Next = n2->Next;
		if (n2 == list->End)
		{
			list->End = n1;
		}
		n2->Next = tn;
		LinkedListNode* tp = nullptr;
		tp=n1->Prev;
		n1->Prev = n2->Prev;
		if (n1 == list->Start)
		{
			list->Start = n2;
		}
		n2->Prev = tp;
	}
	return list;
}

#pragma endregion
