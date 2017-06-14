#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define FILENAME "maze5.txt"		//�޸�Ϊmaze.txt����������ʱ�����Թ����޸�Ϊ�Ѵ����ļ���������Ѵ����Թ�
#define INFINITY 50000
FILE *fp;					//ȫ���ļ�ָ��
typedef enum{ PATH, WALL,RESULT } ElemTag;  //PATH==0:���ϰ�,WALL==1:���ϰ�,RESULT==2:�Թ��Ľ��·��
typedef struct MazeNode
{
	ElemTag tag;		//�������
	int location;		//��freeman�����е�λ��
}MazeNode;
typedef struct Freeman
{
	int code;		//������
	MazeNode* p;	//������������ĵ�ַ
}Freeman;
typedef struct
{
	MazeNode* mp;		//�Թ�����
	int m;				//�Թ�������
	int n;				//�Թ�������
	int x1, y1;			//�Թ��������
	int x2, y2;			//�Թ���������
	Freeman* fp;		//�Թ��Ľ�
}Maze;
void Create()
{
	//�����Թ��ļ�
	int m, n, x1, y1, x2, y2, i, j, tag;
	system("cls");
	fp = fopen(FILENAME, "w");			//����һ���ı��ļ�
	if (!fp)
	{
		printf("�½��ļ�ʧ��!\n");
		exit(0);
	}
	printf("�����Թ�������������(���߽�):");
	scanf("%d %d", &m, &n);	
	printf("�����Թ����������:");
	while (1)
	{
		scanf("%d %d", &x1, &y1);
		if (x1 > 0 && x1 < m && y1 > 0 && y1 < n)
			break;
		else
			printf("������������������������!\n");
	}
	printf("�����Թ��ĳ�������:");
	while (1)
	{
		scanf("%d %d", &x2, &y2);
		if (x2 > 0 && x2 <m && y2 > 0 && y2 < n )
			break;
		else
			printf("�������������������������!\n");
	}
	//д�ļ�
	fprintf(fp, "%d %d\n", m, n);
	fprintf(fp, "%d %d\n", x1, y1);
	fprintf(fp, "%d %d\n", x2, y2);
	printf("���벻���߽��Թ���ʾ��ͼ(0:���ϰ���1:���ϰ�)\n");
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (i == 0 || i == m - 1 || j == 0 || j == n - 1)
				fprintf(fp, "1 ");
			else
			{
				scanf("%d", &tag);
				if (tag == 0 || tag == 1)
				{
					if (i == x1 && j == y1 || i == x2&& j == y2)
						tag = 0;
					fprintf(fp, "%d ", tag);
				}
				else
				{
					printf("��������!\n");
					j--;
				}
			}
		}
		fprintf(fp, "\n");
	}
	//д�ļ�
	fclose(fp);
	fp = fopen(FILENAME, "r");
}//Create
void Open()
{
	//����Թ��ļ����ڣ���pfָ���Թ��ļ������򴴽��Թ��ļ�
	fp = fopen(FILENAME, "r");
	if (!fp)
	{
		printf("�Թ��ļ�������,�����Թ��ļ�!\n");
		system("pause");
		Create();
	}
}//Open

void InitMaze(Maze &M)
{
//�����Թ��ļ�������Ӧ���Թ�����M
	int i, j,t;
	Open();
	fscanf(fp, "%d %d", &M.m, &M.n);
	M.mp = (MazeNode*)malloc(sizeof(MazeNode) * M.m * M.n);
	M.fp = (Freeman*)malloc(sizeof(Freeman) * M.m * M.n * 2);
	if (!M.mp || !M.fp)
	{
		printf("�ڴ����ʧ��!");
		exit(0);
	}
	fscanf(fp, "%d %d", &M.x1, &M.y1);
	fscanf(fp, "%d %d", &M.x2, &M.y2);

	for (i = 0; i<M.m; i++)
		for (j = 0; j<M.n; j++)
		{
			fscanf(fp, "%d", &t);
			switch (t)
			{
			case 0:(M.mp + i * M.n + j)->tag = PATH;break;
			case 1:(M.mp + i * M.n + j)->tag = WALL; break;

			}
			(M.mp + i * M.n + j)->location = INFINITY;	//freeman����λ����ΪINFINITY
		}
	fclose(fp);
}//InitMaze

void DestroyMaze(Maze &M)
{
//�����Թ�����M
	free(M.mp);
	free(M.fp);
	M.m = M.n = 0;
	M.x1 = M.x2 = M.y1 = M.y2 = -1;
}//DestroyMaze

