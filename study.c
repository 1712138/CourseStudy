#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "study.h"

/**
 * 主入口函数main
 */
int main(void)
{
    // 程序开始的时候，就先显示菜单，让学生选择操作命令
    show_menu();

    // 主函数返回值是 int类型，所以要返回 int 类型的值，一般返回0就表示没有发生错误，有错误返回非0
    return 0;
}

/**
 * 显示帮助主菜单
 */
void show_help(void)
{
    printf("================================== Command Menu ==================================\n");
    printf("| To choose a function, enter its letter label:                                  |\n");
    printf("|                                                                                |\n");
    printf("| a) Show all courses or add them, MAX 3 courses.                                |\n");
    printf("| b) Show all chapters by <CourseCode> or add them, MAX 500 chapters.            |\n");
    printf("| c) Show all units by <ChapterCode> or add them, MAX 5000 units.                |\n");
    printf("| d) Enter <StudentID> <ChapterCode> to start learning.                          |\n");
    printf("| e) Show all study records by <StudentID>.                                      |\n");
    printf("| f) Show the learning time of <StudentID> <CourseCode> <CapterCode> <UnitFile>. |\n");
    printf("| q) Quit                                                                        |\n");
    printf("==================================================================================\n");
    printf("Enter a letter: "); // !!!注意，这里不换行，直接在后面输入字母
}

/**
 * 获取主菜单命令的输入
 */
void show_menu(void)
{
    // 定义当前获取输入字符串的一些变量
    char current_student_id[MAX_STUDENT_ID];
    char current_course_code[MAX_COURSE_CODE];
    char current_chapter_code[MAX_CHAPTER_CODE];
    char current_unit_file[MAX_UNIT_FILE];
    // 定义命令字符
    char command;
    do
    {
        show_help();         // 每次循环开始的时候，就显示命令菜单
        command = getchar(); // 获取一个命令，每次获取后，清除缓存
        fflush(stdin);
        switch (command) // 判断每个命令
        {
        case 'a':
            // 显示所有课程
            show_courses();
            puts("");       // 注意加一个换行，是为了美观，所有地方都是这样，下同
            break;
        case 'b':
            // 提示输入课程号，然后显示该课程的所有章节
            printf("\nShow all chapters by <CourseCode>, Press [Enter] to return.\nEnter CourseCode: ");
            if (fgets(current_course_code, sizeof(current_course_code), stdin) != NULL) // 获取键盘输入的课程号
            {

                del_break(current_course_code); // 去掉最后面的\n
                str_toupper(current_course_code);
                if (strlen(current_course_code) > 0)
                {
                    // 调用函数 show_chapters，并将输入的字符串作为参数传递
                    show_chapters(current_course_code);
                }
            }
            puts("");
            break;
        case 'c':
            // 提示输入章节号，以显示该章节下的所有单元
            printf("\nShow all units by <ChapterCode>, Press [Enter] to return.\nEnter ChapterCode: ");
            // 获取键盘输入的章节号
            if (fgets(current_chapter_code, sizeof(current_chapter_code), stdin) != NULL)
            {
                del_break(current_chapter_code); // 去掉最后面的\n
                if (strlen(current_chapter_code) > 0)
                {
                    show_units(current_chapter_code);
                }
            }
            puts("");
            break;
        case 'd':
            // 提示输入的学号和章节号
            puts("\nEnter <StudentID> <ChapterCode> to start learning, Press [Enter] to return.");
            scanf("%s %s", current_student_id, current_chapter_code);
            fflush(stdin);
            // if (strlen(current_student_id) > 0 && strlen(current_chapter_code) > 0)
            // 这里也可以用下面的来判断
            if (current_student_id[0]!='\0' && current_chapter_code[0]!='\0')
            {
                str_toupper(current_student_id);
                choose_unit(current_student_id, current_chapter_code);
            }
            puts("");
            break;
        case 'e':
            // 提示输入学号，查看该学生所有的学习记录
            printf("\nShow all study records by <StudentID>, Press [Enter] to return.\nEnter StudentID: ");
            if (fgets(current_student_id, sizeof(current_student_id), stdin) != NULL)
            {
                del_break(current_student_id); // 去掉最后面的\n
                str_toupper(current_student_id);
                if (strlen(current_student_id) > 0)
                {
                    show_studies(current_student_id);
                }
            }
            puts("");
            break;
        case 'f':
            // 初始化所有变量为空字符串
            current_student_id[0] = '\0';
            current_course_code[0] = '\0';
            current_chapter_code[0] = '\0';
            current_unit_file[0] = '\0';
            // 提示输入学号，查看该学生所有的学习记录
            printf("\nShow learning time by <StudentID> <CourseCode> <ChapterCode> <UnitFile>, Press [Enter] to return.\n");
            printf("Enter StudentID  : ");
            if (fgets(current_student_id, sizeof(current_student_id), stdin) != NULL)
            {
                fflush(stdin);
                del_break(current_student_id);
                str_toupper(current_student_id);
                if (strlen(current_student_id) == 0)
                {
                    puts("");
                    break;
                }
            }
            printf("Enter CourseCode : ");
            if (fgets(current_course_code, sizeof(current_course_code), stdin) != NULL)
            {
                fflush(stdin);
                del_break(current_course_code);
                str_toupper(current_course_code);
                if (strlen(current_course_code) == 0)
                {
                    show_learning_time(current_student_id, "", "", "");
                    puts("");
                    break;
                }
            }
            printf("Enter ChapterCode: ");
            if (fgets(current_chapter_code, sizeof(current_chapter_code), stdin) != NULL)
            {
                fflush(stdin);
                del_break(current_chapter_code);
                if (strlen(current_chapter_code) == 0)
                {
                    show_learning_time(current_student_id, current_course_code, "", "");
                    puts("");
                    break;
                }
            }
            printf("Enter UnitFile   : ");
            if (fgets(current_unit_file, sizeof(current_unit_file), stdin) != NULL)
            {
                fflush(stdin);
                del_break(current_unit_file);
                if (strlen(current_unit_file) == 0)
                {
                    show_learning_time(current_student_id, current_course_code, current_chapter_code, "");
                    puts("");
                    break;
                }
            }
            show_learning_time(current_student_id, current_course_code, current_chapter_code, current_unit_file);
            puts("");
            break;
        default:
            break;
        }
    } while (command != 'q'); // 直到按 q 退出
}

