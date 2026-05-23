// 根据章节号查该章节下的所有知识点，并让用户选择学习哪个知识点
int choose_units(char *student_id, char *chapter_code)
{
    // 先定义一个单元数组，把单元文件读到的数据放这里
    
    // 先打开文件，如果文件不存在，则提示，并返回1，一般返回非0 表示出错，非正常执行到尾

    // 循环读取单元文件的数据，并把它放到单元数组中，判断只有等于 参数指定的章节号 的才显示出来，读取完成后，关闭指针

    // 一直等待用户输入序号，
    printf("Enter number to start learning, or [Non-numberic] to return: ");

    // 获取输入的字符，判断是不是数字，如果不是就返回
    char num;
    num = getchar();
    fflush(stdin);
    if (num == '\n' || !isdigit(num))
        return 2;

    // 如果是数字，就找到这个序号对应的单元，开始学习，这时要记录开始时间
    time_t start_time = time(NULL);
    int number = num - '0';
    char unit_file[MAX_UNIT_FILE];
    strcpy(unit_file, unitList[number - 1].unitFile);

    // 显示知识点的内容，并一直提示用户，只有按 e 键才结束学习，否则一直显示让学生学习。
    int is_end = learn(unit_file) == 0;

    // 如果按了结束标志 e
    if (is_end)
    {
        // 取得当前时间，并用当前时间 - 开始时间，就是这次学习的总时长
        time_t end_time = time(NULL);
        time_t total_time = end_time - start_time;

        // 这个时候要开始写入学习记录了，先从学习记录中找到这个知识点的记录，看是否已经有学习过，有就找出来更新，没有就添加新的学习记录
        char course_code[MAX_COURSE_CODE];
        strcpy(course_code, find_chapter(chapter_code).courseCode);
        int pos = -1;
        struct Study myStudy = find_study(student_id, course_code, chapter_code, unit_file, &pos);

        
        if (pos == -1)
        {
            // 把数据填充到本次学习记录中，
            strcpy(myStudy.studentID, student_id);
            strcpy(myStudy.courseCode, course_code);
            strcpy(myStudy.chapterCode, chapter_code);
            strcpy(myStudy.unitFile, unit_file);
            myStudy.startTime = start_time;
            myStudy.endTime = end_time;
            myStudy.totalTime = total_time;

            // 并添加到二进制学习记录文件中，把这次学习的 study 结构内容，作为参数直接传给 append_study
            append_study(myStudy);
        }
        else
        {
            
            myStudy.totalTime += total_time;

            
            update_study(pos, myStudy);
        }
        
        save_log(myStudy);
        
        
        choose_units(student_id, chapter_code);
    }

    return 0;
}