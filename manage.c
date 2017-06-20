/*1��ͼ��������ҵ��ģ��
1) ��ĵǼ����ݰ�����š������������ߡ��ִ����Ϳ������
2) �������������Ա�����߲���Ч�ʣ�
a)	�ɱ���⣺�¹�һ���飬ȷ����ź󣬵Ǽǵ�ͼ����Ŀ���У�����������У���ֻ����������ӣ�
b)	���ģ����һ������ִ�������0������һ�����Ǽǽ����ߵ���֤�ź͹黹���ޣ��ı��ִ�����
c)	�黹��ע���Խ����ߵĵǼǣ��ı������ִ�����
3����Ҫ�������£�
�����ʽ���ܰ���š������������߲��ҿ����鼮��Ϣ
          �ܰ�ѧ���Ľ���֤����ʾѧ����Ϣ�ͽ�����Ϣ
          �鼮���
          ���鹦��ʵ��
          ���鹦��ʵ��
*/
#include"stdio.h"
#include"time.h"
#include"malloc.h"
#include"string.h"
#include"math.h"
//�����б�
typedef struct{
	int id;  //���
	char*name;  //����
	char*author;  //����
	int rest;  //�ִ���
	int store;  //���
}book;
//���
typedef struct{
	int m;  //��ǰ�鼮��Ŀ
	int M;  //�鼮�ܿ����
	book*element;
}HashBook;
typedef HashBook*pHashBook;
//�����б�
struct BookNode;
typedef struct BookNode* pNode;
struct BookNode{
	book info;  //���������Ϣ
	char* borrow_time;  //����ʱ��
	char* return_time;  // Ӧ��ʱ��
	pNode next;  //ָ����һ����
};
typedef struct BookNode* BookList;
//ѧ������б�
typedef struct{
	int id;  //ѧ��
	char*name;  //����
	BookList borrow;//������ĵ�һ�����
}student;
//ѧ���б�
typedef student*pstudent;
typedef struct{
	int m;//��ǰѧ����Ŀ
	int M;//������ѧ������
	student*element;
}HashStudent;
typedef HashStudent*pHashStudent;

//����ѧ���б�
pHashStudent createEmptyStudent(int M){
	int i;
	pHashStudent ps=(pHashStudent)malloc(sizeof(HashStudent));  //����ռ�
	if(ps!=NULL){  //ps��Ϊ�գ���ʼ��ps
	    ps->M=M;
        ps->m=0;
		ps->element=(student*)malloc(sizeof(student)*M);  //Ϊ������ռ�
		if(ps->element){  //����ɹ�����ʼ������Ԫ��
			for(i=0;i<M;i++){
				ps->element[i].id=0;
				ps->element[i].name=(char*)malloc(sizeof(char)*20);
				ps->element[i].borrow=NULL;
				return ps;
			}
		}
		else free(ps);  //���벻�ɹ����ͷ�ps
	}
	printf("Out of space!");  //���벻�ɹ��������ʾ�����ؿ�
	return NULL;
}

//�������б�
pHashBook createEmptyBook(int M){
	int i;
	pHashBook pb=(pHashBook)malloc(sizeof(HashBook));  //����ռ�
	if(pb!=NULL){  //����ռ䲻Ϊ�� ��ʼ��
		pb->element=(book*)malloc(sizeof(book)*M);  //Ϊ������ռ�
		pb->M=M;
		pb->m=0;
		if(pb->element){  //������ռ�ɹ���Ϊ���ʼ��
			for(i=0;i<M;i++){
				pb->element[i].id=0;
				pb->element[i].name=(char*)malloc(sizeof(char)*20);
				pb->element[i].author=(char*)malloc(sizeof(char)*20);
				pb->element[i].rest=0;
				pb->element[i].store=0;
				return pb;
			}
		}
		else free(pb);  //���벻�ɹ����ͷ�pb����Ŀռ�
	}
	printf("Out of space!");  //���벻�ɹ��������ʾ�������ؿ�
	return NULL;
}

//���ѧ��
void addStudent(pHashStudent ps){
    if(ps->m>=ps->M){
        printf("ѧ������!���ʧ��!\n");
        return;
    }
    int position;
     char*name;
     name=(char*)malloc(sizeof(char)*20);
     printf("�����������:\n");
     scanf("%s",name);
     int id;
     printf("�������ѧ��:\n");
     scanf("%d",&id);
	 if(linearSearch_student_id(ps,id,&position)==1){
		printf("ѧ���Ѵ��ڣ�\n");
		return;
	}
	else if(position!=-1){
        ps->element[position].id=id;
        ps->element[position].name=name;
        ps->m++;
        printf("��ӳɹ�!\n");
        return;
	}
	else{
        printf("ѧ������������\n");
		return;
	}
}

