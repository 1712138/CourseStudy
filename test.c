#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
union V
{
    struct X
    {
        unsigned char s1 : 2;
        unsigned char s2 : 3;
        unsigned char s3 : 3;
    } x;

    unsigned char c;
};

/*
定义了一个联合，也就是 x 和 c 占用同一个空间，这里面 x 采用了位域表示方法，也就是 s1 只占 2 位，s2 占 3 位，s3 占 3 位，总共 8 位。
而这个空间又可以解释为一个字符c，因此对 v.c=100 来说，下一句是把这个 100 占用的空间解释成结构 x，
而 100 换成二进制后为 011 001 00，从后往前分别截取 2，3，3 位分别赋给 s1，s2，s3 因此 s3 获得最高的 011，也就是 3，输出结果就是 3
如果 c 的值设置为 200，也就是 110 010 00
                            s3  s2  s1
                        那么 s3 的值就是 110 也就是 6
*/

int main3()
{
    union V v;
    v.c = 100;
    printf("%d", v.x.s3);
    getchar();
    return 0;
}
