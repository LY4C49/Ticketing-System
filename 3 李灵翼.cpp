//本系统使用VS2019开发，若直接运行提示fopen报错，请按如下步骤操作
//属性-预处理器-预处理器编辑-添加：_CRT_SECURE_NO_WARNINGS
//根据需要添加#include<pch.h>
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<iomanip>
#include<conio.h>
#include<cstring>
#include<fstream>
#include<Windows.h>
using namespace std;

//控制台函数，用于更改输出字体、背景颜色
BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttributes);
}

//航班类
class plane {
	friend class machine;
private:
	string seat[80];//所有座位编号数组
	string row[20] = { "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20" };//行编号
	string column[4] = { "A","B","C","D" };//列编号
	int k = 0;//用来创建seat数组时计数
	string plane_num;//航班号
	string des;//目的地
	string sous;//出发地
	string seat_occupy[80];//已被购买的座位编号数组
	plane* next;//链表，下一个航班
public:
	plane();//构造函数，管理员添加航班时使用
	plane(string a, string b, string c);//构造函数，系统初始化从txt文件读取创建航班使用
	void showseat();//显示该航班座位状态
	bool check_available(string seat);//判断seat是否被购买。空闲返回true,已购返回false；被showseat()调用
};

//乘客类
class passenger {
	friend class machine;
private:
	string name;//姓名
	string id;//身份证号
	string flight;//航班号
	string seat;//座位号
	passenger* next;//链表
public:
	passenger();//构造函数，用于乘客购票
	passenger(string a, string b, string c, string d);//构造函数，用于系统初始化
	void display_passengerinfo();//显示乘客信息
};

//管理员类
class administrator {
	friend class machine;
	friend void login();
private:
	string username;//用户名
	string password;//密码
public:
	administrator(string name, string pass);//构造函数，创建管理员
	bool check_id(string name, string pass);//验证用户名、密码是否正确。正确返回1，否则返回0
};

administrator adm1 = administrator("admin", "admin");//创建一个管理员对象；用户名：admin，密码:admin

//系统类
class machine {
	//passenger、plane为友类，可以访问私有成员
	friend class passenger;
	friend class plane;
private:
	//航班，乘客的头、尾链表地址指针
	plane* first_plane;
	plane* last_plane;
	passenger* first_passenger;
	passenger* last_passenger;

public:
	machine();//默认的构造函数
	void addplane();//管理员添加航班
	void addpassenger();//乘客购票函数
	void addpassenger(string a, string b, string c, string d);//系统从txt初始化乘客
	void addplane(string a, string b, string c);//系统从txt初始化航班
	bool checkenable(string flight, string seat);//购票，如果该座位空闲，则购买并返回true;其余返回false,并显示原因
	bool checkenable(string a, string b, string c, string d);//系统从txt初始化时购票
	void print_all_passenger();//显示所有乘客信息供管理员查看
	void print_passenger_info();//显示特定乘客信息供乘客自己查询
	void show_available_seat(string flight);//显示该航班所有座位状态
	void change_plane();//改签航班
	void refund();//退票
	void del_plane();//管理员删除航班
	void addplane_file();//从txt文件添加航班
	void addpassenger_file();//从txt文件添加乘客
	void writePAtofile();//乘客信息写入txt文件
	void writePLtofile();//航班信息写入txt文件
};
machine ma = machine();//创建machine对象

plane::plane() {
	cout << "请输入航班号:" << endl;
	cin >> plane_num;
	for (int i = 0; i <= 79; i++) {
		seat_occupy[i] = "a";//初始化时，全部标a，避免空数组可能产生的bug
	}
	for (int i = 0; i <= 19; i++) {
		for (int j = 0; j <= 3; j++) {
			//创建所有座位的数组编号
			seat[k] += row[i];
			seat[k] += column[j];
			k++;
		}
	}
	cout << "请输入出发地：" << endl;
	cin >> sous;
	cout << "请输入目的地：" << endl;
	cin >> des;
}

plane::plane(string a, string b, string c) {
	plane_num = a;
	sous = b;
	des = c;
	for (int i = 0; i <= 79; i++) {
		seat_occupy[i] = "a";
	}
	for (int i = 0; i <= 19; i++) {
		for (int j = 0; j <= 3; j++) {
			seat[k] += row[i];
			seat[k] += column[j];
			k++;
		}
	}

}