//�ж��Ƿ�������
int isprime(int n){
    int t,i;
    t=(int)sqrt(n);
    for(i=2;i<t;i++){//���Դ�2��ʼ�����������������һ������������������
        if(n%i==0)
            return 0;
    }
    return 1;
}
//ɢ�к���
int h(int id,int m){
    int i;
    for(i=m;;i++){//����������������������һֱ��һ��ֱ���ҵ����������С����������ѭ��
        if(isprime(i))
            break;
    }
    return id%i;
}
//����(�����)
int linearSearch_book_id(HashBook* pbhash,int id,int* position2)
{
	int d,inc;
	d = h(id,pbhash->M);  //dΪɢ�е�ַ,h(id)Ϊɢ�к���
	for( inc=0;inc<pbhash->M;inc++ )
	{
		if( pbhash->element[d].id==id )  //�����ɹ�
		{
			*position2 = d; //*positionΪ�ҵ���Ԫ����ɢ�б���element���±�ֵ
			return 1;
		}else if( pbhash->element[d].id==0 )  //����ʧ��
		{
			*position2 = d;  //���ɢ�б��л��пɲ���Ŀռ䣬*position�������ʵĲ���λ��
			return 0;
		}
		d = (d+1)%(pbhash->M);
	}
	*position2 = -1;  //����ʧ�ܣ�����û�к��ʵ�λ�ò���Ŀռ�
	return 0;
}

//������(������)
void linearSearch_book_name(HashBook* pbhash)
{
    if(pbhash->m==0){  //���Ϊ��
        printf("���Ϊ�գ�����ʧ�ܣ�\n");
        return;
    }
    char*name;
    name=(char*)malloc(sizeof(char)*20);
    printf("����Ҫ���ҵ��鼮��������\n");
    scanf("%s",name);
    getchar();
	int inc = 0;  //ѭ������
	int flag,f=0;
	for( inc=0;inc<pbhash->M;inc++ )  //���������������ѯ��
	{
	    if(pbhash->element[inc].id!=0){
		flag = strcmp(name,pbhash->element[inc].name);  //�Ƚ�����
            if( flag==0 )  //���������������Ϣ
            {
                printf("������%s\n",pbhash->element[inc].name);
                printf("��ţ�%d\n",pbhash->element[inc].id);
                printf("�����ߣ�%s\n",pbhash->element[inc].author);
                printf("�ִ�����%d\n",pbhash->element[inc].rest);
                printf("�������%d\n",pbhash->element[inc].store);
                f=1;
            }
        }
	}
	if(f==0){  //�Ҳ���
        printf("û�ҵ�\n");
        return;
	}
}

//��������Ϣ(�����)
void show_book(HashBook* pbhash)
{
    if(pbhash->m==0){  //���Ϊ��
        printf("���Ϊ�գ�\n");
        return;
    }
    int id;
	int position2;
	printf("���������\n");
	scanf("%d",&id);
	if( linearSearch_book_id(pbhash,id,&position2)==1 )  //�ҵ������������Ϣ
	{
			printf("������%s\n",pbhash->element[position2].name);
			printf("��ţ�%d\n",pbhash->element[position2].id);
			printf("�����ߣ�%s\n",pbhash->element[position2].author);
			printf("�ִ�����%d\n",pbhash->element[position2].rest);
			printf("�������%d\n",pbhash->element[position2].store);
			return ;
	}
	printf("�޴���");
}

//������(��������)
void linearSearch_book_author(HashBook* pbhash)
{
    if(pbhash->m==0){  //���Ϊ��
        printf("���գ�\n");
        return;
    }
	char* author;
	int inc = 0;
	int flag;
    int f=0;
	printf("������Ҫ��ѯ��������\n");
	author=(char*)malloc(sizeof(char)*20);
	scanf("%s",author);
	for( inc=0;inc<pbhash->M;inc++ )  //���������������ѯ��
	{
	    if(pbhash->element[inc].id!=0){
		flag = strcmp(author,pbhash->element[inc].author);  //�Ƚ�����
            if( flag==0 )  //���������������Ϣ
            {
                printf("������%s\n",pbhash->element[inc].name);
                printf("��ţ�%d\n",pbhash->element[inc].id);
                printf("�����ߣ�%s\n",pbhash->element[inc].author);
                printf("�ִ�����%d\n",pbhash->element[inc].rest);
                printf("�������%d\n",pbhash->element[inc].store);
                f=1;
            }
        }
	}
	if(f==0){
        printf("û�ҵ�\n");
        return;
	}
}

