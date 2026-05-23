/* dualview.c -- 位字段和按位运算符 */
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
/* 位字段符号常量 */
/************************************************************/
/*定义  常量            值(0x为十六进制)      二进制值
/************************************************************/
/* 边框线样式   由下可见，二位就可以表示边框线样式，从 00 到 11，所以在 box_props 中 border_style 定义为 2 位 */
#define SOLID           0               //  00  实线
#define DOTTED          1               //  01  点线
#define DASHED          2               //  11  虚线

/* 三原色       由下可见，三位就可以表示八种颜色，从 000 到 111，所以在 box_props 中 fill_color 和 border_color 定义为 3 位  */
#define BLUE            4               //  100                     = 4
#define GREEN           2               //  010                     = 2
#define RED             1               //  001                     = 1
/* 混合颜色     所以在 colors 数组中，上面和下面的十进制值就正好是索引，通过 colors[index] 索引就可以得到颜色名 */
#define BLACK           0               //  000 
#define YELLOW  (RED | GREEN)           //  001 | 010 = 011         = 3
#define MAGENTA (RED | BLUE)            //  001 | 100 = 101         = 5
#define CYAN    (GREEN | BLUE)          //  010 | 100 = 110         = 6
#define WHITE   (RED | GREEN | BLUE)    //  001 | 010 | 100 = 111   = 7 

/* 按位方法中用到的符号常量 */
#define OPAQUE          0x1             //  0001                    从右到左，在第1位
#define FILL_BLUE       0x8             //  1000
#define FILL_GREEN      0x4             //  0100
#define FILL_RED        0x2             //  0010
#define FILL_CYAN       0xC             //  1100
#define FILL_MASK       0xE             //  1110                    从右到左，在第2到4位，在后面的代码注释中，重点理解掩码的作用：任何数
//                                                                  ，跟掩码取反后与运算，该数掩码位都变成0，也就是清0的作用
#define BORDER          0x100           //  0000 0001 0000 0000     从右到左，在第9位
#define BORDER_BLUE     0x800           //  0000 1000 0000 0000     
#define BORDER_GREEN    0x400           //  0000 0100 0000 0000
#define BORDER_RED      0x200           //  0000 0010 0000 0000
#define BORDER_MASK     0xE00           //  0000 1110 0000 0000     从右到左，在第10到12位，在后面的代码注释中，重点理解掩码的作用：任何
//                                                                  数，跟掩码取反后与运算，该数掩码位都变成0，也就是清0的作用
#define B_SOLID         0               //  0000 0000 0000 0000
#define B_DOTTED        0x1000          //  0001 0000 0000 0000
#define B_DASHED        0x2000          //  0010 0000 0000 0000
#define STYLE_MASK      0x3000          //  0011 0000 0000 0000     从右到左，在第13到14位，在后面的代码注释中，重点理解掩码的作用：任何
//                                                                  数，跟掩码取反后与运算，该数掩码位都变成0，也就是清0的作用
const char *colors[8] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
// 索引                     0       1       2        3         4        5         6       7
// 对应的正是颜色值         000     001     010      011       100      101       110     111