/**
 * 显示所有课程内容功能，如果还没有课程，则输入，按 [Enter] 返回
 */
void show_courses(void)
{
    puts("");
    // 1. 先定义课程数组，用来存放从课程二进制文件中读出来的所有课程
    struct Course courseList[MAX_COURSE_NUMBER];

    // 2. 打开文件：定义课程指针，并打开课程文件
    FILE *pCourse;
    if ((pCourse = fopen("courses.dat", "a+b")) == NULL) // a+b 是以二进制追加方式打开
    {
        file_not_found("courses.dat");
        return;
    }

    // 3. 从文件中读取课程内容并循环显示
    rewind(pCourse); // 指针定位到开始处
    int count = 0;
    int size = sizeof(struct Course);
    while (count < MAX_COURSE_NUMBER && fread(&courseList[count], size, 1, pCourse) == 1)
    {
        if (count == 0)
        {
            printf("%-12s%s\n", "CourseCode", "CourseName");
            line();
        }
        printf("%-12s%s\n", courseList[count].courseCode, courseList[count].courseName);
        count++;
    }

    // 4. 读取完后，如果超出了最大课程数，则返回，后面不用再录入了
    int filecount = count; // 这里要先记录一下当前记录总数，因为后面如果有添加的话，要从这个位置开始写文件
    if (count == MAX_COURSE_NUMBER)
    {
        line();
        printf("%-12s%d courses\n\n", "Total", count);
        file_is_fill("courses.dat");
        return;
    }
    line();
    printf("%-12s%d courses\n\n", "Total", count);

    // 5. 如果没有超出课程数，则提示是否继续录入，是输入课程号录入
    if (continue_add("course") != 'y')
        return;
    printf("Enter CourseCode: ");
    while (count < MAX_COURSE_NUMBER && s_gets(courseList[count].courseCode, MAX_COURSE_CODE) != NULL && courseList[count].courseCode[0] != '\0')
    {
        printf("Enter CourseName: "); // 继续输入课程名
        s_gets(courseList[count].courseName, MAX_COURSE_NAME);
        if (strlen(courseList[count].courseName) == 0)
        { // 如果只输入课程号，没输入课程名，则作废，不存进去，count--退回去，然后后面count++，说明还是这个位置
            count--;
        }
        count++;
        if (count < MAX_COURSE_NUMBER) // 如果还没有达到最大课程数，则继续提示输入课程号
            printf("Enter CourseCode: ");
    }
    puts("");

    // 6. 输入完课程后，再从头开始显示刚才新增的所有课程，
    int index;
    if (count > 0)
    {
        printf("%-12s%s\n", "CourseCode", "CourseName");
        line();
        for (index = 0; index < count; index++)
            printf("%-12s%s\n", courseList[index].courseCode, courseList[index].courseName);
        fwrite(&courseList[filecount], size, count - filecount, pCourse); // 写入文件
    }
    else // 没有录入课程，提示
        file_is_empty("courses.dat");

    // 7. 一定要关闭文件指针
    fclose(pCourse);
}

/**
 * 显示指定课程的所有章节，如果还没有章节，则输入，按 [Enter] 返回
 */