//����ѧ��(��ѧ��)
int linearSearch_student_id(HashStudent* pshash,int id,int* position)
{
	int d,inc;
	int a;
	d = h(id,(pshash->M));  //dΪɢ�е�ַ��ɢ�к���Ϊh
	for( inc=0;inc<pshash->M;inc++ )
	{
		if( pshash->element[d].id==id )
		{
			*position = d;  //�����ɹ���*positionΪ�ҵ���Ԫ����ɢ�б���element����Ԫ�ص��±�ֵ
			return 1;
		}
		else if( pshash->element[d].id==0 )
		{
			*position = d;  //����ʧ�ܣ�������л��пɲ���Ŀռ�*positionΪ���ʵĲ���λ��
            return 0;
		}
		d = (d+1)%(pshash->M);
	}
	*position = -1;  ////ɢ�б����
	return 0;
}



//��ӡѧ����Ϣ
void show_student(HashStudent* pshash,int position1)
{
	pNode p;
	int count = 1;
	p = pshash->element[position1].borrow;
	printf("ѧ����Ϣ��\n"); //���ѧ����Ϣ
	printf("������%s\n",pshash->element[position1].name);
	printf("ѧ�ţ�%d\n",pshash->element[position1].id);
	if( p!=NULL )
	{
		printf("����\t��������\t��������\n");
	}
	while( p!=NULL )  //���ѧ���Ľ������
	{
		printf("%s\t%s\t%s\n",p->info.name,p->borrow_time,p->return_time);
		count++;
		p=p->next;
	}
 }

 //����ɹ�
void borrowbook(HashBook* pbhash,HashStudent* pshash,int position1)
{
	int position2;
	char* d=":";
	char* f;
	//����ʱ��
	struct tm*t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);

        char A[20] = {0};//�꣺
        sprintf(A,"%d",(t->tm_year+1900));//��char*ת����char����
        strcat(A,":");

        char B[20]={0};//�£�
        sprintf(B,"%d",(t->tm_mon));
        strcat(B,":");

        strcat(A,B);//����        //���������ַ��������Ӻ���ַ��������a�У�����a�����㹻�ռ�
        char C[20] = {0};//�գ�
        sprintf(C,"%d",(t->tm_mday));

        strcat(A,C);//�����մ���A
        //printf("%s",A);   //������Ӻ���ַ���

        char E[20] = {0};//�꣺
        sprintf(E,"%d",(t->tm_year+1900));
        strcat(E,":");

        char F[20] = {0};//�գ�
        sprintf(F,"%d",(t->tm_mon+1));
        strcat(F,":");

        strcat(E,F);//����        //���������ַ��������Ӻ���ַ��������a�У�����a�����㹻�ռ�
        char G[20] = {0};//�գ�
        sprintf(G,"%d",(t->tm_mday));

        strcat(E,G);//�����մ���A
        //printf("%s",E);   //������Ӻ���ַ���
	//���ý�����Ϣ
	int id;
	printf("��������ĵ����\n");
	scanf("%d",&id);
	pNode p,q;  //��������Ľ��
	p = (pNode)malloc(sizeof(struct BookNode));  //�������Ϣ
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
			//��ʼ����Ϣ
			p->info.name = pbhash->element[position2].name;
			p->info.id = pbhash->element[position2].id;
			p->info.author = pbhash->element[position2].author;
			p->info.rest = pbhash->element[position2].rest;
			p->info.store = pbhash->element[position2].store;
			p->next = NULL;
			p->borrow_time = A;
			printf("����ʱ��:\n");
			printf("%s\n",p->borrow_time);
			p->return_time = E;
			printf("�����ֹʱ��:\n");
			printf("%s\n",p->return_time);
			//��λ��
			if(q==NULL){
                pshash->element[position1].borrow = p;
                pbhash->element[position2].rest--;
                printf("���ĳɹ���\n");
                return;
			}
			//��Ϊ��ʱ���ҵ����Ľ��
			while( q->next!=NULL )
			{
				q = q->next;
			}
			q->next=p;
			printf("���ĳɹ���");
			pbhash->element[position2].rest--;
			return;
		}else
		{  //�޿��
            printf("�޿�棡\n");
            return;
		}
	}else
	{  //�޴���
		printf("�޴���");
	}
}

//	�ɱ���⣺�¹�һ���飬ȷ����ź󣬵Ǽǵ�ͼ����Ŀ���У�����������У���ֻ����������ӣ�
void inStore(pHashBook pb){/**********/
    if(pb->m>=pb->M){
        printf("�������������ʧ��!\n");
        return;
    }
	int id;
	int position;
	printf("������Ҫ�����鼮��ţ�\n");
	scanf("%d",&id);
	getchar();
	if(linearSearch_book_id(pb,id,&position)==1){
		printf("���Ѵ��ڣ�\n");
		pb->element[position].store++;
		pb->element[position].rest++;
		printf("���ɹ���\n");
		return;
	}
	else if(position!=-1){
		pb->element[position].id=id;
		printf("����������\n");
		pb->element[position].name=(char*)malloc(sizeof(char)*20);
		scanf("%s",pb->element[position].name);
		getchar();
		printf("�������ߣ�\n");
		pb->element[position].author=(char*)malloc(sizeof(char)*20);
		scanf("%s",pb->element[position].author);
		getchar();
		pb->m++;
		pb->element[position].rest=1;
		pb->element[position].store=1;
		printf("���ɹ�!\n");
		return;
	}
}


