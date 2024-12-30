#ifndef stv_function
#define stv_function

#include "head.h"
#include "encrypt.c"

typedef struct {
    char trainNumber[10];
    char startStation[20];
    char endStation[20];
    char ticketType[20];
    int price;
    int quantity;
} Ticket;

Ticket tickets[MAX_TICKETS];
int ticketCount = 0;
char loggedInUser[50];
const char *path;



int system_clear()
{
#if defined(_WIN32) || defined(_WIN64)  // Windows 系统
    system("cls");  // 使用 cls 清屏
#else  // 类 Unix 系统（Linux/macOS等）
    system("clear");  // 使用 clear 清屏
#endif
    return 0;  // 清屏成功
}
#endif


void check_file(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("%s必需文件 %s 不存在, 开始创建...%s\n",red,file_path,endc);
        file = fopen(file_path, "w");
        if (file == NULL)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        else if (strcmp(file_path, "resource/users.txt") == 0)
        {
            const char *content="Nqzva,Nqzva\n";
            if (fputs(content, file) == EOF)
            {
                perror("fputs");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        else if (strcmp(file_path, "resource/admins.txt") == 0)
        {
            const char *content="Nqzva\n";
            if (fputs(content, file) == EOF)
            {
                perror("fputs");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        else if (strcmp(file_path, "resource/tickets.txt") == 0)
        {
            const char *content="Z376,上海,兰州,硬卧,408,6\nK427,上海,酒泉,硬卧,497,1\n";
            if (fputs(content, file) == EOF)
            {
                perror("fputs");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        fclose(file);
        printf("%s目标文件创建成功%s\n",gold,endc);
    }
    else
    {
        printf("%s目标文件已存在\n%s",gray,endc);
    }
}

void check_dictionary(const char *dir_path)
{
    struct stat st;
    if (stat(dir_path, &st) == -1)
    {
        printf("%s必需文件夹 %s 不存在, 开始创建...%s\n",red,dir_path,endc);
        if (mkdir(dir_path) == -1)
        {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }

        // 设置文件夹为隐藏
        DWORD attributes = GetFileAttributes(dir_path);
        if (attributes == INVALID_FILE_ATTRIBUTES)
        {
            perror("GetFileAttributes");
            exit(EXIT_FAILURE);
        }
        if (!SetFileAttributes(dir_path, attributes | FILE_ATTRIBUTE_HIDDEN))
        {
            perror("SetFileAttributes");
            exit(EXIT_FAILURE);
        }
        printf("%s目标文件夹创建成功%s\n",gold,endc);
    }
    else
    {
        printf("%s目标文件夹已存在\n%s",gray,endc);
    }
}


void loadTickets(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("\n无法打开票务文件\n");
        printf("错误码: %d\n", errno);
        printf("错误信息: %s\n", strerror(errno));
        printf("%s",path);
    }

    char line[LINE_LENGTH];
    ticketCount = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%d",
               tickets[ticketCount].trainNumber,
               tickets[ticketCount].startStation,
               tickets[ticketCount].endStation,
               tickets[ticketCount].ticketType,
               &tickets[ticketCount].price,
               &tickets[ticketCount].quantity);
        ticketCount++;
    }
    fclose(file);
}

void saveTickets() {
    FILE *file = fopen("resource/tickets.txt", "w");
    if (file == NULL) {
        printf("\n无法打开票务文件\n");
        exit(1);
    }

    for (int i = 0; i < ticketCount; i++) {
        fprintf(file, "%s,%s,%s,%s,%d,%d\n",
                tickets[i].trainNumber,
                tickets[i].startStation,
                tickets[i].endStation,
                tickets[i].ticketType, // 写入 ticketType 字段
                tickets[i].price,
                tickets[i].quantity);
    }
    fclose(file);
}

const char *path;

void queryTickets() {
    path="resource/tickets.txt";
    loadTickets(path);
    printf("\n车次\t\t起点\t\t终点\t\t类型\t\t售价\t余量\n");
    for (int i = 0; i < ticketCount; i++) {
        printf("%-8s\t%-12s\t%-12s\t%-8s\t%-5d\t%-5d\n",
               tickets[i].trainNumber,
               tickets[i].startStation,
               tickets[i].endStation,
               tickets[i].ticketType,
               tickets[i].price,
               tickets[i].quantity);
    }
}

void debug_function4() {
    path="resource/tickets.txt";
    loadTickets(path);
}

void list_tickets() {
    char path[100];
    sprintf(path, "resource/user_tickets/%s.txt", loggedInUser);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("无法打开用户票务文件\n");
        return;
    }

    printf("\n车次\t\t起点\t\t终点\t\t类型\t\t售价\t数量\n");
    char line[LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char trainNumber[10];
        char startStation[20];
        char endStation[20];
        char ticketType[20];
        int price;
        int quantity;
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%d", trainNumber, startStation, endStation, ticketType, &price, &quantity);
        printf("%-8s\t%-12s\t%-12s\t%-8s\t%-5d\t%-5d\n", trainNumber, startStation, endStation, ticketType, price, quantity);
    }
    fclose(file);
}

void buyTicket() {
    char trainNumber[10];
    char ticketType[20]; // 座位类型
    int numberOfTickets = 1; // 默认购买数量为1
    printf("请输入要购买的车次和座位类型（格式: 车次 软卧 购买数量）: \n(输入cancel取消)\n\t");
    scanf("%s",trainNumber);
    if (strcmp(trainNumber, "cancel") == 0) {
        return;
    }
    scanf("%s", ticketType);
    if (strcmp(ticketType, "cancel") == 0) {
        return;
    }
    scanf("%d", &numberOfTickets);
    if (strcmp(ticketType, "cancel") == 0) {
        system_clear();
        return;
    }
    if (numberOfTickets <= 0)
    {
        printf("请输入有效的数字！\n");
        return;
    }
    for (int i = 0; i < ticketCount; i++)
    {
        if (strcmp(tickets[i].trainNumber, trainNumber) == 0 && strcmp(tickets[i].ticketType, ticketType) == 0) {
            if (tickets[i].quantity >= numberOfTickets) {
                tickets[i].quantity -= numberOfTickets;
                printf("购买成功! 共购买 %d 张票。\n", numberOfTickets);
                saveTickets();

                char filePath[60];
                sprintf(filePath, "resource/user_tickets/%s.txt", loggedInUser); // 使用记录下的用户名
                FILE *userFile = fopen(filePath, "a"); // 以追加模式打开文件
                if (userFile == NULL)
                {
                    printf("无法打开用户文件\n");
                    return;
                }
                fprintf(userFile, "%s,%s,%s,%s,%d,%d\n", trainNumber, tickets[i].startStation, tickets[i].endStation, ticketType, tickets[i].price, numberOfTickets);
                fclose(userFile);

            }
            else
            {
                printf("无法进行超量购买!\n");
            }
            return;
        }
    }
    printf("未找到该车次或座位类型!\n");
}


void returnTicket() {
    char trainNumber[10];
    char ticketType[20]; // 座位类型
    int numberOfTickets = 1; // 默认购买数量为1
    debug_function4();

    // 打开用户文件，以只读模式打开文件
    char filePath[PATH_MAX];
    sprintf(filePath, "resource/user_tickets/%s.txt", loggedInUser);
    FILE *userFile = fopen(filePath, "r");
    // 以只读模式打开文件
    if (userFile == NULL) {
        printf("无法打开用户文件\n");
        return;
    }

    // 列出用户持有的车票
    printf("\n您持有的车票:\n");
    list_tickets();
    printf("请输入要退订的车次、座位类型和数量（格式: Gxxx 站票 number）: \n(输入cancel取消)\n");
    scanf("%s",trainNumber);
    if (strcmp(trainNumber, "cancel") == 0) {
        return;
    }
    scanf("%s", ticketType);
    if (strcmp(ticketType, "cancel") == 0) {
        return;
    }
    scanf("%d", &numberOfTickets);

    if (numberOfTickets <= 0) {
        printf("请输入有效的数字！");
        return;
    }
    printf("%s %s %d", trainNumber, ticketType, numberOfTickets);
    // 读取用户文件内容到内存
    char lines[MAX_TICKETS][LINE_LENGTH];
    int lineCount = 0;
    char line[LINE_LENGTH];
    while (fgets(line, sizeof(line), userFile)) {
        strcpy(lines[lineCount++], line);
    }
    fclose(userFile); // 关闭文件

    // printf("读取用户文件成功\n");

    int heldTickets = 0;
    for (int i = 0; i < lineCount; i++) {
        char tempTrainNumber[10];
        char tempstartStation[20];
        char tempendStation[20];
        char tempTicketType[20];
        int tempprice;
        int tempQuantity;
        sscanf(lines[i], "%[^,],%[^,],%[^,],%[^,],%d,%d", tempTrainNumber, tempstartStation,
               tempendStation, tempTicketType, &tempprice,&tempQuantity);
        if (strcmp(tempTrainNumber, trainNumber) == 0 && strcmp(tempTicketType, ticketType) == 0) {
            heldTickets += tempQuantity;
        }
    }

    // printf("计算用户票量成功\n");

    if (heldTickets >= numberOfTickets) {
        // 执行退票操作
        for (int i = 0; i < lineCount; i++) {
            char tempTrainNumber[10];
            char tempstartStation[20];
            char tempendStation[20];
            char tempTicketType[20];
            int tempprice;
            int tempQuantity;
            sscanf(lines[i], "%[^,],%[^,],%[^,],%[^,],%d,%d", tempTrainNumber, tempstartStation,
                   tempendStation, tempTicketType, &tempprice,&tempQuantity);
            if (strcmp(tempTrainNumber, trainNumber) == 0 && strcmp(tempTicketType, ticketType) == 0) {
                if (tempQuantity > numberOfTickets) {
                    sprintf(lines[i], "%s,%s,%s,%s,%d,%d\n", tempTrainNumber, tempstartStation, tempendStation, tempTicketType, tempprice, tempQuantity - numberOfTickets);
                } else {
                    // 如果数量正好等于则删除
                    for (int j = i; j < lineCount - 1; j++) {
                        strcpy(lines[j], lines[j + 1]);
                    }
                    lineCount--;
                }
                break;
            }
        }

        // printf("\n执行退票操作成功\n");

        // 将修改后的数据写回文件
        userFile = fopen(filePath, "w");
        if (userFile == NULL) {
            printf("无法打开用户文件\n");
            return;
        }
        for (int i = 0; i < lineCount; i++) {
            fputs(lines[i], userFile);
        }
        fclose(userFile);

        // printf("更新成功\n");

        // 更新总票务文件
        for (int i = 0; i < ticketCount; i++)
        {
            if (strcmp(tickets[i].trainNumber, trainNumber) == 0 && strcmp(tickets[i].ticketType, ticketType) == 0) {
                tickets[i].quantity += numberOfTickets;
                saveTickets();
                break;
            }
        }
        // printf("更新总票务文件成功\n");

        printf("退订成功!\n");
    }

    else
    {
        printf("您没有这么多车票！\n");
    }
    fclose(userFile);
}


int login()
{
    printf("%s\n\n  欢迎使用票务信息管理系统%s\n%s————————————————————————————%s  \n",gold,endc,gray,endc);
    printf("1.        登录系统\n2.        注册用户\n3.        退出系统\n\n请选择操作:\n\t");
    int log_check=0;
    scanf("%d",&log_check);
    getchar();
    if (log_check == 1)
    {
        char password[50];
        char userName[50];
        printf("请输入用户名: \n\t");
        scanf("%s", userName);
        printf("请输入密码: \n\t");
        scanf("%s", password);


        // 提权
        if (strcmp(userName, "stv") == 0 && strcmp(password, "stv") == 0)
        {
            strcpy(loggedInUser, userName); // 记录登录成功的用户名到全局变量中//string copy
            return 1; // 返回1表示登录成功
        }

        FILE *file = fopen("resource/users.txt", "r");
        if (file == NULL) {
            printf("%s无法打开用户信息文件%s\n",red,endc);
            return 0; // 登录失败，返回0
        }

        char line[LINE_LENGTH];

        while (fgets(line, sizeof(line), file)) {
            char storedUserName[50];
            char storedPassword[50];
            sscanf(line, "%[^,],%s", storedUserName, storedPassword);
            if (strcmp(storedUserName, encrypt(userName)) == 0 && strcmp(storedPassword, encrypt(password)) == 0) { // 如果用户名和密码匹配，则登录成功
                fclose(file); // 关闭文件
                strcpy(loggedInUser, userName); // 记录登录成功的用户名到全局变量中

                char log_path[PATH_MAX];
                sprintf(log_path, "resource/user_tickets/%s.txt", userName);
                check_file(log_path); // 检查用户文件是否存在

                return 1; // 返回1表示登录成功
            }
        }
        fclose(file); // 关闭文件
        log_check = 1;
        return 0; // 登录失败，返回0
    }
    if (log_check == 3)
    {
        log_check = 1;
        return -1;
    }
    if (log_check == 2)
    {
        char new_user[50];
        char new_pass[50];
        printf("请输入用户名(仅允许字母与数字)\n\t");
        scanf("%s", new_user);
        if (strcmp(new_user, "stv") == 0 | alpha_num_check(new_user) == 0)
        {
            printf("%s非法用户名！%s\n", red, endc);
            return 0;
        }

        FILE *file = fopen("resource/users.txt", "r");
        if (file == NULL)
        {
            printf("%s无法打开用户信息文件%s\n", red, endc);
            return 0;
        }

        char line[LINE_LENGTH];
        while (fgets(line, sizeof(line), file))
        {
            char storedUserName[50];
            char storedPassword[50];
            sscanf(line, "%[^,],%s", storedUserName, storedPassword);
            if (strcmp(storedUserName, encrypt(new_user)) == 0)
            {
                printf("%s用户名已存在！%s\n", red, endc);
                fclose(file);
                return 0;
            }
        }
        fclose(file);

        printf("请输入密码\n\t");
        scanf("%s", new_pass);

        file = fopen("resource/users.txt", "a"); // 以追加模式打开文件
        if (file == NULL)
        {
            printf("无法打开用户信息文件\n");
            return 0; // 注册失败，返回0
        }
        fprintf(file, "%s,%s\n", encrypt(new_user), encrypt(new_pass));
        fclose(file);
        printf("注册成功!\n");
        return 2; // 返回2表示注册成功
    }
    log_check = 1;
    return 0;
}

bool is_admin(const char *username) {
    // 提权
    if (strcmp(username, "stv") == 0) {
        return true;
    }

    FILE *file = fopen("resource/admins.txt", "r");
    if (file == NULL) {
        printf("%s无法打开管理员信息文件%s\n", red , endc);
        return false;
    }

    char adminName[50];
    while (fscanf(file, "%s", adminName) != EOF) {
        if (strcmp(adminName, encrypt(username)) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

void add_ticket() {
    if (ticketCount >= MAX_TICKETS) {
        printf("%s车票信息已满，无法添加新车票%s\n", red, endc);
        return;
    }

    Ticket newTicket;
    printf("请输入要添加的车票数量：\n\t");
    int num=0;
    scanf("%d",&num);
    for (int i = 0;i < num;i++)
    {
        printf("请输入新车票信息,格式: \n车次 起点站 终点站 票类型 价格 余票数量: \n");
        scanf("%s %s %s %s %d %d", newTicket.trainNumber,
              newTicket.startStation, newTicket.endStation, newTicket.ticketType, &newTicket.price,
              &newTicket.quantity);

        tickets[ticketCount++] = newTicket;
        FILE *file = fopen("resource/tickets.txt", "a");
        if (file == NULL)
        {
            printf("\n无法打开票务文件\n");
            exit(1);
        }

        fprintf(file, "%s,%s,%s,%s,%d,%d\n",
                newTicket.trainNumber,
                newTicket.startStation,
                newTicket.endStation,
                newTicket.ticketType,
                newTicket.price,
                newTicket.quantity);
        fclose(file);
        system_clear();
        printf("%s车票添加成功%s\n", gold, endc);
    }
}

void delete_user(const char *file_path, const char *username);

// 输出用户名单的函数
void list_user(const char* filename, int n )
{
    system_clear();
    do{
        printf("%s用户列表如下:%s", gold, endc);
        FILE* file = fopen(filename, "r"); // 打开文件
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }

        char line[256]; // 存储每一行的字符
        while (fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = '\0';

            // 查找是否包含逗号（用于区分格式1和格式2）
            char* comma_pos = strchr(line, ',');
            if (comma_pos != NULL)
            {
                *comma_pos = '\0'; // 将 ',' 位置替换为 '\0'，使得用户名和密码分开
                char* encrypted_username = line; // 用户名部分
                char* decrypted_username = decrypt(encrypted_username); // 解密用户名
                if (decrypted_username != NULL)
                {
                    printf("\n%s\n", decrypted_username); // 输出解密后的用户名
                    free(decrypted_username); // 释放解密后的内存
                }
            }

            else
            {
                // 如果没有逗号，则说明是格式2，只包含用户名
                char* encrypted_username = line;
                char* decrypted_username = decrypt(encrypted_username); // 解密用户名
                if (decrypted_username != NULL)
                {
                    printf("\n%s", decrypted_username); // 输出解密后的用户名
                    free(decrypted_username); // 释放解密后的内存
                }
            }
        }
        fclose(file);
        if(strcmp(loggedInUser,"stv")==0 && n == 1)
        {
            int choice;
            printf("\n\n请选择操作：\n");
            printf("1、删除用户\n");
            printf("2、返回上一级\n\t");

            if(scanf("%d", &choice)!=1)
            {
                while (getchar() != '\n')
                {
                    system_clear();
                    printf("%s无效的输入，请重新选择。%s\n", red, endc);
                    continue;
                }
            }

            if (choice == 1)
            {
                char username[50];
                printf("请输入要删除的用户名: \n\t");
                scanf("%s", username);
                delete_user(filename, username);
            }

            else if (choice == 2)
            {
                system_clear();
                return;
            }
        }
        else{
            return;
        }
    }while(1);
}


////////////////////////////////////////////////
void delete_user(const char *file_path, const char *username)
{
    // 对用户名进行加密
    char *encrypted_username = encrypt(username);
    if (encrypted_username == NULL) {
        printf("加密失败\n");
        return;
    }

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", file_path);
        free(encrypted_username);
        return;
    }

    char lines[LINE_LENGTH][LINE_LENGTH];
    int line_count = 0;
    char line[LINE_LENGTH];
    int found = 0;  // 用于标记是否找到用户名

    // 逐行读取文件内容
    while (fgets(line, sizeof(line), file))
    {
        // 提取行中的第一个用户名（直到逗号或换行符）
        char first_word[50];
        sscanf(line, "%49[^,\n]", first_word);

        // 如果该行的用户名与加密后的用户名不匹配，则保留该行
        if (strcmp(first_word, encrypted_username) != 0)
        {
            strcpy(lines[line_count++], line);
        }
        else {
            found = 1;  // 找到用户名
        }
    }

    fclose(file);
    free(encrypted_username);  // 释放

    // 如果没有找到用户名，直接返回
    if (!found)
    {
        system_clear();
        printf("%s用户 %s 不存在%s\n", red, username, endc);
        return;
    }

    // 将更新后的内容写回文件
    file = fopen(file_path, "w");
    if (file == NULL) {
        printf("无法重新打开文件 %s\n", file_path);
        return;
    }

    for (int i = 0; i < line_count; i++)
    {
        fputs(lines[i], file);
    }

    fclose(file);
    system_clear();
    printf("%s用户 %s 删除成功%s\n", red, username, endc);
}


void manage_admins(const char *filename)
{
    list_user(filename, 0);
    int choice;
    printf("\n\n\n请选择操作:\n1、添加管理员用户\n");
    printf("2、删除管理员用户\n");
    printf("3、返回上一级\n\t");

    if(scanf("%d", &choice)!=1)
    {
        while (getchar() != '\n')
        {
            system_clear();
            printf("%s无效的输入，请重新选择。%s\n", red, endc);
            return;
        }
    }
    switch (choice)
    {
        case 1:
        {
                char new_admin[50];
                printf("请输入要添加的管理员用户名: ");
                scanf("%s", new_admin);
                FILE *file = fopen(filename, "a");
                if (file == NULL)
                {
                    printf("无法打开管理员文件\n");
                    return;
                }
                char* encrypted_admin = encrypt(new_admin);
                fprintf(file, "%s\n", encrypted_admin);  // 添加管理员
                fclose(file);
                system_clear();
                printf("%s管理员 %s 添加成功%s\n", gold, new_admin, endc);
                break;
        }
        case 2:
        {
            char admin_to_remove[50];
            printf("请输入要删除的管理员用户名: ");
            scanf("%s", admin_to_remove);
            delete_user(filename, admin_to_remove);
            break;
        }
        case 3:
            system_clear();
            return;
    }
}
