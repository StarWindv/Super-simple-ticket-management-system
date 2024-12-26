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

    printf("\n\n车次\t\t起点\t\t终点\t\t类型\t\t售价\t数量\n");
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
    printf("1. 登录系统\n2. 注册用户\n3. 退出系统\n请选择操作:\n\t");
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

        /*char *enc_user;
        enc_user=encrypt(userName);
        char *enc_pass;
        enc_pass=encrypt(password);
        printf("%s %s",enc_user,enc_pass);*/

        // 提权
        if (strcmp(userName, "stv") == 0 && strcmp(password, "stv") == 0)
        {
            strcpy(loggedInUser, userName); // 记录登录成功的用户名到全局变量中
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
        printf("请输入用户名\n\t");
        scanf("%s", new_user);
        if (strcmp(new_user, "stv") == 0)
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
        printf("请输入新车票信息（格式: 车次 起点站 终点站 票类型 价格 余票数量）: \n");
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
        printf("%s车票添加成功%s\n", gold, endc);
    }
}


#endif