void show_chapters(char *course_code)
{
    puts("");

    // 先判断课程二进制数据中，是否存在 course_code 的课程，如果不存在则提示并返回
    struct Course *p;
    if ((p = find_course(course_code)) == NULL)
    {
        printf("%s is not exists in courses.dat\n", course_code);
        return;
    }
    // *p 的值，就是返回的课程内容，指针变量可以通过以下两种方式访问成员
    printf("The chapters of all [%s: %s] are as follows\n\n", (*p).courseCode, p->courseName);

    // 先定义章节数组，用来存放所有章节
    struct Chapter chapterList[MAX_CHAPTER_NUMBER];

    // 定义章节指针，并打开章节文件
    FILE *pChapter;
    if ((pChapter = fopen("chapters.dat", "a+b")) == NULL)
    {
        file_not_found("chapters.dat");
        return;
    }

    // 循环读取章节内容并显示
    rewind(pChapter);
    int count = 0;
    int findCount = 0;
    int size = sizeof(struct Chapter);
    while (count < MAX_CHAPTER_NUMBER && fread(&chapterList[count], size, 1, pChapter) == 1)
    {
        if (count == 0)
        {
            printf("%-12s%s\n", "CourseCode", "Chapter");
            line();
        }
        if (strcmp(chapterList[count].courseCode, course_code) == 0) // 如果是参数指定的课程代码才显示
        {
            if (strlen(chapterList[count].chapterCode) == 2)
            {
                printf("%-12s%s %s\n", chapterList[count].courseCode, chapterList[count].chapterCode, chapterList[count].chapterName);
            }
            else
            {
                char cc[MAX_CHAPTER_CODE];
                strncpy(cc, chapterList[count].chapterCode + 3, 2);
                printf("%-12s   %s %s\n", chapterList[count].courseCode, cc, chapterList[count].chapterName);
            }
            findCount++;
        }
        count++;
    }

    // 读取完后，如果超出了最大章节数，则返回，后面不用再录入了
    int filecount = count; // 记录当前的章节数，后面要从这个位置写数据
    if (count == MAX_CHAPTER_NUMBER)
    {
        line();
        printf("%-15s%d chapters\n\n", "Total", count);
        file_is_fill("chapters.dat");
        return;
    }
    line();
    printf("%-15s%d chapters\n\n", "Total", findCount);

    // 如果没有超出章节数，则提示输入章节号继续录入
    if (continue_add("chapters") != 'y')
        return;
    printf("Enter ChapterCode: ");
    while (count < MAX_CHAPTER_NUMBER && s_gets(chapterList[count].chapterCode, MAX_CHAPTER_CODE) != NULL && chapterList[count].chapterCode[0] != '\0')
    {
        // 把课程号先放到数组元素中，章节结构体有课程号字段
        strcpy(chapterList[count].courseCode, course_code);
        // 提示输入章节名称
        printf("Enter ChapterName: ");
        // 获取输入的章节名
        s_gets(chapterList[count].chapterName, MAX_CHAPTER_NAME);

        if (strlen(chapterList[count].chapterName) == 0)
        { // 如果只输入章节号，而没有输入章节名，那么这条数据作废，就是count不往后移的意思，意思是不存进去，count--退回去，然后后面count++，说明还是这个位置
            count--;
        }
        count++;
        // 如果没有达到最大章节数，继续提示输入
        if (count < MAX_CHAPTER_NUMBER)
        {
            // printf("Continue add Chapter, Press [Enter] to return.\nEnter ChapterCode: ");
            printf("Enter ChapterCode: ");
        }
    }
    puts("");

    // 输入完章节后，再显示刚才新增的所有章节，
    int index;
    if (count > 0)
    {
        printf("%-12s%s\n", "CourseCode", "Chapter");
        line();
        for (index = 0; index < count; index++)
        {
            /* char a[9];
            char b[3];
            char c[3];
            char d[9];
            int i, j;
            strcpy(a, chapterList[index].chapterCode);
            if (strlen(a)==2) {
                i = atoi(a);
                sprintf(d, "%d",i);
            } else {
                strncpy(b, a, 2);
                i = atoi(b);
                strncpy(c, a+3, 3);
                j = atoi(c);
                sprintf(d, "%d.%d",i,j);
            }
            strcpy(chapterList[index].chapterCode, d);
            FILE *p;
            p = fopen("chapters.dat", "rb");
            fwrite(&chapterList[index], sizeof(struct Chapter), 1, p);
            fclose(p); */

            if (strcmp(chapterList[index].courseCode, course_code) == 0)
            { // 如果是参数指定的课程代码才显示
                if (strlen(chapterList[index].chapterCode) == 2)
                {
                    printf("%-12s%s %s\n", chapterList[index].courseCode, chapterList[index].chapterCode, chapterList[index].chapterName);
                }
                else
                {
                    char cc[MAX_CHAPTER_CODE];
                    strncpy(cc, chapterList[index].chapterCode + 3, 2);
                    printf("%-12s   %s %s\n", chapterList[index].courseCode, cc, chapterList[index].chapterName);
                }
            }
        }
        // 在filecount 位置写入新增的章节数据
        int blocks = count - filecount;
        fwrite(&chapterList[filecount], size, blocks, pChapter);
    }
    else // 没有录入章节，提示
        file_is_empty("chapters.dat");

    // 一定要关闭打开的文件指针
    fclose(pChapter);
}

