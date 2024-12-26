### main.c

`main.c` 是项目中的主要控制文件，它负责处理用户交互、登录、注册以及管理员和普通用户的操作界面。以下是 `main.c` 中定义的一些关键功能：

1. **登录与注册**：
   
   - `login()`：处理用户的登录和注册操作。
   - `is_admin(const char *username)`：检查用户是否为管理员。

2. **管理员功能**：
   
   - `add_ticket()`：添加新的火车票信息。
   - 清空用户信息、票务信息等。

3. **普通用户功能**：
   
   - `queryTickets()`：查询火车票信息。
   - `buyTicket()`：购买火车票。
   - `returnTicket()`：退票。
   - `list_tickets()`：列出用户持有的所有票。

4. **文件和目录检查**：
   
   - `check_file(const char *file_path)`：检查并创建必要的文件。
   - `check_dictionary(const char *dir_path)`：检查并创建必要的目录。

5. **主循环**：
   
   - 控制整个程序的主循环，根据用户的输入调用不同的功能。

### main-AI.c

`main-AI.c` 文件在 `main.c` 的基础上集成了与 AI 服务器进行交互的功能。它通过 Python/C API 调用 `py_ollama.pyd` 模块中的 `ollama()` 函数，实现与 AI 的对话功能。以下是 `main-AI.c` 中定义的关键功能：

1. **初始化 Python 环境**：
   
   - `Py_Initialize()`：初始化 Python 解释器。
   - `Py_Finalize()`：关闭 Python 解释器。

2. **导入并调用 PyD 模块**：
   
   - 使用 `PyImport_Import()` 导入 `py_ollama.pyd` 模块。
   - 使用 `PyObject_GetAttrString()` 获取 `ollama()` 函数。
   - 使用 `PyObject_CallObject()` 调用 `ollama()` 函数。

3. **AI 对话**：
   
   - 通过 `ollama()` 函数实现与 AI 的对话，处理用户输入并获取 AI 的回复。

### 使用方法：

如果您的系统上没有安装ollama，或者是不想安装，您可以直接构建并运行main.c；如果您有ollama，并且有模型和完整的python3解释器，您可以在命令行cd到项目文件，然后输入以下代码：

```bash
gcc test.c -o test -I盘符:/Python/include -L盘符:/Python/libs -lpython313(请修改成你的python版本) -L项目地址 py_ollama.cp313-win_amd64.pyd
```

这将构建一个带有ai模块的可执行文件，如此一来您便可以使用AI对话功能。

### 总结

- `main.c` 是用户交互和主要业务逻辑的处理中心，负责登录、注册、购票、退票等操作。
- `main-AI.c` 则是在main.c的基础上增加了AI服务，通过调用 Python 模块实现 AI 对话功能。
