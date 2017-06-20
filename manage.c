/*1、图书管理基本业务模拟
1) 书的登记内容包括书号、书名、著作者、现存量和库存量；
2) 建立索引表（线性表）以提高查找效率；
a)	采编入库：新购一种书，确定书号后，登记到图书帐目表中，如果表中已有，则只将库存量增加；
b)	借阅：如果一种书的现存量大于0，则借出一本，登记借阅者的书证号和归还期限，改变现存量；
c)	归还：注销对借阅者的登记，改变该书的现存量。
3）主要功能如下：
输出形式：能按书号、书名、著作者查找库存的书籍信息
          能按学生的借书证号显示学生信息和借阅信息
          书籍入库
          借书功能实现
          还书功能实现
*/
#include"stdio.h"
#include"time.h"
#include"malloc.h"
#include"string.h"
#include"math.h"
//书结点列表
typedef struct{
	int id;  //书号
	char*name;  //书名
	char*author;  //作者
	int rest;  //现存量
	int store;  //库存
}book;
//书表
typedef struct{
	int m;  //当前书籍数目
	int M;  //书籍总库存量
	book*element;
}HashBook;
typedef HashBook*pHashBook;
//借书列表
struct BookNode;
typedef struct BookNode* pNode;
struct BookNode{
	book info;  //借阅书的信息
	char* borrow_time;  //借阅时间
	char* return_time;  // 应还时间
	pNode next;  //指向下一本书
};
typedef struct BookNode* BookList;
//学生结点列表
typedef struct{
	int id;  //学号
	char*name;  //姓名
	BookList borrow;//所借书的第一个结点
}student;
//学生列表
typedef student*pstudent;
typedef struct{
	int m;//当前学生数目
	int M;//最大借阅学生数量
	student*element;
}HashStudent;
typedef HashStudent*pHashStudent;

//创建学生列表
pHashStudent createEmptyStudent(int M){
	int i;
	pHashStudent ps=(pHashStudent)malloc(sizeof(HashStudent));  //申请空间
	if(ps!=NULL){  //ps不为空，初始化ps
	    ps->M=M;
        ps->m=0;
		ps->element=(student*)malloc(sizeof(student)*M);  //为表申请空间
		if(ps->element){  //申请成功，初始化表内元素
			for(i=0;i<M;i++){
				ps->element[i].id=0;
				ps->element[i].name=(char*)malloc(sizeof(char)*20);
				ps->element[i].borrow=NULL;
				return ps;
			}
		}
		else free(ps);  //申请不成功，释放ps
	}
	printf("Out of space!");  //申请不成功，输出提示并返回空
	return NULL;
}

//创建书列表
pHashBook createEmptyBook(int M){
	int i;
	pHashBook pb=(pHashBook)malloc(sizeof(HashBook));  //申请空间
	if(pb!=NULL){  //申请空间不为空 初始化
		pb->element=(book*)malloc(sizeof(book)*M);  //为表申请空间
		pb->M=M;
		pb->m=0;
		if(pb->element){  //表申请空间成功，为表初始化
			for(i=0;i<M;i++){
				pb->element[i].id=0;
				pb->element[i].name=(char*)malloc(sizeof(char)*20);
				pb->element[i].author=(char*)malloc(sizeof(char)*20);
				pb->element[i].rest=0;
				pb->element[i].store=0;
				return pb;
			}
		}
		else free(pb);  //申请不成功，释放pb申请的空间
	}
	printf("Out of space!");  //申请不成功，输出提示，并返回空
	return NULL;
}

//添加学生
void addStudent(pHashStudent ps){
    if(ps->m>=ps->M){
        printf("学生已满!添加失败!\n");
        return;
    }
    int position;
     char*name;
     name=(char*)malloc(sizeof(char)*20);
     printf("输入你的姓名:\n");
     scanf("%s",name);
     int id;
     printf("输入你的学号:\n");
     scanf("%d",&id);
	 if(linearSearch_student_id(ps,id,&position)==1){
		printf("学生已存在！\n");
		return;
	}
	else if(position!=-1){
        ps->element[position].id=id;
        ps->element[position].name=name;
        ps->m++;
        printf("添加成功!\n");
        return;
	}
	else{
        printf("学生名额已满！\n");
		return;
	}
}