/**
 * 显示指定章节的所有单元，如果还没有，则输入，按 [Enter] 返回
 */
void show_units(char *chapter_code)
{
    puts("");

    // ？？先判断章节二进制数据中，是否存在 chapter_code 的章节，如果不存在则提示，并返回1

    // 先定义单元数组，用来存放所有单元数据
    struct Unit unitList[MAX_UNIT_NUMBER];

    // 定义单元指针，并打开单元文件
    FILE *pUnit;
    if ((pUnit = fopen("units.dat", "a+b")) == NULL)
    {
        file_not_found("units.dat");
        return;
    }

    // 循环读取单元内容并显示
    rewind(pUnit);
    int count = 0;
    int findCount = 0;
    int size = sizeof(struct Unit);
    while (count < MAX_UNIT_NUMBER && fread(&unitList[count], size, 1, pUnit) == 1)
    {
        if (count == 0)
        {
            printf("%-14s%s\n", "UnitCode", "UnitName");
            line();
        }
        if (strcmp(unitList[count].chapterCode, chapter_code) == 0) // 如果是参数指定的课程代码才显示
        {
            printf("%-14s%s\n", unitList[count].unitFile, unitList[count].unitName);
            findCount++;
        }
        count++;
    }
    line();
    printf("%-14s%d units\n\n", "Total", findCount);

    // 读取完后，如果超出了最大单元数，则返回，后面不用再录入了
    int filecount = count;
    if (count == MAX_UNIT_NUMBER)
    {
        file_is_fill("units.dat");
        return;
    }

    // 如果没有超出章节数，则提示输入单元文件名继续录入
    if (continue_add("units") != 'y')
        return;
    printf("Enter UnitFile: ");
    // 获取单元文件名
    while (count < MAX_UNIT_NUMBER && s_gets(unitList[count].unitFile, MAX_UNIT_FILE) != NULL && unitList[count].unitFile[0] != '\0')
    {
        // 如果文件名没有 .txt 后缀，自动补上
        if (strstr(unitList[count].unitFile, ".txt") == NULL)
        {
            strcat(unitList[count].unitFile, ".txt");
        }
        // 通过章节号查找章节对应的课程号
        struct Chapter foundChapter;
        find_chapter(chapter_code, &foundChapter);
        strcpy(unitList[count].courseCode, foundChapter.courseCode);
        // 将章节号先存在单元数据元素中
        strcpy(unitList[count].chapterCode, chapter_code);
        // 然后提示输入单元名
        printf("Enter UnitName: ");
        // 等待获取单元名
        s_gets(unitList[count].unitName, MAX_UNIT_NAME);
        if (strlen(unitList[count].unitName) == 0)
        { // 如果只输入单元名，而没有输入单元标题，那么这条数据作废，就是count不往后移的意思，count--退回去，然后后面count++，说明还是这个位置
            count--;
        }
        count++;
        // 如果还没有到达最大单元数据，则提示继续录入
        if (count < MAX_UNIT_NUMBER)
            printf("Enter UnitFile: ");
    }
    puts("");

    // 输入完单元后，再显示刚才新增的所有单元
    int index;
    if (count > 0)
    {
        printf("%-14s%s\n", "UnitCode", "UnitName");
        line();
        for (index = 0; index < count; index++)
        {
            if (strcmp(unitList[index].chapterCode, chapter_code) == 0) // 如果是参数指定的课程代码才显示
                printf("%-14s%s\n", unitList[index].unitFile, unitList[index].unitName);
        }

        fwrite(&unitList[filecount], size, count - filecount, pUnit);
    }
    else // 没有录入，提示
        file_is_empty("units.dat");

    // 一定要关闭打开的文件指针
    fclose(pUnit);
}

/**
 * 显示参数指定的学号的所有学习记录。
 */