struct box_props {                      // 加起来正好16位，占2字节
    bool opaque                 : 1;    // 低位，最右边算起，1表示不透明，0表示透明
    unsigned int fill_color     : 3;    // 填充颜色，占3位，看颜色二进制值
    unsigned int                : 4;    // 未命名结构体成员，填充 4 位，下图用 x 表示
    bool show_border            : 1;    // 是否显示边框，1显示，0不显示
    unsigned int border_color   : 3;    // 边框颜色，占3位，看颜色二进制值
    unsigned int border_style   : 2;    // 边框样式，占2位，
    unsigned int                : 2;    // 高位，在最左边，看边框线样式的二进制值
};
// 这个结构在内存中的位模式如下
// 内存地址位            高位                                                     低位(内存起始地址，由于us_view和此结构体共用内存，
//                                                                               所以us_view的值，也就是这个起始位置所指向的值，也就是1)
// 二进制位           15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0 
// 对应的值                     2          2       1                      3        1
// 对应值含义                 DASHED     GREEN    true                  YELLOW    true 
//                  ┌───┬───╂───┬───╂───┬───┬───╂───╂───┬───┬───┬───╂───┬───┬───╂───┐
//                  │ x │ x ┃ 1 │ 0 ┃ 0 │ 1 │ 0 ┃ 1 ┃ x │ x │ x │ x ┃ 0 │ 1 │ 1 ┃ 1 │
//                  └───┴─┬─┸───┴─┬─┸───┴───┴─┬─┸─┬─┸───┴───┴───┴─┬─┸───┴───┴─┬─┸─┬─┘
//                        │       │           │   │               │           │   │
// unused 2     <─────────┘       │           │   │               │           │   │
// border_style <─────────────────┘           │   │               │           │   │
// border_color <─────────────────────────────┘   │               │           │   │
// show_border  <─────────────────────────────────┘               │           │   │
// unused 4     <─────────────────────────────────────────────────┘           │   │
// fill_color   <─────────────────────────────────────────────────────────────┘   │
// opaque       <─────────────────────────────────────────────────────────────────┘ 

union Views { /* 把数据看作结构或unsigned short类型的变量 */
    struct box_props st_view;   // box_props 结构体占2字节
    unsigned short us_view;     // short 类型变量也是占2字节
};
// union 是联合体，共享内存意思，也就是 us_view 和 st_view 地址相同，因此读取 us_view，也就是读 st_view 的数据
// unsigned short us_view 正好是二字节，占16位，跟 box_props 的位相同，所以可以把数据看作结构类型变量

void show_settings(const struct box_props *pb);
void show_settings1(unsigned short);
char *itobs(int n, char *ps);
void show_bstr (const char * str);

int main(void) {
    /* 创建Views联合，并初始化 initialize struct box view */
    union Views box = {{true, YELLOW, true, GREEN, DASHED}};
    //                    1     3       1     2       2
    //                    1    011      1    010      10
    // 注意：未命名的结构位，起到填充占位的作用，没有实际值，用 x 显示代替
    // 初始化上面的 box 后，内存中二进制位模式如上图所示，由于 Views 是一个共享内存的联合体，所以 us_view 的内存地址，跟 st_view 是一样的
    // 因此us_view的值，就是内存起始位置第一个值，也就是1（但如果box_props所有成员类型和us_view类型一样，就是另外情况，请看 test.c ）
    char bin_str[8 * sizeof(unsigned int) + 1];
    printf("ORIGINAL BOX SETTINGS:\n");
    show_settings(&box.st_view);
    printf("\nBox settings using unsigned int view:\n");
    show_settings1(box.us_view);
    itobs(box.us_view, bin_str);
    printf("bits are            : ");
    show_bstr(bin_str);
    printf("\n\n=========================================\n");
    // box.us_view = 1 = 0001                       // 上面已经解释了为什么运行到这里这个值为1
    box.us_view &= ~FILL_MASK;                      /* 把表示填充色的位清0，如代码31行所描述的一样 */
    //      FILL_MASK = 0xE = 1110
    //                 ~0xE = 0001                  // 取反就是1为0，0为1
    //              us_view = 0001 
    //                     &= 0001 = 1
    box.us_view |= FILL_CYAN;                       /* 重置填充色， 或运算可以理解为赋上值的意思，因为0或1就等1*/
    //      FILL_BLUE = 0x8 = 1000
    //     FILL_GREEN = 0x4 = 0100
    //            0x8 | 0x4 = 1100
    //              us_view = 0001 
    //                     |= 1101 = 13
    box.us_view ^= OPAQUE;                          /* 切换是否透明的位，异或运算就是找出不同的地方，只要不同就为1，只要相同就为0 */
    //         OPAQUE = 0x1 = 0001
    //              us_view = 1101 
    //                     ^= 1100 = 12             // 异或运算就是相同的为0，不同的为1
    box.us_view |= BORDER_RED;                      /* 设置边框为红色，上面说了，或运算其实就是赋值的意思，对着二进制位，仔细思考和理解 */
    //   BORDER_RED = 0x200 = 0010 0000 0000
    //              us_view = 0000 0000 1100
    //                     |= 0010 0000 1100 = 524  // 或就是有一个为1就是1
    box.us_view &= ~STYLE_MASK;                     /* 把样式的位清0，如代码42行所描述的一样 */
    //  STYLE_MASK = 0x3000 = 0011 0000 0000 0000
    //              ~0x3000 = 1100 1111 1111 1111
    //              us_view = 0000 0010 0000 1100
    //                     &= 0000 0010 0000 1100 = 524
    box.us_view |= B_DOTTED;                        /* 把样式设置为点 */
    //    B_DOTTED = 0x1000 = 0001 0000 0000 0000
    //              us_view = 0000 0010 0000 1100
    //                     |= 0001 0010 0000 1100 = 4620
    printf("\nMODIFIED BOX SETTINGS:\n");
    show_settings(&box.st_view);
    printf("\nBox settings using unsigned int view:\n");
    show_settings1(box.us_view);
    itobs(box.us_view, bin_str);
    printf("bits are            : ");
    show_bstr(bin_str);
    printf("\n");
    getchar();
    return 0;
}

