#include "function.c"

int main() {
    const char *directories[] = {
        "resource",
        "resource/user_tickets"
    };
    size_t num_directories = sizeof(directories) / sizeof(directories[0]);

    const char *files[] = {
        "resource/tickets.txt",
        "resource/users.txt",
        "resource/admins.txt"
    };
    size_t num_files = sizeof(files) / sizeof(files[0]);

    const char *stv[] = {
        "resource/user_tickets/stv.txt"
    };
    size_t num_stv = sizeof(stv) / sizeof(stv[0]);

    for (size_t i = 0; i < num_directories; ++i)
    {
        check_dictionary(directories[i]);
    }

    for (size_t i = 0; i < num_files; ++i) {
        check_file(files[i]);
    }


    int choice;
    printf("%s\n\n��ӭʹ��Ʊ����Ϣ����ϵͳ%s\n",gold,endc);
    do
    {
        int log = login();

        if (log == 0)
        {
            rewind(stdin);
            printf("%s��ѡ����ȷ��ѡ�%s\n", red, endc);
            continue;
        }
        if (log == 2)
        {
            rewind(stdin);
            continue;
        }
        if (log == -1)
        {
            return 0;
        }
        int admin_point = 0;
        if (is_admin(loggedInUser))
        {
            admin_point = 1;
        }
        if (admin_point == 1)
        {
            for (size_t i = 0; i < num_stv; ++i)
            {
                check_file(stv[i]);
            }
            int exit_point1 = 0;
            printf("%s��ӭ���װ��Ĺ���Ա��\n\n%s", gold, endc);
            rewind(stdin);
            do
            {
            admin_lable:
                system("cls");
                printf("%s", loggedInUser);
                printf("\n\n\t Ʊ�����Աϵͳ\n*******************************");
                printf("\n1.���ӳ�Ʊ��Ϣ\n");
                printf("2.������ͨ�û�����\n");
                printf("3.�����û���Ϣ\n");
                printf("4.��ճ�Ʊ��Ϣ\n");
                printf("5.�˳���¼\n");
                printf("%s6.CodeGeeX4%s\n",gold,endc);
                printf("*******************************\n\n");
                printf("��ѡ�����: \n\t");
                if (scanf("%d", &choice) != 1) {
                    // ������������
                    while (getchar() != '\n') {
                        printf("%s\n\n��Ч�����룬������ѡ�������%s\n\n", red, endc);
                    }
                    continue;
                }
                switch(choice)
                {
                    case 1:
                        add_ticket();
                        break;
                    case 2:
                        system("cls");
                        goto normal_user_lable;
                    case 3:
                    {
                        FILE *file = fopen(files[1], "w");
                        if (file == NULL)
                        {
                            printf("%s�޷����û��ļ�%s\n", red, endc);
                        }
                        else
                        {
                            printf("%s�û���Ϣ������%s\n", gold, endc);
                            fputs("Admin,Admin\n",file);
                            fclose(file);
                        }
                    }
                    break;
                    case 4:
                    {
                        FILE *file = fopen(files[0], "w");
                        if (file == NULL)
                        {
                            printf("%s�޷���Ʊ���ļ�%s\n", red, endc);
                        }
                        else
                        {
                            printf("%s��Ʊ��Ϣ�����%s\n", gold, endc);
                            fclose(file);
                        }
                    }
                    break;
                    case 5:
                        exit_point1 = 1;
                        break;
                    default:
                        printf("��ѡ����ȷ��ѡ�\n");
                        break;
                }
            }while(exit_point1 == 0);

        }
        else
        {
            int exit_point1 = 0;
            do
            {
            normal_user_lable: // �ű�
                rewind(stdin);
                exit_point1 = 0;
                printf("\n\t  Ʊ�����ϵͳ\n*******************************");
                printf("\n1.��ѯƱ����Ϣ\n");
                printf("2.����Ʊ\n");
                printf("3.�˶���Ʊ\n");
                printf("4.�鿴���г�Ʊ\n");
                printf("5.�˳���¼\n");
                if (admin_point == 1)
                {
                    printf("%s6.���ع���Աҳ��\n%s",gold,endc );
                }
                printf("*******************************\n\n");
                printf("��ѡ�����: \n\t");

                if (scanf("%d", &choice) != 1)
                {
                    // ������������
                    while (getchar() != '\n')
                    {
                        printf("%s\n\n��Ч�����룬������ѡ�������%s\n\n", red, endc);

                    }
                }

                switch (choice)
                {
                    case 1:
                        system("cls");
                        queryTickets();
                        break;
                    case 2:
                        buyTicket();
                        break;
                    case 3:
                        system("cls");
                        returnTicket();
                        break;
                    case 4:
                        system("cls");
                        list_tickets();
                        break;
                    case 5:
                        printf("�˳�ϵͳ\n");
                        exit_point1 = 1;
                        break;
                    case 6:
                        if (is_admin(loggedInUser))
                        {
                            system("cls");
                            goto admin_lable;
                        }
                        else
                        {
                            printf("%s��Ч�����룬������ѡ��%s\n",red,endc);
                        }
                        break;
                    default:
                        printf("%s��Ч�����룬������ѡ�������%s\n", red, endc);
                        break;
                }
            }
            while (exit_point1 == 0);
        }
    }
    while (1);
}

//1970176603@qq.com ������ 13309441726 ѧ��2452234
//��ҹ����һҹд����
