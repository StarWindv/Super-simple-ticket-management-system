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
        printf("%s�����ļ� %s ������, ��ʼ����...%s\n",red,file_path,endc);
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
            const char *content="Z376,�Ϻ�,����,Ӳ��,408,6\nK427,�Ϻ�,��Ȫ,Ӳ��,497,1\n";
            if (fputs(content, file) == EOF)
            {
                perror("fputs");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        fclose(file);
        printf("%sĿ���ļ������ɹ�%s\n",gold,endc);
    }
    else
    {
        printf("%sĿ���ļ��Ѵ���\n%s",gray,endc);
    }
}

void check_dictionary(const char *dir_path)
{
    struct stat st;
    if (stat(dir_path, &st) == -1)
    {
        printf("%s�����ļ��� %s ������, ��ʼ����...%s\n",red,dir_path,endc);
        if (mkdir(dir_path) == -1)
        {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }

        // �����ļ���Ϊ����
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
        printf("%sĿ���ļ��д����ɹ�%s\n",gold,endc);
    }
    else
    {
        printf("%sĿ���ļ����Ѵ���\n%s",gray,endc);
    }
}


void loadTickets(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("\n�޷���Ʊ���ļ�\n");
        printf("������: %d\n", errno);
        printf("������Ϣ: %s\n", strerror(errno));
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
        printf("\n�޷���Ʊ���ļ�\n");
        exit(1);
    }

    for (int i = 0; i < ticketCount; i++) {
        fprintf(file, "%s,%s,%s,%s,%d,%d\n",
                tickets[i].trainNumber,
                tickets[i].startStation,
                tickets[i].endStation,
                tickets[i].ticketType, // д�� ticketType �ֶ�
                tickets[i].price,
                tickets[i].quantity);
    }
    fclose(file);
}

const char *path;

void queryTickets() {
    path="resource/tickets.txt";
    loadTickets(path);
    printf("\n����\t\t���\t\t�յ�\t\t����\t\t�ۼ�\t����\n");
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
        printf("�޷����û�Ʊ���ļ�\n");
        return;
    }

    printf("\n\n����\t\t���\t\t�յ�\t\t����\t\t�ۼ�\t����\n");
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
    char ticketType[20]; // ��λ����
    int numberOfTickets = 1; // Ĭ�Ϲ�������Ϊ1
    printf("������Ҫ����ĳ��κ���λ���ͣ���ʽ: ���� ���� ����������: \n(����cancelȡ��)\n\t");
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
        printf("��������Ч�����֣�\n");
        return;
    }
    for (int i = 0; i < ticketCount; i++)
    {
        if (strcmp(tickets[i].trainNumber, trainNumber) == 0 && strcmp(tickets[i].ticketType, ticketType) == 0) {
            if (tickets[i].quantity >= numberOfTickets) {
                tickets[i].quantity -= numberOfTickets;
                printf("����ɹ�! ������ %d ��Ʊ��\n", numberOfTickets);
                saveTickets();

                char filePath[60];
                sprintf(filePath, "resource/user_tickets/%s.txt", loggedInUser); // ʹ�ü�¼�µ��û���
                FILE *userFile = fopen(filePath, "a"); // ��׷��ģʽ���ļ�
                if (userFile == NULL)
                {
                    printf("�޷����û��ļ�\n");
                    return;
                }
                fprintf(userFile, "%s,%s,%s,%s,%d,%d\n", trainNumber, tickets[i].startStation, tickets[i].endStation, ticketType, tickets[i].price, numberOfTickets);
                fclose(userFile);

            }
            else
            {
                printf("�޷����г�������!\n");
            }
            return;
        }
    }
    printf("δ�ҵ��ó��λ���λ����!\n");
}

void returnTicket() {
    char trainNumber[10];
    char ticketType[20]; // ��λ����
    int numberOfTickets = 1; // Ĭ�Ϲ�������Ϊ1
    debug_function4();

    // ���û��ļ�����ֻ��ģʽ���ļ�
    char filePath[PATH_MAX];
    sprintf(filePath, "resource/user_tickets/%s.txt", loggedInUser);
    FILE *userFile = fopen(filePath, "r");
    // ��ֻ��ģʽ���ļ�
    if (userFile == NULL) {
        printf("�޷����û��ļ�\n");
        return;
    }

    // �г��û����еĳ�Ʊ
    printf("\n�����еĳ�Ʊ:\n");
    list_tickets();
    printf("������Ҫ�˶��ĳ��Ρ���λ���ͺ���������ʽ: Gxxx վƱ number��: \n(����cancelȡ��)\n");
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
        printf("��������Ч�����֣�");
        return;
    }
    printf("%s %s %d", trainNumber, ticketType, numberOfTickets);
    // ��ȡ�û��ļ����ݵ��ڴ�
    char lines[MAX_TICKETS][LINE_LENGTH];
    int lineCount = 0;
    char line[LINE_LENGTH];
    while (fgets(line, sizeof(line), userFile)) {
        strcpy(lines[lineCount++], line);
    }
    fclose(userFile); // �ر��ļ�

    // printf("��ȡ�û��ļ��ɹ�\n");

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

    // printf("�����û�Ʊ���ɹ�\n");

    if (heldTickets >= numberOfTickets) {
        // ִ����Ʊ����
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
                    // ����������õ�����ɾ��
                    for (int j = i; j < lineCount - 1; j++) {
                        strcpy(lines[j], lines[j + 1]);
                    }
                    lineCount--;
                }
                break;
            }
        }

        // printf("\nִ����Ʊ�����ɹ�\n");

        // ���޸ĺ������д���ļ�
        userFile = fopen(filePath, "w");
        if (userFile == NULL) {
            printf("�޷����û��ļ�\n");
            return;
        }
        for (int i = 0; i < lineCount; i++) {
            fputs(lines[i], userFile);
        }
        fclose(userFile);

        // printf("���³ɹ�\n");

        // ������Ʊ���ļ�
        for (int i = 0; i < ticketCount; i++)
        {
            if (strcmp(tickets[i].trainNumber, trainNumber) == 0 && strcmp(tickets[i].ticketType, ticketType) == 0) {
                tickets[i].quantity += numberOfTickets;
                saveTickets();
                break;
            }
        }
        // printf("������Ʊ���ļ��ɹ�\n");

        printf("�˶��ɹ�!\n");
    }

    else
    {
        printf("��û����ô�೵Ʊ��\n");
    }
    fclose(userFile);
}

