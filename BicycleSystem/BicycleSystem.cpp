
#include <stdio.h>
#include "stdafx.h"
#define BIKES_COUNT 1024
#define BRANDS_COUNT 5
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996)

const char* newGUID(void);

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
	const char* id = newGUID();
	//元素的索引
	int index = 0;
	//转换成字符串
	const char* (*str)(struct _LinkedListNode* node);
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
	//密码
	const char* pwd;
	//手机号码
	const char* phone;
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
typedef struct _brand {
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
}Brand;

void InitializeBrands();
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
//遍历
void Traverse(LinkedList* list, void (*action)(LinkedListNode* node, void** params));
//统计
int Count(LinkedList* list, bool (*predicate)(LinkedListNode *node, void **params));
//前几名
LinkedList* TopN(LinkedList* list, int direction,
	int n, int(*comparer)(LinkedListNode* node, LinkedListNode* next));
//筛选
LinkedList* Select(LinkedList* list, bool (*predicate)(LinkedListNode* node, void** params));
//过滤
LinkedList* Filter(LinkedList* list, bool(*predicate)(LinkedListNode* node, void** params));
//映射
LinkedList* Map(LinkedList* list, LinkedListNode* (*map)(LinkedListNode* node, void** params));
//存在
bool Exists(LinkedList* list, bool(*exists)(LinkedListNode* node, void** params));
//逆序
LinkedList* Reverse(LinkedList* list);
//搜寻
LinkedListNode* Search(LinkedList* list, void* target, bool (*predicate)(LinkedListNode* node, void* target, void** params));
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

LinkedList Users, Bikes, Brands, Menus, UsersLogined;

void println(LinkedListNode* node, void** params)
{
	if(node->str==nullptr)
	{
		printf("%s\n", (const char*)node->Value);
	}
	else
	{
		printf("%s\n", node->str(node));
	}
}

const char* user_str(LinkedListNode* node) 
{
	char* buffer = (char*)malloc(sizeof(char)*256);
	User* user = (User*)node->Value;
	sprintf(buffer, "姓名：%s\t手机：%s\t密码：%s", user->name, user->phone,user->pwd);
	return buffer;
}

Brand brands[BRANDS_COUNT];

void main(void)
{
	InitializeBrands();
	Add(&Menus, "1.注册", strlen("1.注册") + 1);
	Add(&Menus, "2.登录", strlen("2.登录") + 1);
	Add(&Menus, "3.退出", strlen("3.退出") + 1);
	Traverse(&Menus, println);
	User* user = Register("韩宏乖", "Qq1361699880","13375413533");
	User* user2 = Register("陈竹筠", "Qq1361699880", "17860616702");
	Traverse(&Users, println);
	int choice = -1;
	while (choice != 0)
	{
		puts("****************欢迎来到单车管理系统****************");
		/*puts("1.  注册");
		puts("2.  登录");
		puts("3.  骑车");
		puts("4.  归还");*/
		puts("5.  单车使用情况查询");
		puts("6.  损坏车辆统计");
		puts("7.  客户骑行统计");
		puts("8.  当日使用情况统计");
		puts("9. 日营业额统计");
		puts("10. 受欢迎程度统计");
		puts("0.  退出");
		scanf("%d", &choice);
	}
}