void show_studies(char *student_id)
{
    puts("");

    // 定义指针，并打开文件
    FILE *pStudy;
    if ((pStudy = fopen("study.dat", "rb")) == NULL)
    {
        file_not_found("study.dat");
        return;
    }

    int size = sizeof(struct Study);
    long fileSize, recordCount;
    fseek(pStudy, 0, SEEK_END);    // 获取文件大小前，一定要先把文件指针移到最后
    fileSize = ftell(pStudy);      // 文件总字节数
    recordCount = fileSize / size; // 计算记录总数

    // 先定义学习记录数组，用来存放所有学习记录，注意！！！！这里，由于没有定义 MAX_STUDY_NUMBER，所以要动态分配内存
    struct Study *studyList = (struct Study *)malloc(recordCount * size);

    // 读取内容并显示
    rewind(pStudy);
    int count = 0;
    int foundCount = 0;
    int timeCount = 0;

    while (count < recordCount && fread(&studyList[count], size, 1, pStudy) == 1)
    {
        if (count == 0)
        {
            printf("StudentID %s study records in study.dat:\n\n", student_id);
            printf("%-10s%-11s%-12s%-13s%-20s%16s\n", "StudentID", "CourseCode", "ChapterCode", "UnitFile", "StartTime", "TotalTime");
            line();
        }
        if (strcmp(studyList[count].studentID, student_id) == 0)
        {
            struct tm *timeinfo = gmtime(&studyList[count].startTime);
            char timestr[20];
            strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", timeinfo);
            printf("%-10s%-11s%-12s%-13s%-20s%8d seconds\n",
                   studyList[count].studentID, studyList[count].courseCode, studyList[count].chapterCode, studyList[count].unitFile, timestr, studyList[count].totalTime);
            foundCount++;
            timeCount += studyList[count].totalTime;
        }
        count++;
    }
    line();
    printf("%-10s%d %-54s%8d seconds", "Total", foundCount, "records", timeCount);
    puts("");
    fflush(stdin);

    // 注意！！！！动态分配内存后，一定要手动释放内存
    free(studyList);

    // 一定要关闭打开的文件指针
    fclose(pStudy);
    printf("\nPress any Key to return.");
    getchar();
}

/**
 * 根据指定的参数，显示学习时间
 */
void show_learning_time(char *student_id, char *course_code, char *chapter_code, char *unit_file)
{
    FILE *pStudy;
    if ((pStudy = fopen("study.dat", "rb")) == NULL)
    {
        file_not_found("study.dat");
        return;
    }

    fseek(pStudy, 0, SEEK_END);
    long fileSize = ftell(pStudy);
    long size = sizeof(struct Study);
    long fileCount = fileSize / size;

    int total_time_count[4] = {0, 0, 0, 0};
    struct Study myStudy;
    char full_unit_file[MAX_UNIT_FILE];
    strcpy(full_unit_file, unit_file);
    strcat(full_unit_file, ".txt");

    rewind(pStudy);
    for (int i = 0; i < fileCount; i++)
    {
        fread(&myStudy, size, 1, pStudy);
        if (strcmp(myStudy.studentID, student_id) == 0)
        {
            total_time_count[0] += myStudy.totalTime;
        }
        if (strcmp(myStudy.studentID, student_id) == 0 && strcmp(myStudy.courseCode, course_code) == 0)
        {
            total_time_count[1] += myStudy.totalTime;
        }
        if (strcmp(myStudy.studentID, student_id) == 0 && strcmp(myStudy.courseCode, course_code) == 0 && strcmp(myStudy.chapterCode, chapter_code) == 0)
        {
            total_time_count[2] += myStudy.totalTime;
        }
        if (strcmp(myStudy.studentID, student_id) == 0 && strcmp(myStudy.courseCode, course_code) == 0 && strcmp(myStudy.chapterCode, chapter_code) == 0 && strcmp(myStudy.unitFile, full_unit_file) == 0)
        {
            total_time_count[3] += myStudy.totalTime;
        }
    }
    puts("");
    printf("%-15s %-15s %s\n", "Item", "Value", "Total Time");
    line();
    if (strlen(student_id) > 0)
        printf("%-15s %-15s %-4d seconds\n", "Student", student_id, total_time_count[0]);
    if (strlen(course_code) > 0)
        printf("%-15s %-15s %-4d seconds\n", "CourseCode", course_code, total_time_count[1]);
    if (strlen(chapter_code) > 0)
        printf("%-15s %-15s %-4d seconds\n", "ChapterCode", chapter_code, total_time_count[2]);
    if (strlen(unit_file) > 0)
        printf("%-15s %-15s %-4d seconds\n", "UnitFile", unit_file, total_time_count[3]);
    printf("\nPress any Key to return.");
    getchar();
}

/**
 * 根据课程号查找，返回课程的结构体数据
 */
struct Course *find_course(char *course_code)
{
    // 打开课程二进制文件
    FILE *pCourse;
    if ((pCourse = fopen("courses.dat", "rb")) == NULL)
    {
        file_not_found("courses.dat");
        return NULL;
    }

