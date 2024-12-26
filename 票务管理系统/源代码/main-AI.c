#include "function.c"
#include <Python.h>

int ai();
int main() {
    Py_Initialize();
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
    printf("%s\n\n欢迎使用票务信息管理系统%s\n",gold,endc);
    do
    {
        int log = login();

        if (log == 0)
        {
            rewind(stdin);
            printf("%s请选择正确的选项！%s\n", red, endc);
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
            printf("%s欢迎！亲爱的管理员！\n\n%s", gold, endc);
            rewind(stdin);
            do
            {
            admin_lable:
                system("cls");
                // printf("%s", loggedInUser);
                printf("\n\n\t 票务管理员系统\n*******************************");
                printf("\n1.增加车票信息\n");
                printf("2.进入普通用户界面\n");
                printf("3.重置用户信息\n");
                printf("4.清空车票信息\n");
                printf("5.退出登录\n");
                printf("%s6.CodeGeeX4%s\n",gold,endc);
                printf("*******************************\n\n");
                printf("请选择操作: \n\t");
                if (scanf("%d", &choice) != 1) {
                    // 清除错误的输入
                    while (getchar() != '\n') {
                        printf("%s\n\n无效的输入，请重新选择操作。%s\n\n", red, endc);
                    }
                    continue;
                }
                switch(choice)
                {
                    case 1:
                        system("cls");
                        add_ticket();
                        break;
                    case 2:
                        system("cls");
                        goto normal_user_lable;
                    case 3:
                    {
                        system("cls");
                        FILE *file = fopen(files[1], "w");
                        if (file == NULL)
                        {
                            printf("%s无法打开用户文件%s\n", red, endc);
                        }
                        else
                        {
                            printf("%s用户信息已重置%s\n", gold, endc);
                            fputs("Admin,Admin\n",file);
                            fclose(file);
                        }
                    }
                    break;
                    case 4:
                    {
                        system("cls");
                        FILE *file = fopen(files[0], "w");
                        if (file == NULL)
                        {
                            printf("%s无法打开票务文件%s\n", red, endc);
                        }
                        else
                        {
                            printf("%s车票信息已清空%s\n", gold, endc);
                            fclose(file);
                        }
                    }
                    break;
                    case 5:
                        exit_point1 = 1;
                        break;
                    case 6:
                    {
                        system("cls");
                        ai();
                        break;
                    }
                    default:
                        printf("请选择正确的选项！\n");
                        break;
                }
            }while(exit_point1 == 0);

        }
        else
        {
            int exit_point1 = 0;
            do
            {
            normal_user_lable: // 信标
                rewind(stdin);
                exit_point1 = 0;
                printf("\n\t  票务管理系统\n*******************************");
                printf("\n1.查询票务信息\n");
                printf("2.购买车票\n");
                printf("3.退订车票\n");
                printf("4.查看已有车票\n");
                printf("5.退出登录\n");
                if (admin_point == 1)
                {
                    printf("%s6.返回管理员页面\n%s",gold,endc );
                }
                printf("*******************************\n\n");
                printf("请选择操作: \n\t");

                if (scanf("%d", &choice) != 1)
                {
                    // 清除错误的输入
                    while (getchar() != '\n')
                    {
                        printf("%s\n\n无效的输入，请重新选择操作。%s\n\n", red, endc);

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
                        printf("退出系统\n");
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
                            printf("%s无效的输入，请重新选择。%s\n",red,endc);
                        }
                        break;
                    default:
                        printf("%s无效的输入，请重新选择操作。%s\n", red, endc);
                        break;
                }
            }while (exit_point1 == 0);
        }
        Py_Finalize();
    }while (1);

}

int ai()
{
    // 初始化 Python 解释器
    // Py_Initialize();


    // 导入 pyollama.pyd 模块
    PyObject *pName = PyUnicode_DecodeFSDefault("py_ollama");
    PyObject *pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != NULL) {
        // 调用 Python 函数
        PyObject *pFunc = PyObject_GetAttrString(pModule, "ollama");

        if (PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_Pack(0); // 无参数
            PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
            Py_XDECREF(pArgs);
            Py_XDECREF(pValue);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }

    // 关闭 Python 解释器
    // Py_Finalize();

}
//starwindv@qq.com 徐瑞泽-2452234
//熬夜两天一夜写出来的