void show_settings(const struct box_props *pb) {
    printf("Box is              : %s\n", pb->opaque == true ? "opaque" : "transparent");
    printf("The fill color is   : %s\n", colors[pb->fill_color]);
    printf("Border              : %s\n", pb->show_border == true ? "shown" : "not shown");
    printf("The border color is : %s\n", colors[pb->border_color]);
    printf("The border style is : ");
    switch (pb->border_style) {
    case SOLID:
        printf("solid\n");
        break;
    case DOTTED:
        printf("dotted\n");
        break;
    case DASHED:
        printf("dashed\n");
        break;
    default:
        printf("unknown type\n");
    }
}

void show_settings1(unsigned short us) {
    printf("box is              : %s\n", (us & OPAQUE) == OPAQUE ? "opaque" : "transparent");
    printf("The fill color is   : %s\n", colors[(us >> 1) & 07]);
    // 这里重点理解，为什么右移 1 位，因为 fill_color 就是 2 到 4 位，把最右边 1 位的 opaque 移出去，
    // 然后因为 fill_color 占 3 位，所以和 07 与运算，即和 111 进行与运算，得到的值，正是颜色的下标（索引），然后通过下标得到颜色名
    printf("Border              : %s\n", (us & BORDER) == BORDER ? "shown" : "not shown");
    printf("The border style is : ");
    switch (us & STYLE_MASK) {
    case B_SOLID:
        printf("solid\n");
        break;
    case B_DOTTED:
        printf("dotted\n");
        break;
    case B_DASHED:
        printf("dashed\n");
        break;
    default:
        printf("unknown type\n");
    }
    printf("The border color is : %s\n", colors[(us >> 9) & 07]);   // 07 是八进制书写方式，前面加0
    // 这里重点理解，为什么右移 9 位，因为 border_color 就是 10 到 12 位，把最右边 9 位移出去，就剩下含 border_color 的值了
    // 然后因为 border_color 占 3 位，所以和 07 与运算，即和 111 进行与运算，得到的值，正是颜色的下标（索引），然后通过下标得到颜色名
}

char *itobs(int n, char *ps) {
    int i;
    const static int size = CHAR_BIT * sizeof(short);
    for (i = size - 1; i >= 0; i--, n >>= 1)
        ps[i] = (01 & n) + '0';                                     // 01 是八进制书写方式，前面加0
    ps[size] = '\0';
    return ps;
}

void show_bstr (const char * str) {
    int i = 0;
    while (str[i]) {        // 把上面转化成二进制的字符串，按4个一组分开来
        putchar (str[i]);   // 输出二进制数字字符（0或1）
        if (++i % 4 == 0 && str[i])
            putchar(' ');
    }
}