    // 读取课程内容并显示
    rewind(pCourse);
    int count = 0;
    int size = sizeof(struct Course);
    // 定义课程数组，用来存放读取的课程数据
    // struct Course courseList[MAX_COURSE_NUMBER];
    struct Course myCourse;
    struct Course *foundCourse = NULL;
    // int block = fread(&courseList[count], size, 1, pCourse);
    while (count < MAX_COURSE_NUMBER && fread(&myCourse, size, 1, pCourse) == 1)
    {
        if (strcmp(myCourse.courseCode, course_code) == 0)
        {
            foundCourse = &myCourse;
            break;
        }
        count++;
    }
    // 记得关闭文件指针
    fclose(pCourse);
    return foundCourse;
}

/**
 * 根据章节号，查找章节，返回章节的结构体数据
 */
void find_chapter(char *chapter_code, struct Chapter *chapter)
{
    // 定义章节数组，用来存放读取的章节数据
    struct Chapter chapterList[MAX_CHAPTER_NUMBER];

    // 打开章节二进制文件
    FILE *pChapter;
    if ((pChapter = fopen("chapters.dat", "r")) == NULL)
    {
        file_not_found("chapters.dat");
        return;
    }

    // 读取章节内容并显示
    rewind(pChapter);
    int count = 0;
    int size = sizeof(struct Chapter);
    while (count < MAX_CHAPTER_NUMBER && fread(&chapterList[count], size, 1, pChapter) == 1)
    {
        if (strcmp(chapterList[count].chapterCode, chapter_code) == 0)
        {
            *chapter = chapterList[count];
        }
        count++;
    }
    // 记得关闭文件指针
    fclose(pChapter);
}

/**
 * 查找单元，根据 单元文件名查找，返回单元结构体数据
 */
void find_unit(char *unit_file, struct Unit *unit)
{
    // 定义单元结构数组，用来存放文件读出来的数据
    struct Unit unitList[MAX_UNIT_NUMBER];
    FILE *pUnit;
    if ((pUnit = fopen("units.dat", "r")) == NULL)
    {
        file_not_found("units.dat");
        return;
    }

    // 读取单元内容并显示
    rewind(pUnit);
    int count = 0;
    int size = sizeof(struct Unit);
    while (count < MAX_CHAPTER_NUMBER && fread(&unitList[count], size, 1, pUnit) == 1)
    {
        if (strcmp(unitList[count].unitFile, unit_file) == 0)
        {
            *unit = unitList[count];
        }
        count++;
    }
    // 记得关闭文件指针
    fclose(pUnit);
}

/**
 * 根据传入的参数，查找知识点学习记录，返回 Study 的结构体，指针 POS 返回的是找到记录的索引
 */
void find_study(char *student_id, char *course_code, char *chapter_code, char *unit_file, struct Study *study, int *pos)
{
    // 定义指针，并打开文件
    FILE *pStudy;
    if ((pStudy = fopen("study.dat", "a+b")) == NULL)
    {
        file_not_found("study.dat");
        return;
    }

    // 获取文件大小前，要先把指针移到最后
    fseek(pStudy, 0, SEEK_END);
    long fileSize = ftell(pStudy); // 获取文件大小
    long size = sizeof(struct Study);
    long recordCount = fileSize / size;                                   // 计算记录总数
    struct Study *studyList = (struct Study *)malloc(recordCount * size); // 先定义数组，用来存放所有学习记录

    // 读取内容并显示，要先把指针移到最前
    rewind(pStudy);
    int count = 0;
    while (count < recordCount && fread(&studyList[count], size, 1, pStudy) == 1)
    {
        if (strcmp(studyList[count].studentID, student_id) == 0 && strcmp(studyList[count].courseCode, course_code) == 0 && strcmp(studyList[count].chapterCode, chapter_code) == 0 && strcmp(studyList[count].unitFile, unit_file) == 0)
        {
            *pos = count;
            *study = studyList[count];
        }
        count++;
    }
    fclose(pStudy);
}

/**
 * 根据章节号查该章节下的所有知识点，选择学习哪个知识点，并先传入学号，供保存学习记录用
 */