void InitializeBrands()
{
	//品牌名称
	const char* names[] = {"哈罗","捷安特","永久","凤凰","美利达"};
	//投放量
	int counts[] = {1000, 2000, 800, 1400, 2500};
	//免费时长
	int frees[] = {60, 30, 60, 50, 60, 40};
	//起步价
	float prices_bascic[] = {2, 1.5, 2.5, 2, 2.5};
	//收费标准
	float prices[] = { 1, 0.5, 0.8, 1.2, 1.5 };
	//优惠开始时长
	int preferentials_start[] = {180, 150, 200, 240, 180};
	//优惠价格
	float prices_preferentials[] = {0.8, 0.3, 0.5, 1, 1};
	for (int i = 0; i < BRANDS_COUNT;i++) {
		brands[i].brand = names[i];
		brands[i].count = counts[i];
		brands[i].free = frees[i];
		brands[i].price_basic = prices_bascic[i];
		brands[i].price = prices[i];
		brands[i].preferential_start = preferentials_start[i];
		brands[i].preferential_price = prices_preferentials[i];
		Add(&Brands, &brands[i], sizeof(brands[i]));
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
	User* user = (User*)malloc(sizeof(User));
	user->name = name;
	user->pwd = pwd;
	user->phone = phone;
	user->bike = nullptr;
	user->location = "";
	user->mileage = 0;
	user->mileage_total = 0;
	LinkedListNode* node=Add(&Users, user, sizeof(User));
	node->str = user_str;
	return user;
}

bool validateByName(LinkedListNode* node, void* target, void** params)
{
	User* each = (User*)node->Value;
	User* search = (User*)target;
	return strcmp(each->name, search->name)==0 && strcmp(each->pwd, search->pwd) == 0;
}

User* LoginByName(const char* name, const char* pwd)
{
	LinkedListNode* node = Search(&Users, validateByName, nullptr);
	User* user = (User*)node->Value;
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
	ele->Next = nullptr;
	ele->str = nullptr;
	if (list->count == 0) 
	{
		list->Start = ele;
		ele->Prev = nullptr;
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
	LinkedList* sorted = (LinkedList*)malloc(sizeof(LinkedList));
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
		Add(sorted, minmax->Value, minmax->Size);
	}
	return sorted;
}

void Traverse(LinkedList* list, void(*action)(LinkedListNode* node, void** params))
{
	for (LinkedListNode* node = list->Start; node != nullptr; node = node->Next)
	{
		action(node, nullptr);
	}
}

int Count(LinkedList* list, bool (*predicate)(LinkedListNode *node, void **params))
{
	int count = 0;
	for (LinkedListNode* node = list->Start; node != nullptr; node = node->Next)
	{
		if (predicate(node, nullptr))
		{
			count++;
		}
	}
	return count;
}

//前几名
LinkedList* TopN(LinkedList* list, int direction,
	int n, int(*comparer)(LinkedListNode* node, LinkedListNode* next))
{
	LinkedList* topn = (LinkedList*)malloc(sizeof(LinkedList));
	int times = 0;
	for (LinkedListNode* i = list->Start; i != list->End && times<n; (i = i->Next, times++))
	{
		LinkedListNode* minmax = i;
		for (LinkedListNode* j = i->Next; j != nullptr; j = j->Next)
		{
			if (comparer != nullptr)
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
		Add(topn, minmax->Value, minmax->Size);
	}
	return topn;
}

LinkedList* Select(LinkedList* list, bool(*predicate)(LinkedListNode* node, void** params))
{
	LinkedList* selected = (LinkedList*)malloc(sizeof(LinkedList));
	for (LinkedListNode* node = list->Start; node != nullptr; node = node->Next)
	{
		if (predicate(node, nullptr))
		{
			Add(selected, node->Value, node->Size);
		}
	}
	return selected;
}

LinkedList* Filter(LinkedList* list, bool(*predicate)(LinkedListNode* node, void** params))
{
	LinkedList* selected = (LinkedList*)malloc(sizeof(LinkedList));
	for (LinkedListNode* node = list->Start; node != nullptr; node = node->Next)
	{
		if (!predicate(node, nullptr))
		{
			Add(selected, node->Value, node->Size);
		}
	}
	return selected;
}

LinkedList* Map(LinkedList* list, LinkedListNode* (*map)(LinkedListNode* node, void** params))
{
	LinkedList* mapped = (LinkedList*)malloc(sizeof(LinkedList));
	for (LinkedListNode* node = list->Start; node != nullptr; node = node->Next)
	{
		LinkedListNode* res = map(node, nullptr);
		Add(list, res->Value, res->Size);
	}
	return mapped;
}

bool Exists(LinkedList* list, bool(*predicate)(LinkedListNode* node, void** params))
{
	bool exists = false;
	for (LinkedListNode* node = list->Start; node != nullptr; node = node->Next)
	{
		if(predicate && predicate(node, nullptr))
		{
			exists = true;
			break;
		}
	}
	return predicate;
}

LinkedList* Reverse(LinkedList* list)
{
	LinkedList * reversed = (LinkedList*)malloc(sizeof(LinkedList));
	for (LinkedListNode* node = list->End; node != nullptr; node = node->Prev)
	{
		Add(reversed, node->Value, node->Size);
	}
	return reversed;
}

LinkedListNode* Search(LinkedList* list, void* target, bool(*predicate)(LinkedListNode* node, void* target, void** params))
{
	LinkedListNode *node = nullptr;
	for (;node!=nullptr;node=node->Next)
	{
		if (predicate(node, target, nullptr))
		{
			break;
		}
	}
	return node;
}
#pragma endregion