//判断座位状态
bool plane::check_available(string seat) {
	for (int i = 0; i <= 79; i++) {
		if (seat == seat_occupy[i]) {
			return false;//被购买
		}
	}
	return true;//空座位
}

//展示该航班所有座位情况
void plane::showseat() {
	cout << "\t\t" << "    " << "A" << "   " << "B" << "   " << "C" << "   " << "D" << endl;
	int m = 0;
	for (int i = 0; i <= 19; i++) {
		if (i >= 9) {
			cout << "\t\t" << i + 1 << " ";
		}
		else {
			cout << "\t\t" << i + 1 << "  ";
		}
		for (int j = 0; j <= 3; j++) {
			if (check_available(seat[m]) == 1) {
				cout << "√" << "  ";
			}
			else {
				SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				cout << "×" << "  ";
				SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			}
			m++;
		}
		cout << "\n";
	}
}

//添加乘客
passenger::passenger() {
	cout << "请输入您的姓名:";
	cin >> name;
	cout << "请输入您的身份证号:";
	string a;
	char ch = _getch();
	//只能输入0~9或者X；非法字符拒绝录入并蜂鸣器报警一声
	while (1) {
		while (ch != '\n' && ch != '\r') {
			if ((ch <= '9' && ch >= '0') || ch == 'X') {
				_putch(ch);
				a += ch;
				ch = _getch();
			}
			else {
				//输入为退格键时，删除一个字符
				if (ch == '\b') {
					if (a.length() != 0) {
						cout << "\b \b";
						a.resize(a.length() - 1);
						ch = _getch();
					}
				}
				else {
					cout << "\a ";
					ch = _getch();
				}
			}
		}
		//长度必须为18位，否则重新输入
		if (a.length() == 18) {
			id = a;
			break;
		}
		else {
			SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			cout << "长度错误，请检查后重试" << endl;
			SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			cout << "\n请输入您的身份证号:";
			a = "";
			ch = _getch();
		}
	}
	cout << '\n';
	cout << "请输入您想购买的航班" << endl;
	string b;
	cin >> flight;
	b = flight;
	ma.show_available_seat(b);//展示该航班座位状态，方便选座
	cout << "请输入您想买的座位号" << endl;
	cin >> seat;
}

//系统从txt初始化乘客时创建乘客对象使用
passenger::passenger(string a, string b, string c, string d) {
	name = a;
	id = b;
	flight = c;
	seat = d;
}

void passenger::display_passengerinfo() {
	cout << setw(20) << name << "\t\t    " << setw(20) << id << " " << setw(10) << flight << "        " << setw(10) << seat << endl;
}

administrator::administrator(string name, string pass) {
	username = name;
	password = pass;
}

bool administrator::check_id(string name, string pass) {
	if ((name == username) && (pass == password)) {
		return true;
	}
	else {
		return false;
	}
}

#define WRITE_PA "passenger.txt"
#define WRITE_PL "plane.txt"
#define READ_PA "passenger.txt"
#define READ_PL "plane.txt"

//构造函数
machine::machine() {
	//初始化时，航班、乘客链表头尾均为空指针
	first_passenger = NULL;
	first_passenger = NULL;
	last_plane = NULL;
	last_passenger = NULL;
}

//展示指定航班座位情况
void machine::show_available_seat(string flight) {
	plane* t = first_plane;
	//根据航班号查找该航班
	while (t) {
		if (t->plane_num == flight)
			break;
		t = t->next;
	}
	if (!t) {
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "未找到该航班！" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		return;
	}
	SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	cout << "\n航班号:" << setw(10) << t->plane_num << "        座位分布如图" << "\n";
	cout << "√代表空余，×代表已有人。19,20排为吸烟区！\n";
	SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	t->showseat();//调用该航班对象的showseat()方法
}