void choose_unit(char *student_id, char *chapter_code)
{
    puts("");

    // 先定义一个单元数组，把单元文件读到的数据放这里
    struct Unit unitList[MAX_UNIT_NUMBER];

    // 先打开文件，如果文件不存在，则提示，
    FILE *pUnit;
    if ((pUnit = fopen("units.dat", "r")) == NULL)
    {
        file_not_found("units.dat");
        return;
    }

    // 循环读取单元文件的数据，并把它放到单元数组中，判断只有等于 参数指定的章节号 的才显示出来，读取完成后，关闭指针
    rewind(pUnit);
    int count = 0;
    int size = sizeof(struct Unit);

    /* struct Unit unit;
    while (fread(&unit, size, 1, pUnit) == 1) {
        if (count == 0)
            printf("Current %s contents of units.dat:\n\n", chapter_code);
        if (strcmp(unit.chapterCode, chapter_code) == 0) // 如果是参数指定的课程代码才显示
        {
            printf("%2d) %-12s: %s\n", count + 1, unit.unitFile, unit.unitName);
        }
        count++;
    } */
    while (count < MAX_UNIT_NUMBER && fread(&unitList[count], size, 1, pUnit) == 1)
    {
        if (count == 0)
            printf("Current %s contents of units.dat:\n\n", chapter_code);
        if (strcmp(unitList[count].chapterCode, chapter_code) == 0) // 如果是参数指定的课程代码才显示
        {
            printf("%2d) %-12s: %s\n", count + 1, unitList[count].unitFile, unitList[count].unitName);
        }
        count++;
    }

    // 关闭文件指针
    fclose(pUnit);
    puts("");

    // 一直等待用户输入序号，
    printf("Enter number to start learning, or [Non-numberic] to return: ");

    // 获取输入的字符，判断是不是数字，如果不是就返回
    char num;
    num = getchar();
    fflush(stdin);
    if (num == '\n' || !isdigit(num))
        return;

    // 把字母的数字，转为整型数字，由于‘1’排在‘0’后，ASCII码相减，正好是整数1
    int number = num - '0';
    // 如果输入的数字大于最大的单元数，则返回
    if (number > count)
        return;

    // 开始学习，这时要记录开始时间
    time_t start_time = time(NULL);

    // 从单元数组中取出单元文件名和单元标题，并存到变量中，后面保存学习记录的时候，要存进去
    char unit_file[MAX_UNIT_FILE];
    strcpy(unit_file, unitList[number - 1].unitFile);
    char unit_name[MAX_UNIT_NAME];
    strcpy(unit_name, unitList[number - 1].unitName);

    // 显示知识点的内容，并一直提示用户，只有按 y 键才结束学习，否则一直显示让学生学习。
    int is_end = open_unit(unit_file, &start_time) == 0;

    if (!is_end)
        return; // 如果没有按结束标志 y

    // 取得当前时间，并用当前时间 - 开始时间，就是这次学习的总时长
    time_t end_time = time(NULL);
    time_t total_time = end_time - start_time;

    // 这个时候要开始写入学习记录了，因为学习记录中要有课程号，所以要先通过章节号找到课程号
    char course_code[MAX_COURSE_CODE];
    struct Chapter foundChapter;
    find_chapter(chapter_code, &foundChapter);
    strcpy(course_code, foundChapter.courseCode);

    // 先从学习记录中找到这个知识点的记录，看是否已经有学习过，有就找出来更新，没有就添加新的学习记录
    int pos = -1;
    struct Study foundStudy;
    // 下面的函数执行后，如果有找到，则pos就是找到的学习记录的索引号，否则就是-1
    find_study(student_id, course_code, chapter_code, unit_file, &foundStudy, &pos);
    if (pos == -1) // 如果没有找到学习记录，把数据填充到本次学习结构体中，
    {
        strcpy(foundStudy.studentID, student_id);
        strcpy(foundStudy.courseCode, course_code);
        strcpy(foundStudy.chapterCode, chapter_code);
        strcpy(foundStudy.unitFile, unit_file);
        strcpy(foundStudy.unitName, unit_name);
        foundStudy.startTime = start_time;
        foundStudy.endTime = end_time;
        foundStudy.totalTime = total_time;

        // 并添加到二进制学习记录文件中，把这次学习的 study 结构内容，作为参数直接传给 append_study
        append_study(&foundStudy);
    }
    else // 如果已经有学习过这个知识点，就把这个知识点原来的学习时间 + 本次学习的时间，然后更新学习记录
    {
        foundStudy.totalTime += total_time;

        // 然后更新这个学习记录，更新时要指定更新哪条数据，所以这里要指定，上面 find_study 找到的序号，并把这次学习的 study 结构内容，作为参数直接传给 update_study
        update_study(&pos, &foundStudy);
    }

    // 保存完二进制学习记录后，还要保存本次的学习日志，把这次学习的 study 结构内容，作为参数直接传进去，在 save_log 中就可以取到数据了
    save_log(&foundStudy);

    // 保存完后，继续显示这个章节的其它知识点，让学生选择是否继续学习其它知识点
    choose_unit(student_id, chapter_code);
}

/**
 * 传入单元文件名，开始读取文件学习，并传入一个开始时间
 */