//判断是否是质数
int isprime(int n){
    int t,i;
    t=(int)sqrt(n);
    for(i=2;i<t;i++){//除以从2开始依次增大的正整数，一旦能整除，则不是素数
        if(n%i==0)
            return 0;
    }
    return 1;
}
//散列函数
int h(int id,int m){
    int i;
    for(i=m;;i++){//如果传入的数不是素数，则一直加一，直到找到比它大的最小素数则跳出循环
        if(isprime(i))
            break;
    }
    return id%i;
}
//查找(按书号)
int linearSearch_book_id(HashBook* pbhash,int id,int* position2)
{
	int d,inc;
	d = h(id,pbhash->M);  //d为散列地址,h(id)为散列函数
	for( inc=0;inc<pbhash->M;inc++ )
	{
		if( pbhash->element[d].id==id )  //检索成功
		{
			*position2 = d; //*position为找到的元素在散列表中element的下标值
			return 1;
		}else if( pbhash->element[d].id==0 )  //检索失败
		{
			*position2 = d;  //如果散列表中还有可插入的空间，*position给出合适的插入位置
			return 0;
		}
		d = (d+1)%(pbhash->M);
	}
	*position2 = -1;  //检索失败，并且没有合适的位置插入的空间
	return 0;
}

//查找书(按书名)
void linearSearch_book_name(HashBook* pbhash)
{
    if(pbhash->m==0){  //库存为空
        printf("书库为空！查找失败！\n");
        return;
    }
    char*name;
    name=(char*)malloc(sizeof(char)*20);
    printf("输入要查找的书籍的书名：\n");
    scanf("%s",name);
    getchar();
	int inc = 0;  //循环变量
	int flag,f=0;
	for( inc=0;inc<pbhash->M;inc++ )  //遍历书表，按书名查询书
	{
	    if(pbhash->element[inc].id!=0){
		flag = strcmp(name,pbhash->element[inc].name);  //比较书名
            if( flag==0 )  //书名相等输出书的信息
            {
                printf("书名：%s\n",pbhash->element[inc].name);
                printf("书号：%d\n",pbhash->element[inc].id);
                printf("著作者：%s\n",pbhash->element[inc].author);
                printf("现存量：%d\n",pbhash->element[inc].rest);
                printf("库存量：%d\n",pbhash->element[inc].store);
                f=1;
            }
        }
	}
	if(f==0){  //找不到
        printf("没找到\n");
        return;
	}
}

//查找书信息(按书号)
void show_book(HashBook* pbhash)
{
    if(pbhash->m==0){  //书库为空
        printf("书库为空！\n");
        return;
    }
    int id;
	int position2;
	printf("请输入书号\n");
	scanf("%d",&id);
	if( linearSearch_book_id(pbhash,id,&position2)==1 )  //找到书后输出书的信息
	{
			printf("书名：%s\n",pbhash->element[position2].name);
			printf("书号：%d\n",pbhash->element[position2].id);
			printf("著作者：%s\n",pbhash->element[position2].author);
			printf("现存量：%d\n",pbhash->element[position2].rest);
			printf("库存量：%d\n",pbhash->element[position2].store);
			return ;
	}
	printf("无此书");
}

//查找书(按著作者)
void linearSearch_book_author(HashBook* pbhash)
{
    if(pbhash->m==0){  //书库为空
        printf("书库空！\n");
        return;
    }
	char* author;
	int inc = 0;
	int flag;
    int f=0;
	printf("请输入要查询的作者名\n");
	author=(char*)malloc(sizeof(char)*20);
	scanf("%s",author);
	for( inc=0;inc<pbhash->M;inc++ )  //遍历书表，按书名查询书
	{
	    if(pbhash->element[inc].id!=0){
		flag = strcmp(author,pbhash->element[inc].author);  //比较书名
            if( flag==0 )  //书名相等输出书的信息
            {
                printf("书名：%s\n",pbhash->element[inc].name);
                printf("书号：%d\n",pbhash->element[inc].id);
                printf("著作者：%s\n",pbhash->element[inc].author);
                printf("现存量：%d\n",pbhash->element[inc].rest);
                printf("库存量：%d\n",pbhash->element[inc].store);
                f=1;
            }
        }
	}
	if(f==0){
        printf("没找到\n");
        return;
	}
}

//查找学生(按学号)
int linearSearch_student_id(HashStudent* pshash,int id,int* position)
{
	int d,inc;
	int a;
	d = h(id,(pshash->M));  //d为散列地址，散列函数为h
	for( inc=0;inc<pshash->M;inc++ )
	{
		if( pshash->element[d].id==id )
		{
			*position = d;  //检索成功。*position为找到的元素在散列表中element数组元素的下标值
			return 1;
		}
		else if( pshash->element[d].id==0 )
		{
			*position = d;  //检索失败，如果表中还有可插入的空间*position为合适的插入位置
            return 0;
		}
		d = (d+1)%(pshash->M);
	}
	*position = -1;  ////散列表溢出
	return 0;
}



