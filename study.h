#include <time.h>

/* 这里先定义一些常量，这些可以在定义结构的时候，边补充的*/
#define MAX_STUDENT_ID 8       // 2309174格式，原因：因为字符串要加1位\0
#define MAX_CHAPTER_CODE 9     // 17.12.11格式，原因：要考虑最大章节号情况，因为字符串要加1位 \0
#define MAX_CHAPTER_NAME 100   // 章节名称比较长，先定义为100
#define MAX_COURSE_CODE 7      // CST103格式，要加1位，原因如上
#define MAX_COURSE_NAME 100    // 先定义为100
#define MAX_COURSE_NUMBER 5    // 先定义最大课程数为1，即只学习 CST103
#define MAX_CHAPTER_NUMBER 500 // 先定义最大章节数为500，即每课最多有500个章节
#define BUFFER_SIZE 1024       // 先定义每次读取知识点文本文件的数据缓存块大小
#define MAX_UNIT_FILE 13       // 05.05.01.txt 这种格式的文件名 要加\0
#define MAX_UNIT_NAME 100      // 最大的知识单元名称，先定义为100
#define MAX_UNIT_NUMBER 5000   // 最大的知识单元数量，先定义为5000

/* 这里定义一些基本的结构体 */
struct Course // 定义课程结构
{
    char courseCode[MAX_COURSE_CODE]; // 课程号
    char courseName[MAX_COURSE_NAME]; // 课程名称
};

struct Chapter // 定义章节结构
{
    char chapterCode[MAX_CHAPTER_CODE]; // 章节号
    char chapterName[MAX_CHAPTER_NAME]; // 章节名称
    char courseCode[MAX_COURSE_CODE];   // 章节属于什么课程，这样这个程序可以扩展到其它课程的学习，如数学，马来语等。
};

struct Unit // 定义知识单元结构
{
    char unitFile[MAX_UNIT_FILE];       // 单元文件名，如：01.01.01.TXT
    char unitName[MAX_UNIT_NAME];       // 单元标题
    char courseCode[MAX_COURSE_CODE];   // 记录单元所属的课程
    char chapterCode[MAX_CHAPTER_CODE]; // 单元属于哪个章节
};

struct Study // 定义学习记录结构，用来存储学习记录
{
    char studentID[MAX_STUDENT_ID];     // 学号：2309174
    char courseCode[MAX_COURSE_CODE];   // 课程号：CST103 等
    char chapterCode[MAX_CHAPTER_CODE]; // 单节号：09， 09.01 等
    char unitFile[MAX_UNIT_FILE];       // 单元文件名：05.01.01.txt
    char unitName[MAX_UNIT_NAME];       // 知识单元名称
    time_t startTime;                   // 用时间戳来记录，方便格式化输出各种格式的时间
    time_t endTime;                     // 用时间戳来记录，方便格式化输出各种格式的时间
    time_t totalTime;                   // 总用时，秒
};

/** 定义函数原型 **/
// void test(struct Course s[]);
char *s_gets(char *st, int n);                                  // 从键盘输入获取字符串
void line(void);                                                // 输出线条
void show_help(void);                                           // 显示主菜单
void show_menu(void);                                           // 获取菜单命令的的输入
void show_courses(void);                                        // 显示所有课程
void show_chapters(char *course_code);                          // 根据课程号，显示章节内容
void show_units(char *chapter_code);                            // 根据章节号，显示单元内容
void show_studies(char *student_id);                            // 根据学号，显示某个学生的学习记录
void choose_unit(char *student_id, char *chapter_code);         // 根据学号，章节号，显示单元内容 ，输入序号，学习相应的知识点
int open_unit(char *unitFile, time_t *startTime);               // 传入单元文件名，学习知识单元
struct Course *find_course(char *course_code);                  // 根据课程号查找，返回课程的结构体数据，也可以将找到的课程通过指针返回
void find_chapter(char *chapter_code, struct Chapter *chapter); // 根据章节号，在章节数据文件中，查找章节，返回匹配的章节结构内容
void find_unit(char *unit_file, struct Unit *unit);             // 根据单元文件名，在单元数据文件中，查找单元，返回匹配的单元结构内容
void append_study(struct Study *study);                         // 将学习记录结构内容，添加到学习记录中
void update_study(int *pos, struct Study *study);               // 更新学习内容到指定的序号中
void save_log(struct Study *study);                             // 保存为人类可读的文本文件日志
// 根据学号，课程号，章节号，单元文件名，查找学习记录，存在则更新，不存在则添加，并将序号返回到POS指针中。
void find_study(char *student_id, char *course_code, char *chapter_code, char *unit_file, struct Study *study, int *pos);
void show_learning_time(char *student_id, char *course_code, char *chapter_code, char *unit_file);
void del_break(char *str);      // 删除空行
void str_toupper(char *str);    // 字符串转大写
void file_not_found(char *str); // 输出文件找不到，是否继续添加
void file_is_fill(char *str);   // 输出文件已满
void file_is_empty(char *str);  // 输出文件为空
char continue_add(char *str);   // 是否继续添加