//����
void returnbook(pHashBook pb,pHashStudent ps,int position1){
    int id;
    pNode p,q;
    p=(pNode)malloc(sizeof(struct BookNode));
    int position2;
    printf("����Ҫ���������ţ�");
    scanf("%d",&id);
    if(!(linearSearch_book_id(pb,id,&position2)==1)){//����в����ڴ���
        printf("������Ŵ���!����ʧ��!\n");
        return;
    }
    p=ps->element[position1].borrow;
    if(p==NULL){//ѧ��δ����
        printf("��ѧ��δ���飬����ʧ��!\n");
        return;
    }
    else{
        if((p->info.id)==id){//ѧ����ĵ�һ����Ϊ����
            ps->element[position1].borrow=p->next;
            free(p);
            pb->element[position2].rest++;
            printf("����ɹ�!\n");
            return;
        }
        while(p!=NULL&&(p->info.id!=id)){
            q=p;
            p=p->next;
        }
        if(p==NULL){//ѧ��������в����ڸ���
            printf("��ͬѧδ�����\n");
            return;
        }
        else{//�ҵ�Ҫ������
            q->next=p->next;
            free(p);
            pb->element[position2].rest++;
            printf("����ɹ�!\n");
            return;
        }
    }
}

//ѧ����¼
void login(pHashStudent ps,int*position){
	int id;
	int i;
	char*name;
	name=(char*)malloc(sizeof(char)*20);
	printf("��½ϵͳ\n");
	printf("����ѧ��:");
	scanf("%d",&id);
	printf("��������:");
	scanf("%s",name);
	if((linearSearch_student_id(ps,id,position))==1){
        i=strcmp(ps->element[*position].name,name);
        if(i==0){
            printf("��½�ɹ���\n");
            return;
        }
        else {
                printf("�û�������,��¼ʧ�ܣ�\n");
                ps->element[*position].id=0;
                return;
        }
	}  //���ڴ�ѧ��
    else
        printf("��¼ʧ�ܣ�\n");
}

//ѧ�����Ľ���
void show(){
    printf("\n");
    printf("1.����Ų����鼮��Ϣ\n");
    printf("2.�����������鼮��Ϣ\n");
    printf("3.�������߲����鼮��Ϣ\n");
    printf("4.��ѧ������֤����ʾѧ����Ϣ�ͽ�����Ϣ\n");
    printf("5.�鼮���\n");
    printf("6.����\n");
    printf("7.����\n");
    printf("8.�˳�\n");
    printf("\n");
}
int main(){
    pHashBook pb=(pHashBook)malloc(sizeof(HashBook));
    pHashStudent ps=(pHashStudent)malloc(sizeof(HashStudent));
    int m,n;
    int p=1,q=1;
    int position1;
    printf("**********************************************\n");
    printf("��ʼ�����棺\n");
    printf("\n");
    printf("�������������");
    scanf("%d",&m);
    printf("�������Ľ�������:");
    scanf("%d",&n);
    pb=createEmptyBook(m);
    ps=createEmptyStudent(n);
    while(q){
        if(pb->m>=pb->M){
            printf("���������\n");
            printf("\n");
            q=0;
            break;
        }
        printf("�Ǽ�ͼ����Ϣ��(����1,ֹͣ0)\n");
        scanf("%d",&q);
        if(q==1){
            inStore(pb);
            printf("\n");
        }
    }
    while(p){
        if(ps->m>=ps->M){
            printf("ѧ������������\n");
            printf("\n");
            q=0;
            break;
        }
        printf("�Ǽǽ�������Ϣ��(����1,ֹͣ0)\n");
        scanf("%d",&p);
        if(p==1){
            addStudent(ps);
             printf("\n");
        }
    }
    printf("��ʼ���ɹ�\n");
    printf("**********************************************\n");
    int choice;
	int tag=1;
	login(ps,&position1);
    printf("**********************************************\n");
    if(ps->element[position1].id!=0){
        printf("ͼ�����ϵͳ\n\n");
        while(tag){
            show();
            printf("�������ѡ��:");
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
            default:printf("�������\n");break;
            }
        }
    }
	printf("ллʹ�ã�");
	return 0;
}