//打印学生信息
void show_student(HashStudent* pshash,int position1)
{
	pNode p;
	int count = 1;
	p = pshash->element[position1].borrow;
	printf("学生信息：\n"); //输出学生信息
	printf("姓名：%s\n",pshash->element[position1].name);
	printf("学号：%d\n",pshash->element[position1].id);
	if( p!=NULL )
	{
		printf("书名\t借书日期\t还书日期\n");
	}
	while( p!=NULL )  //输出学生的借书情况
	{
		printf("%s\t%s\t%s\n",p->info.name,p->borrow_time,p->return_time);
		count++;
		p=p->next;
	}
 }

 //借书成功
void borrowbook(HashBook* pbhash,HashStudent* pshash,int position1)
{
	int position2;
	char* d=":";
	char* f;
	//设置时间
	struct tm*t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);

        char A[20] = {0};//年：
        sprintf(A,"%d",(t->tm_year+1900));//将char*转化成char数组
        strcat(A,":");

        char B[20]={0};//月：
        sprintf(B,"%d",(t->tm_mon));
        strcat(B,":");

        strcat(A,B);//年月        //连接两个字符串，连接后的字符串存放在a中，数组a中有足够空间
        char C[20] = {0};//日：
        sprintf(C,"%d",(t->tm_mday));

        strcat(A,C);//年月日存入A
        //printf("%s",A);   //输出连接后的字符串

        char E[20] = {0};//年：
        sprintf(E,"%d",(t->tm_year+1900));
        strcat(E,":");

        char F[20] = {0};//日：
        sprintf(F,"%d",(t->tm_mon+1));
        strcat(F,":");

        strcat(E,F);//年月        //连接两个字符串，连接后的字符串存放在a中，数组a中有足够空间
        char G[20] = {0};//日：
        sprintf(G,"%d",(t->tm_mday));

        strcat(E,G);//年月日存入A
        //printf("%s",E);   //输出连接后的字符串
	//设置借阅信息
	int id;
	printf("请输入借阅的书号\n");
	scanf("%d",&id);
	pNode p,q;  //借阅新书的结点
	p = (pNode)malloc(sizeof(struct BookNode));  //存放新信息
	if( p==NULL )
	{
		printf("Out of space!");
		return;
	}
	if( linearSearch_book_id(pbhash,id,&position2)==1 )
	{
		if( pbhash->element[position2].rest>0 )
		{
			q = pshash->element[position1].borrow;
			//初始化信息
			p->info.name = pbhash->element[position2].name;
			p->info.id = pbhash->element[position2].id;
			p->info.author = pbhash->element[position2].author;
			p->info.rest = pbhash->element[position2].rest;
			p->info.store = pbhash->element[position2].store;
			p->next = NULL;
			p->borrow_time = A;
			printf("借书时间:\n");
			printf("%s\n",p->borrow_time);
			p->return_time = E;
			printf("还书截止时间:\n");
			printf("%s\n",p->return_time);
			//表位空
			if(q==NULL){
                pshash->element[position1].borrow = p;
                pbhash->element[position2].rest--;
                printf("借阅成功！\n");
                return;
			}
			//表不为空时，找到最后的结点
			while( q->next!=NULL )
			{
				q = q->next;
			}
			q->next=p;
			printf("借阅成功！");
			pbhash->element[position2].rest--;
			return;
		}else
		{  //无库存
            printf("无库存！\n");
            return;
		}
	}else
	{  //无此书
		printf("无此书");
	}
}

//	采编入库：新购一种书，确定书号后，登记到图书帐目表中，如果表中已有，则只将库存量增加；
void inStore(pHashBook pb){/**********/
    if(pb->m>=pb->M){
        printf("书库已满！加书失败!\n");
        return;
    }
	int id;
	int position;
	printf("请输入要入库的书籍书号：\n");
	scanf("%d",&id);
	getchar();
	if(linearSearch_book_id(pb,id,&position)==1){
		printf("书已存在！\n");
		pb->element[position].store++;
		pb->element[position].rest++;
		printf("入库成功！\n");
		return;
	}
	else if(position!=-1){
		pb->element[position].id=id;
		printf("输入书名：\n");
		pb->element[position].name=(char*)malloc(sizeof(char)*20);
		scanf("%s",pb->element[position].name);
		getchar();
		printf("输入作者：\n");
		pb->element[position].author=(char*)malloc(sizeof(char)*20);
		scanf("%s",pb->element[position].author);
		getchar();
		pb->m++;
		pb->element[position].rest=1;
		pb->element[position].store=1;
		printf("入库成功!\n");
		return;
	}
}