void SolveMaze(Maze &M)
{
	//����Թ����������freeman������ʽ����
	int x, y, d, index, t1, t2;
	x = M.x1; y = M.y1;				//�������
	d = 0;					//ȷ��̽��λ��
	index = 2;						//freeman�����±�
	while (x != M.x2 || y != M.y2 )
	{
		t1 = x;
		t2 = y;
		switch (d)
		{					//̽������仯
		case 0:t1 += 1; break;
		case 1:t1 += 1; t2 += 1; break;
		case 2:t2 += 1; break;
		case 3:t1 -= 1; t2 += 1; break;
		case 4:t1 -= 1; break;
		case 5:t1 -= 1; t2 -= 1; break;
		case 6:t2 -= 1; break;
		case 7:t1 += 1; t2 -= 1; break;
		}
		if ((M.mp + t1 * M.n + t2)->tag == PATH)
		{
			
			if ((M.mp + t1 * M.n + t2)->location == INFINITY)		//�жϸ�λ���Ƿ���ʹ�
				(M.mp + t1 * M.n + t2)->location = index;			//��¼freeman���������±�
			M.fp[index].code = d;	//����freeman��������
			M.fp[index].p = M.mp + t1 * M.n + t2;
			index++;
			d = (d + 6) % 8;		//̽��ɹ���һ��̽��λ��Ϊ������ת90��
			x = t1;
			y = t2;
		}
		else
			d = (d + 1) % 8;			//̽��ʧ����һ��̽��λ��Ϊ������ת45��
	}
	M.fp[0].code = index - 1;
}//SolveMaze

void OptimizeFreeman(Maze &M)
{
//�Ż�·��
	int t, d, min,dmin,flag;
	MazeNode *p , *pt, *pmin=NULL;
	p = M.mp + M.x2 * M.n + M.y2;	//ָ���յ�
	t = M.fp[0].code;	//ָ��freeman�������һλ
	//���Թ��յ����
	while (t > 0)
	{
		flag = 0;
		if (M.fp[t].code != INFINITY)
		{
			min = INFINITY;
			for (d = 0; d < 8; d++)		//�ҵ����λ���ڽӵ�������freeman������ֵ�λ��
			{
				pt = p;
				switch (d)				//�˸�����̽��
				{
				case 0:pt = pt + M.n; break;
				case 1:pt = pt + M.n + 1; break;
				case 2:pt = pt + 1; break;
				case 3:pt = pt - M.n + 1; break;
				case 4:pt = pt - M.n; break;
				case 5:pt = pt - M.n - 1; break;
				case 6:pt = pt - 1; break;
				case 7:pt = pt + M.n - 1; break;
				}
				if (pt->location < min)		
				{
					min = pt->location;		//��¼������freeman������ֵ�λ��
					pmin = pt;				//��¼�õ����Թ������λ��
					dmin = d;				//��¼������
				}
			}
			if (min != t - 1)			//����������λ�ò�����freeman����ǰһ��λ�ã�˵���м��в�·
			{
				for (d = t; d > min + 1; d--)		//�޳���·
					M.fp[d].code = INFINITY;
				if (d != t)		
				{
					M.fp[min + 1].code = (dmin + 4) % 8;	//�޸ĸ�λ�÷�����
					t = min;		//freeman��min��λ�ÿ�ʼ������ǰ
					p = pmin;		//�޸���һ��λ��
					flag = 1;		//��ʾ�в�·��t��ֵ�ı�
				}
			}			
		}
		if (!flag)
		{
			t--;
			p = pmin;
		}
	}
}//OptimizeFreeman
void DisplayResult_T(Maze M)
{
//����freeman���룬��������ʽ����Թ��������
	int x, y, t, d,flag = 0,count = 1;
	x = M.x1;
	y = M.y1;
	printf("�Թ��Ľ�:\n");
	(M.mp + x * M.n + y)->tag = RESULT;
	for (t = 1; t <= M.fp[0].code; t++)
	{
		flag = 0;
		d = M.fp[t].code;
		switch (d)			//����任
		{
		case 0:x += 1; break;
		case 1:x += 1; y += 1; break;
		case 2:y += 1; break;
		case 3:x -= 1; y += 1; break;
		case 4:x -= 1; break;
		case 5:x -= 1; y -= 1; break;
		case 6:y -= 1; break;
		case 7:x += 1; y -= 1; break;
		case INFINITY:flag = 1; break;
		}
		if (flag == 0)
		{
			printf("(%d,%d)", x, y);
			(M.mp + x * M.n + y)->tag = RESULT;
			count++;
			if (count % 8 == 0)
				printf("\n");
		}
		
	}
}//DisplayResult
void DisplayResult_G(Maze M)
{
	//�����Թ�����Ԫ�ص�tagλ����ͼ����ʽ����Թ��������
	int m, n;
	printf("\n");
	for (m = 0; m < M.m; m++)
	{
		for (n = 0; n < M.n; n++)
		{
			switch ((M.mp + m * M.n + n)->tag)
			{
			case PATH:	printf("  "); break;
			case WALL:	printf("��"); break;
			case RESULT: printf(" o"); break;
			}
		}
		printf("\n");
	}
}//DisplayResult
int main()
{
	Maze M;
	InitMaze(M);
	SolveMaze(M);
	OptimizeFreeman(M);
	DisplayResult_T(M);
	DisplayResult_G(M);
	DestroyMaze(M);
	system("pause");
	return 0;
}
