#pragma once

struct MSG_INFO
{
	int m_Type;//����
	char m_From[20];//����������
	char m_To[20];//����������
	COLORREF m_Color;//��Ϣ����ɫ
	char m_IP[20];//�û�IP�����ߵ�ʱ���õ�
	char m_Text[1000];
};

struct USER_INFO
{
	char m_User[20]; // �û���
	char m_IP[20]; // �û�IP
	SOCKADDR_IN m_SA; // �û���ַ����Ϊsendto�Ĳ���
};

//type�ĵĺ���
/*
 0		����
 1		����̸��
-1		����
-2		����
-3		����
-4    ϵͳ�ر�
*/
