#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node         //定义节点
{
    float x;                //系数
    int y;                  //指数
    struct node* prev;      //设置前驱
    struct node* next;      //指向下一个节点
}node;
typedef struct stack        //定义栈
{
    int size;               //栈的实时大小
    node* top;              //栈顶
    node* head;             //栈的头结点
}stack;
void init(stack *s)         //初始化栈函数
{
    s->head=(node*)malloc(sizeof(node)); //创建头结点
    s->size=0;                           //此时栈的大小为0
    s->top=s->head;                      //栈顶指向头结点
    s->top->next=NULL;                   
    s->top->prev=NULL;
}
void push(stack* s,float x,int y)      //将系数为x，指数为y的节点压入栈
{
    s->top->next=(node*)malloc(sizeof(node));   //创建一个节点并将其设为此时栈顶的下一个节点
    s->top->next->prev=s->top;                  //将新创建节点的前驱指向此时栈顶
    s->top=s->top->next;                        //将头结点指向新创建的节点
    s->top->x=x;
    s->top->y=y;                                //给新节点赋值
    s->top->next=NULL;                          
    s->size++;                                  //栈的大小增加1
}
int sort(stack* s)                      //栈排序函数
{
    node *i,*j;                         
    if(s->size==1||s->size==0)          //若栈空或者栈中只有一个元素，直接返回0，函数调用结束
        return 0;
    i=s->head;                          //令i等于头结点
    while(i!=s->top->prev)              //当i等于头结点前一个节点时，循环结束
    {
        i=i->next;                      
        j=i->next;
        while(j!=NULL)                  //该循环结束后，i指向节点元素的幂次最大
        {
            if(j->y>i->y)
            {
                float tempx=i->x,tempy=i->y;
                i->x=j->x;
                i->y=j->y;
                j->x=tempx;
                j->y=tempy;
            }
            j=j->next;
        }
    }
    return 1;
}
int pop(stack* s,node *m)           //出栈函数,并把弹出的节点的值赋给m节点
{
    if(s->head==s->top)             //若栈空，则退出
        return 0;
    m->x=s->top->x;                 
    m->y=s->top->y;
    s->top=s->top->prev;            //栈顶指向栈顶前一个节点
    free(s->top->next);             //清除弹出的节点
    s->size--;                      //栈的大小减小1
    return 1;
}
void calculate(stack *s1,stack *s2,stack* result,char c)        //计算多项式，并将结果升幂存入result，c决定是+还是-
{
    int t;
    node m,n;                                                  
    sort(s1);                                                   
    sort(s2);                                                   //先让s1,s2降幂排列
    while(s1->size&&s2->size)                                   //直到s1,s2至少有一个为空，循环结束
    {
        if(s1->top->y==s2->top->y)                              //若两多项式栈顶元素幂次相同
        {
            pop(s1,&m);                                         
            pop(s2,&n);                                         //分别让两个元素出栈
            if(c=='+')
                m.x+=n.x;
            else
                m.x-=n.x;                           
            if(result->top->y==m.y)                             //若该元素的幂次与已进入result且存在栈顶的元素幂次相同，则直接相加
                    result->top->x+=m.x;
            else
                push(result,m.x,m.y);                           //否则直接入栈
        }
        else
        {
            s1->top->y < s2->top->y ?(pop(s1,&m),t=1):(pop(s2,&m),t=0);//若s1栈顶元素幂次较小，则其出栈且使t=1；否则s2出栈且使t=0
            if(result->top->y==m.y)        //与已入result的栈顶元素幂次相同
            {    
                if(c=='-'&&t==0)
                    result->top->x-=m.x;
                else
                    result->top->x+=m.x;
            }
            else
                (c=='-'&&t==0)?push(result,-m.x,m.y):push(result,m.x,m.y);// 若c='-',且出栈的是s2的元素,则让系数-m.x入栈，否则还是m.x
        }
    }
    if(s1->size||s2->size)                                                  //若s1,s2还有一个栈非空
    {
        stack* k= s1->size ? s1 : s2;        //令k等于那个非空的栈
        while(k->size!=0)               //直到k栈大小为0
        {
            pop(k,&m);                  
            if(result->top->y==m.y)     //与已入result的栈顶元素幂次相同
            {    
                if(c=='+')
                    result->top->x+=m.x;
                else
                    result->top->x-=m.x;
            }
            else
                (c=='-'&&k==s2)?push(result,-m.x,m.y):push(result,m.x,m.y);   //若c='-'且k=s2,则让系数-m.x入栈，否则还是m.x
        }
    }
}
int main()
{
    stack d1,d2,d3;  //创建三个栈，分别用于储存两个多项式以及最终计算结果
    float a;        
    int b,t=0;      
    char c=0,h=0;
    stack *s1=&d1,*s2=&d2,*result=&d3;//创建3个指针分别指向三个栈
    init(result);
    init(s1);
    init(s2);             //初始化三个栈
    do//进行第一个多项式的读取，遇到加减号停止，或者遇到换行符或EOF则退出整个循环   
    {            
        char j[10];         
        scanf("%s",j);      //每次读入一个字符串到字符数组j，遇到空格停止
        if(strlen(j)==1&&(j[0]=='+'||j[0]=='-'))    //若读到+，-且数组只有一个元素，则转入第二个多项式的读取
        {
            h=j[0];             //用h储存+或者-
            t=0;                //表示读入字符串的次数为偶数还是奇数，0为奇数，1为偶数
            do      //遇到换行符或者EOF退出
            {
                scanf("%s",j);
                if(t==0)
                    a=atof(j);//读取奇数次，则将字符串转换为浮点数
                else
                    b=atoi(j);//读取偶数次，则将字符串转换为整型数，并且接下来使a,b入栈
                if(t==1)
                {
                    if(s2->top->y==b)   //若与已入栈的元素幂次相同
                        s2->top->x+=a;
                    else                //否则则直接入栈
                        push(s2,a,b);   
                }
                t=t^1;          //t与1异或用于表示下次读取次数是奇数还是偶数
            } while (c=getchar()!='\n'&&c!=EOF);
            break;//退出整个循环
        }
        if(t==0)                //同上
            a=atof(j);
        else
            b=atoi(j);
        if(t==1)
        {
            if(s1->top->y==b)
                s1->top->x+=a;
            else
                push(s1,a,b);
        }
        t=t^1;
    }while(c=getchar()!='\n'&&c!=EOF);
    calculate(s1,s2,result,h);//计算两多项式结果，并将其升幂储存到result内
    node* lk=result->head->next,m;  
    while(lk&&!(lk->x)) //lk已经遍历整个result或者result中有一个节点系数不为0则退出，否则继续指向下一个节点
        lk=lk->next;    
    if(!lk)     //result里元素全部系数为0
        printf("0 0");
    else//result中有一个元素系数不为0
    {
        while(result->size)
        {
            if(result->top->x!=0)
            printf("%g %d ",result->top->x,result->top->y);
            pop(result,&m);//将已输出的元素弹出栈
        }
    }
    system("pause");
    return 0;
}