//展示指定乘客的机票信息
void machine::print_passenger_info() {
	string id;
	passenger* t = first_passenger;
	cout << "请输入您的身份证号：";
	cin >> id;
	cout << "\n\n";
	int n = 0;//记录共找到符合条件的信息数
	//根据ID查找该乘客
	//乘客可能购买多张机票，故显示该乘客全部信息
	//循环遍历所有乘客
	while (t) {
		if ((t->id) == id) {
			n++;
			SetConsoleColor(FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			cout << "\t\t姓名" << "                     " << "身份证号" << "             " << "航班号" << "            " << "座位号" << endl;
			SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			t->display_passengerinfo();//调用该乘客对象的display_passengerinfo方法
			cout << "\n";
			t = t->next;
		}
		else { t = t->next; }

	}
	if (!t) {
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n\n查找完毕，已找到共" << n << "条信息！\n" << endl;
		cout << "为了您的隐私安全，请查阅完毕后按Y清除信息！" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		char y;
		while (true) {
			cin >> y;
			switch (y) {
			case 'y':system("cls"); return;
			case 'Y':system("cls"); return;
			default:cout << "无效输入！" << endl;
			}
		}
		return;
	}

}

//管理员查看所有乘客信息
void machine::print_all_passenger() {
	passenger* t = first_passenger;
	//循环遍历所有乘客
	while (t) {
		SetConsoleColor(FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\t\t姓名" << "                     " << "身份证号" << "             " << "航班号" << "            " << "座位号" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		t->display_passengerinfo();//调用该乘客对象的display_passengerinfo方法
		t = t->next;
	}
}

//购票函数
//查询该座位是否允许购买
//空闲则购买并标记为已购买状态，返回true;其余情况则返回false,并显示原因
bool machine::checkenable(string flight, string seat) {
	plane* t = first_plane;
	string a = "a";//标记为a则代表空闲
	//找到指定航班
	while (t) {

		if (t->plane_num == flight)
			break;
		t = t->next;
	}
	if (!t) {
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "未找到要买的航班！" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		return false;
	}
	//遍历seat_occupy数组判断该座位是否空闲
	for (int i = 0; i <= 79; i++) {
		if (t->seat_occupy[i] == seat) {
			SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			cout << "位置已有人" << endl;
			SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			return false;
		}
	}
	//若是空闲，则将该座位号写入该航班对象的seat_occupy数组中
	for (int i = 0; i <= 79; i++) {
		if (t->seat_occupy[i] == a) {
			t->seat_occupy[i] = seat;
			SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			cout << "选座成功" << endl;
			SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			return true;
		}
	}
	SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	cout << "航班已满人" << endl;
	SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	return false;
}

//同理，用于系统初始化时，初始化乘客时使用的购票函数
bool machine::checkenable(string a, string b, string c, string d) {
	plane* t = first_plane;
	string e = "a";
	while (t) {
		if (t->plane_num == c)
			break;
		t = t->next;
	}
	if (!t) {
		return false;
	}
	for (int i = 0; i <= 79; i++) {
		if (t->seat_occupy[i] == d) {
			return false;
		}
	}
	for (int i = 0; i <= 79; i++) {
		if (t->seat_occupy[i] == e) {
			t->seat_occupy[i] = d;
			return true;
		}
	}
	return false;
}

//管理员添加航班
void machine::addplane() {
	plane* t = new plane;
	plane* p = first_plane;
	plane* temp = NULL;//用于链表结构中，删除对象时避免出现错误使用的辅助指针
	while (p) {
		if (p->plane_num == t->plane_num)
		{
			SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			cout << "\n航班已存在！(如需修改，请先删除再重新录入)" << endl;
			SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			temp = t;
			temp->next = t->next;
			delete t;
			return;
		}
		p = p->next;
	}
	if (first_plane == NULL)
	{
		first_plane = last_plane = t;

	}
	else
	{
		last_plane->next = t;
		last_plane = last_plane->next;
	}
}

//系统从txt文件中初始化航班
void machine::addplane_file() {
	string a;//航班号
	string b;//出发地
	string c;//目的地
	plane* p = first_plane;
	FILE* fw = fopen("plane.txt", "a+");//若不存在该文件，则创立plane.txt文件
	fclose(fw);//关闭文件
	ifstream myfile("plane.txt");//标准文件输入流
	if (!myfile.is_open()) {
		cout << "文件未打开" << endl;
	}
	while (!myfile.eof()) {
		//文件中以%s\t%s\t%s\n的格式存储航班号，出发地，目的地
		getline(myfile, a, '\t');
		getline(myfile, b, '\t');
		getline(myfile, c, '\n');
		//避免因文件内容的格式问题创建出信息为空的航班对象
		if (a == "") {
			continue;
		}
		addplane(a, b, c);
	}
	myfile.close();
	cout << "初始化航班成功" << endl;
}

void machine::addpassenger_file() {
	string a;
	string b;
	string c;
	string d;
	passenger* p = first_passenger;
	FILE* fw = fopen("passenger.txt", "a+");//若不存在该文件，则创建passenger.txt文件
	fclose(fw);//关闭文件
	ifstream myfile("passenger.txt");//标准文件输入流
	if (!myfile.is_open()) {
		cout << "文件未打开" << endl;
	}
	while (!myfile.eof()) {
		//文件中以%s\t%s\t%s\t%s\n的格式存储姓名、身份证号、航班号、座位号

		getline(myfile, a, '\t');

		getline(myfile, b, '\t');

		getline(myfile, c, '\t');

		getline(myfile, d, '\n');
		//避免因为文件内容的格式问题造成创建数据为空的对象
		if (a == "")
		{
			continue;
		}
		addpassenger(a, b, c, d);

	}
	myfile.close();
	cout << "初始化乘客成功！" << endl;
	cout << "2s后进入系统！" << endl;
	Sleep(2000);
	system("cls");

}

//乘客购票函数
void machine::addpassenger() {
	passenger* t = new passenger;
	passenger* temp = NULL;//链表结构删除对象时用的辅助指针，避免出错
	if (checkenable(t->flight, t->seat) == true) {
		if (first_passenger == NULL)
		{
			first_passenger = last_passenger = t;

		}
		else
		{
			last_passenger->next = t;
			last_passenger = last_passenger->next;
		}
	}
	else {
		cout << "请重新操作" << endl;
		temp = t;
		temp->next = t->next;
		delete t;
		return;
	}
	cout << "3s后返回系统！" << endl;
	Sleep(3000);
	system("cls");

}

//系统初始化乘客时用于添加乘客
void machine::addpassenger(string a, string b, string c, string d) {
	passenger* t = new passenger(a, b, c, d);
	if (checkenable(a, b, c, d) == true) {
		if (first_passenger == NULL)
		{
			first_passenger = last_passenger = t;

		}
		else
		{
			last_passenger->next = t;
			last_passenger = last_passenger->next;
		}
	}
	else {
		//这个分支理论不可能进入！
		delete t;
		return;
	}

}

//系统初始化航班时用于添加航班
void machine::addplane(string a, string b, string c) {
	plane* t = new plane(a, b, c);
	if (first_plane == NULL) {
		first_plane = last_plane = t;
	}
	else {
		last_plane->next = t;
		last_plane = last_plane->next;
	}
}

//改签函数
void machine::change_plane() {
	string id;
	string plane_num;
	string plane_want;
	string seat_want;
	string e;//临时存放原座位号
	cout << "\n请输入您的身份证号:";
	cin >> id;
	cout << "\n请输入您要改签的航班:";
	cin >> plane_num;
	cout << "\n请问您想改签至哪个航班:";
	cin >> plane_want;
	ma.show_available_seat(plane_want);
	cout << "\n请输入您想购买的座位号:";
	cin >> seat_want;

	passenger* t = first_passenger;
	plane* p = first_plane;

	//根据ID和航班号找到指定乘客
	while (t) {
		if ((t->id == id) && (t->flight == plane_num)) {
			e = t->seat;
			break;
		}
		t = t->next;
	}
	if (!t) {
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n未找到您的机票信息！" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		return;
	}
	//根据plane_want和seat_want，调用购票函数。
	//成功购票则将该乘客信息中的航班号和座位号更新
	if (checkenable(plane_want, seat_want) == 1) {
		t->flight = plane_want;
		t->seat = seat_want;
		//根据输入的原航班号和原座位号，将该航班的座位由已被购买改为空闲
		while (p) {
			if (p->plane_num == plane_num) {
				for (int i = 0; i <= 79; i++) {
					if (p->seat_occupy[i] == e) {
						p->seat_occupy[i] = "a";//将该座位号在seat_occupy数组中改为a
						break;
					}
				}
			}
			p = p->next;
		}
		cout << "改签成功！" << endl;
		cout << "3s后返回主菜单！" << endl;
		Sleep(3000);
		system("cls");
	}
	else {
		return;
	}

}

//退票函数
void machine::refund() {
	string id;
	string plane_want_refund;
	cout << "\n请输入您的身份证号:";
	cin >> id;
	cout << "\n请输入您想退票的航班号:";
	cin >> plane_want_refund;
	//c,d暂存航班号和座位号
	string c;
	string d;
	plane* t = first_plane;
	passenger* p = first_passenger;
	passenger* help = NULL;
	//根据id和plane_want_refund查找该乘客
	while (p) {
		if ((p->id == id) && (p->flight == plane_want_refund)) {
			c = p->flight;
			d = p->seat;
			break;
		}
		else {
			help = p;
			p = p->next;
		}
	}
	if (!p) {
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n未找到相关信息！" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "3s后返回主菜单！" << endl;
		Sleep(3000);
		system("cls");
		return;
	}

	//找到该乘客则执行退票的步骤，该if else用于退票
	if (!help) {
		first_passenger = first_passenger->next;
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n退票成功!" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		delete p;
	}
	else {
		help->next = p->next;
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n退票成功!" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		delete p;
	}
	//退票步骤结束

	//根据c中的航班号找到该航班，根据d中座位号将该座位从已购买变为空闲
	while (t) {
		if (t->plane_num == c) {
			for (int i = 0; i <= 79; i++) {
				if (t->seat_occupy[i] == d) {
					t->seat_occupy[i] = "a";
				}
			}
			break;
		}
		else {
			t = t->next;
		}
	}
	cout << "3s后返回主菜单！" << endl;
	Sleep(3000);
	system("cls");

}

//管理员删除航班
void machine::del_plane() {
	string a;//航班号
	cout << "\n请输入您要删除的航班号：";
	cin >> a;
	plane* p = first_plane;
	plane* help = NULL;
	while (p) {
		if (p->plane_num == a) {
			for (int i = 0; i <= 79; i++) {
				//遍历该航班seat_occupy数组，如果有人购买，则禁止删除
				if (p->seat_occupy[i] == "a") {}
				else {
					SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
					cout << "\n该航班有人购买，禁止删除！";
					SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
					return;
				}
			}
			break;
		}
		else {
			help = p;
			p = p->next;
		}
	}
	if (!p) {
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n未找到该航班，请查证后再输入！" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		return;
	}

	//执行删除步骤
	if (!help) {
		first_plane = first_plane->next;
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n删除成功!" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		delete p;
	}
	else {
		help->next = p->next;
		SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		cout << "\n删除成功!" << endl;
		SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		delete p;
	}
	//删除结束
}

//将航班信息写入plane.txt
void machine::writePLtofile() {
	plane* t = first_plane;
	FILE* fw = fopen(WRITE_PL, "w+");
	if (fw) {
		while (t) {
			//航班号\t出发地\t目的地\n
			fprintf(fw, "%s\t%s\t%s\n", t->plane_num.c_str(), t->sous.c_str(), t->des.c_str());//要加.c_str,fprintf()不能直接处理string!
			t = t->next;
		}
		fclose(fw);
		cout << "航班写入完毕！" << endl;

	}
	else { cout << "打开错误！" << endl; }
}

//将乘客信息写入passenger.txt
void machine::writePAtofile() {
	passenger* t = first_passenger;
	FILE* fw = fopen(WRITE_PA, "w+");
	if (fw) {
		while (t) {
			//姓名\t身份证号\t航班号\t座位号\n
			fprintf(fw, "%s\t%s\t%s\t%s\n", t->name.c_str(), t->id.c_str(), t->flight.c_str(), t->seat.c_str());
			t = t->next;
		}
		fclose(fw);
		cout << "乘客写入完毕！" << endl;
	}
	else { cout << "打开错误" << endl; }
}

//主菜单
void mainmenu() {
	cout << "\n" << endl;
	SetConsoleColor(FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	cout << "\t\t\t********欢迎使用航班售票系统********" << endl;
	cout << "\t\t\t*                                  *\n";
	cout << "\t\t\t*        " << "1.查看座位信息            *" << endl;
	cout << "\t\t\t*        " << "2.购票                    *" << endl;
	cout << "\t\t\t*        " << "3.打印机票信息            *" << endl;
	cout << "\t\t\t*        " << "4.改签                    *" << endl;
	cout << "\t\t\t*        " << "5.退票                    *" << endl;
	cout << "\t\t\t*        " << "6.管理员                  *" << endl;
	cout << "\t\t\t*        " << "7.安全退出                *" << endl;
	cout << "\t\t\t*                                  *\n";
	cout << "\t\t\t************************************" << endl;
	SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	cout << "\n";
}

//管理员身份验证函数
void login() {
	SetConsoleColor(FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	cout << "\t\t\t********欢迎您，管理员********" << endl;
	SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	while (1) {
		string inuser;
		string inpass;
		cout << "\t\t\t\t请输入用户名：";
		cin >> inuser;
		cout << "\t\t\t\t请输入密码:";
		char ch = _getch();
		//输入密码功能
		//输入字符，屏幕显示*
		while (ch != '\n' && ch != '\r') {
			//退格键删除一个字符
			if (ch == '\b') {
				if (inpass.length() != 0) {
					cout << "\b \b";
					inpass.resize(inpass.length() - 1);
					ch = _getch();
				}
			}
			else {
				_putch('*');
				inpass += ch;
				ch = _getch();
			}
		}
		//调用administrator的check__id方法，验证身份；正确通过，错误重新输入
		if (adm1.check_id(inuser, inpass) == 1) {
			cout << '\n';
			return;
		}
		else {
			SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			cout << "\n用户名或密码错误，请再试一次" << endl;
			SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		}
	}
}
void administration() {
	system("cls");
	login();//调用login验证身份
	system("cls");
	//管理员操作菜单
	cout << "\n" << endl;
	SetConsoleColor(FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	cout << "\t\t\t*************欢迎您，管理员*************" << endl;
	cout << "\t\t\t*                                      *" << endl;
	cout << "\t\t\t*       请输入您想要进行的操作         *" << endl;
	cout << "\t\t\t*       1.添加航班                     *" << endl;
	cout << "\t\t\t*       2.打印所有乘客信息             *" << endl;
	cout << "\t\t\t*       3.取消航班                     *" << endl;
	cout << "\t\t\t*       4.返回                         *" << endl;
	cout << "\t\t\t*                                      *" << endl;
	cout << "\t\t\t****************************************" << endl;
	SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	int k;
	while (true) {
		cout << "\n请输入要操作的功能序号:" << endl;
		cin >> k;
		switch (k) {
		case 1:ma.addplane(); break;
		case 2:ma.print_all_passenger(); break;
		case 3:ma.del_plane(); break;
		case 4:system("cls"); return;
		default:cout << "输入无效，请按序号输入！" << endl;
		}
	}
}

//登录过渡界面，防止乘客误操作选择了管理员按钮。该界面提供返回功能
void log_or_back() {
	system("cls");
	cout << "\n\n" << endl;
	cout << "\t\t\t*\t1.登录\t\t*" << endl;
	cout << "\t\t\t*\t2.返回\t\t*" << endl;
	int k;
	while (1) {
		cin >> k;
		switch (k) {
		case 1:administration();
		case 2:system("cls"); return;
		default:cout << "无效输入，请按序号输入！" << endl;
		}
	}
}

//查询指定航班座位状态函数
void show_seat() {
	cout << "请输入要查询的航班" << endl;
	string a;
	cin >> a;
	ma.show_available_seat(a);//调用machine的show_available_seat方法
	SetConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	cout << "请按任意键返回系统！" << endl;
	SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	char in;
	cin >> in;
	if (1) {
		system("cls");
		return;
	}

}

int main() {
	//将控制台背景改为白色
	SetConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	//初始化航班信息之后再初始化乘客信息！注意：顺序不可相反
	ma.addplane_file();
	ma.addpassenger_file();
	int h;
	while (true) {
		mainmenu();
		cout << "请输入对应序号:";
		cin >> h;
		switch (h) {
		case 1:show_seat(); break;//查阅指定航班座位信息
		case 2:ma.addpassenger(); break;//乘客购买机票
		case 3:ma.print_passenger_info(); break;//乘客查询机票信息
		case 4:ma.change_plane(); break;//乘客改签航班
		case 5:ma.refund(); break;//乘客退票
		case 6:log_or_back(); break;//管理员
		case 7:ma.writePLtofile();//保存航班信息
			ma.writePAtofile();//保存乘客信息
			cout << "保存完毕，3s后返回系统。如需退出，可在返回系统后直接关闭该窗口！\n";
			Sleep(3000);
			system("cls");
			break;
		default:cout << "\n无效序号" << endl;
		}
	}
}