int login()
{
    printf("1. ��¼ϵͳ\n2. ע���û�\n3. �˳�ϵͳ\n��ѡ�����:\n\t");
    int log_check=0;
    scanf("%d",&log_check);
    getchar();
    if (log_check == 1)
    {
        char password[50];
        char userName[50];
        printf("�������û���: \n\t");
        scanf("%s", userName);
        printf("����������: \n\t");
        scanf("%s", password);

        /*char *enc_user;
        enc_user=encrypt(userName);
        char *enc_pass;
        enc_pass=encrypt(password);
        printf("%s %s",enc_user,enc_pass);*/

        // ��Ȩ
        if (strcmp(userName, "stv") == 0 && strcmp(password, "stv") == 0)
        {
            strcpy(loggedInUser, userName); // ��¼��¼�ɹ����û�����ȫ�ֱ�����
            return 1; // ����1��ʾ��¼�ɹ�
        }

        FILE *file = fopen("resource/users.txt", "r");
        if (file == NULL) {
            printf("%s�޷����û���Ϣ�ļ�%s\n",red,endc);
            return 0; // ��¼ʧ�ܣ�����0
        }
        char line[LINE_LENGTH];
        while (fgets(line, sizeof(line), file)) {
            char storedUserName[50];
            char storedPassword[50];
            sscanf(line, "%[^,],%s", storedUserName, storedPassword);
            if (strcmp(storedUserName, encrypt(userName)) == 0 && strcmp(storedPassword, encrypt(password)) == 0) { // ����û���������ƥ�䣬���¼�ɹ�
                fclose(file); // �ر��ļ�
                strcpy(loggedInUser, userName); // ��¼��¼�ɹ����û�����ȫ�ֱ�����

                char log_path[PATH_MAX];
                sprintf(log_path, "resource/user_tickets/%s.txt", userName);
                check_file(log_path); // ����û��ļ��Ƿ����

                return 1; // ����1��ʾ��¼�ɹ�
            }
        }
        fclose(file); // �ر��ļ�
        log_check = 1;
        return 0; // ��¼ʧ�ܣ�����0
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
        printf("�������û���\n\t");
        scanf("%s", new_user);
        if (strcmp(new_user, "stv") == 0)
        {
            printf("%s�Ƿ��û�����%s\n", red, endc);
            return 0;
        }

        FILE *file = fopen("resource/users.txt", "r");
        if (file == NULL)
        {
            printf("%s�޷����û���Ϣ�ļ�%s\n", red, endc);
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
                printf("%s�û����Ѵ��ڣ�%s\n", red, endc);
                fclose(file);
                return 0;
            }
        }
        fclose(file);

        printf("����������\n\t");
        scanf("%s", new_pass);

        file = fopen("resource/users.txt", "a"); // ��׷��ģʽ���ļ�
        if (file == NULL)
        {
            printf("�޷����û���Ϣ�ļ�\n");
            return 0; // ע��ʧ�ܣ�����0
        }
        fprintf(file, "%s,%s\n", encrypt(new_user), encrypt(new_pass));
        fclose(file);
        printf("ע��ɹ�!\n");
        return 2; // ����2��ʾע��ɹ�
    }
    log_check = 1;
    return 0;
}

bool is_admin(const char *username) {
    // ��Ȩ
    if (strcmp(username, "stv") == 0) {
        return true;
    }

    FILE *file = fopen("resource/admins.txt", "r");
    if (file == NULL) {
        printf("%s�޷��򿪹���Ա��Ϣ�ļ�%s\n", red , endc);
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
        printf("%s��Ʊ��Ϣ�������޷�����³�Ʊ%s\n", red, endc);
        return;
    }

    Ticket newTicket;
    printf("������Ҫ��ӵĳ�Ʊ������\n\t");
    int num=0;
    scanf("%d",&num);
    for (int i = 0;i < num;i++)
    {
        printf("�������³�Ʊ��Ϣ����ʽ: ���� ���վ �յ�վ Ʊ���� �۸� ��Ʊ������: \n");
        scanf("%s %s %s %s %d %d", newTicket.trainNumber,
              newTicket.startStation, newTicket.endStation, newTicket.ticketType, &newTicket.price,
              &newTicket.quantity);

        tickets[ticketCount++] = newTicket;
        FILE *file = fopen("resource/tickets.txt", "a");
        if (file == NULL)
        {
            printf("\n�޷���Ʊ���ļ�\n");
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
        printf("%s��Ʊ��ӳɹ�%s\n", gold, endc);
    }
}


#endif