//还书
void returnbook(pHashBook pb,pHashStudent ps,int position1){
    int id;
    pNode p,q;
    p=(pNode)malloc(sizeof(struct BookNode));
    int position2;
    printf("输入要还的书的书号：");
    scanf("%d",&id);
    if(!(linearSearch_book_id(pb,id,&position2)==1)){//书库中不存在此书
        printf("输入书号错误!还书失败!\n");
        return;
    }
    p=ps->element[position1].borrow;
    if(p==NULL){//学生未借书
        printf("该学生未借书，还书失败!\n");
        return;
    }
    else{
        if((p->info.id)==id){//学生借的第一本书为此书
            ps->element[position1].borrow=p->next;
            free(p);
            pb->element[position2].rest++;
            printf("还书成功!\n");
            return;
        }
        while(p!=NULL&&(p->info.id!=id)){
            q=p;
            p=p->next;
        }
        if(p==NULL){//学生借的书中不存在该书
            printf("该同学未借此书\n");
            return;
        }
        else{//找到要还的书
            q->next=p->next;
            free(p);
            pb->element[position2].rest++;
            printf("还书成功!\n");
            return;
        }
    }
}

//学生登录
void login(pHashStudent ps,int*position){
	int id;
	int i;
	char*name;
	name=(char*)malloc(sizeof(char)*20);
	printf("登陆系统\n");
	printf("输入学号:");
	scanf("%d",&id);
	printf("输入名字:");
	scanf("%s",name);
	if((linearSearch_student_id(ps,id,position))==1){
        i=strcmp(ps->element[*position].name,name);
        if(i==0){
            printf("登陆成功！\n");
            return;
        }
        else {
                printf("用户名错误,登录失败！\n");
                ps->element[*position].id=0;
                return;
        }
	}  //存在此学生
    else
        printf("登录失败！\n");
}

//学生借阅界面
void show(){
    printf("\n");
    printf("1.按书号查找书籍信息\n");
    printf("2.按书名查找书籍信息\n");
    printf("3.按著作者查找书籍信息\n");
    printf("4.按学生借书证号显示学生信息和借阅信息\n");
    printf("5.书籍入库\n");
    printf("6.借书\n");
    printf("7.还书\n");
    printf("8.退出\n");
    printf("\n");
}
int main(){
    pHashBook pb=(pHashBook)malloc(sizeof(HashBook));
    pHashStudent ps=(pHashStudent)malloc(sizeof(HashStudent));
    int m,n;
    int p=1,q=1;
    int position1;
    printf("**********************************************\n");
    printf("初始化界面：\n");
    printf("\n");
    printf("输入最大库存量：");
    scanf("%d",&m);
    printf("输入最大的借阅人数:");
    scanf("%d",&n);
    pb=createEmptyBook(m);
    ps=createEmptyStudent(n);
    while(q){
        if(pb->m>=pb->M){
            printf("书库已满！\n");
            printf("\n");
            q=0;
            break;
        }
        printf("登记图书信息：(增加1,停止0)\n");
        scanf("%d",&q);
        if(q==1){
            inStore(pb);
            printf("\n");
        }
    }
    while(p){
        if(ps->m>=ps->M){
            printf("学生名额已满！\n");
            printf("\n");
            q=0;
            break;
        }
        printf("登记借阅人信息：(增加1,停止0)\n");
        scanf("%d",&p);
        if(p==1){
            addStudent(ps);
             printf("\n");
        }
    }
    printf("初始化成功\n");
    printf("**********************************************\n");
    int choice;
	int tag=1;
	login(ps,&position1);
    printf("**********************************************\n");
    if(ps->element[position1].id!=0){
        printf("图书管理系统\n\n");
        while(tag){
            show();
            printf("输入你的选择:");
            scanf("%d",&choice);
            switch(choice){
            case 1:show_book(pb);break;
            case 2:linearSearch_book_name(pb);break;
            case 3:linearSearch_book_author(pb);break;
            case 4:show_student(ps,position1);break;
            case 5:inStore(pb);break;
            case 6:borrowbook(pb,ps,position1);break;
            case 7:returnbook(pb,ps,position1);break;
            case 8:tag=0;break;
            default:printf("输入错误！\n");break;
            }
        }
    }
	printf("谢谢使用！");
	return 0;
}