int open_unit(char *unitFile, time_t *startTime)
{
    puts("");

    // 打开文件
    char full_unitfile[MAX_UNIT_FILE] = "./docs/";
    strcat(full_unitfile, unitFile);
    FILE *pUnit;
    if ((pUnit = fopen(full_unitfile, "r")) == NULL)
    {
        file_not_found(full_unitfile);
        return 1;
    }

    // 定义每次读取的数据块，比 fgetc 一个字符一个字符读取效率更高，
    char buffer[BUFFER_SIZE];
    // 将开始时间戳转为日期结构
    struct tm *timeinfo = gmtime(startTime);
    char timestr[20];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", timeinfo);
    // 打印开头
    printf("------------ Start of the %s at %s -------------\n", unitFile, timestr);
    while (fgets(buffer, BUFFER_SIZE, pUnit) != NULL)
    {
        printf("%s", buffer);
    }
    // 打印结尾
    printf("\n--------------------------------- End ----------------------------------\n\n");
    // 关闭打开的文件指针
    fclose(pUnit);

    // 提示是否结束学习？
    printf("Do you want to end your studies? [y/n]: ");
    char end = getchar();
    fflush(stdin);
    // 如果是就返回0
    if (end == 'y')
    {
        return 0;
    }
    // 否则自己调用自己，按任何键都一直显示单元内容，表示一直在学习中。
    return open_unit(unitFile, startTime);
}

/**
 * 从学习记录数据中，保存学习的日志 为文本文件
 */
void save_log(struct Study *study)
{
    // 打开文件
    FILE *pStudy;
    if ((pStudy = fopen("study.log", "a+")) == NULL)
    {
        file_not_found("study.log");
        return;
    }

    // 将时间戳转为日期格式显示，比较直观
    // struct tm *startTime = gmtime(&(*study).startTime); study->startTime
    struct tm *startTime = gmtime(&study->startTime);
    char st[20];
    strftime(st, sizeof(st), "%Y-%m-%d %H:%M:%S", startTime);
    // 同上
    struct tm *endTime = gmtime(&(*study).endTime);
    char et[20];
    strftime(et, sizeof(et), "%Y-%m-%d %H:%M:%S", endTime);
    // fprintf 可以对文件指针操作数据，你看，写文本文件是不是很容易
    fprintf(pStudy, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%ld\n", (*study).studentID, (*study).courseCode, (*study).chapterCode, (*study).unitFile, (*study).unitName, st, et, (*study).totalTime);

    // 记得一定要关闭
    fclose(pStudy);
}

/**
 * 找不到 单元学习记录 的情况下，就以二进制追加的方式 ，添加学习记录
 */
void append_study(struct Study *study)
{
    // 打开学习记录文件
    FILE *pStudy;
    if ((pStudy = fopen("study.dat", "a+b")) == NULL)
    {
        file_not_found("study.dat");
        return;
    }

    // 获取结构的大小
    int size = sizeof(struct Study);
    // 往 pStudy 文件指针，写入1块大小为size 的数据 study
    fwrite(study, size, 1, pStudy);

    // 关闭指针
    fclose(pStudy);
}

/**
 * 在指定的位置中，更新学习记录
 */
void update_study(int *pos, struct Study *study)
{
    // 打开文件
    FILE *pStudy;
    if ((pStudy = fopen("study.dat", "rb+")) == NULL)
    {
        file_not_found("study.dat");
        return;
    }
    // 获取结构大小
    int size = sizeof(struct Study);
    // 从头开始查找，从POS位置开始的SIZE大小的数据，
    fseek(pStudy, (*pos) * size, SEEK_SET);
    // 记录定位到位置后，开始写大小为SIZE的1块的数据进去
    fwrite(study, size, 1, pStudy);

    // 记得关闭指针
    fclose(pStudy);
}

/**
 * 打印一条线
 */
void line(void)
{
    printf("----------------------------------------------------------------------------------\n");
}

/**
 * 输出文件找不到，按任意键返回
 */
void file_not_found(char *str)
{
    printf("The %s file not found, Press any Key to return.", str);
    getchar();
}

/**
 * 输出文件已满了
 */
void file_is_fill(char *str)
{
    printf("The %s file is full, Press any Key to return.", str);
    getchar();
}

/**
 * 输出文件是空的提示
 */
void file_is_empty(char *str)
{
    printf("The %s file is empty, Press any Key to return.", str);
    getchar();
}

/**
 * 输出是否继续添加
 */
char continue_add(char *str)
{
    printf("Do you want to continue adding %s? [y/<any Key to return>]", str);
    fflush(stdin);
    return getchar();
}

/**
 * 字符串转大写
 */
void str_toupper(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }
}

/**
 * 删除字符串最后面的回车
 */
void del_break(char *str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

/**
 * 获取输入字符串功能
 */
char *s_gets(char *st, int n)
{
    fflush(stdin);
    char *ret_val;
    char *find;
    ret_val = fgets(st, n, stdin);
    if (ret_val) // 键盘输入按回车后到这里，
    {
        find = strchr(st, '\n'); // 这里的意思是，找到回车的位置，把\n改为\0
        if (find)
            *find = '\0';
        else
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}
