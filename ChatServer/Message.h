#pragma once

struct MSG_INFO
{
	int m_Type;//类型
	char m_From[20];//发信人名字
	char m_To[20];//收信人姓名
	COLORREF m_Color;//信息的颜色
	//BOOL m_Secret;//是否悄悄话
	//int m_Image;//用户头像
	char m_IP[20];//用户IP，上线的时候用到
	char m_Text[1000];
};

struct USER_INFO
{
	char m_User[20]; // 用户名
	char m_IP[20]; // 用户IP
	//int m_Image; // 用户头像
	SOCKADDR_IN m_SA; // 用户地址，作为sendto的参数
};

//type的的含义
/*
 0		隐身
 1		正常谈话
-1		上线
-2		下线
-3		重名
-4    系统关闭
*/
