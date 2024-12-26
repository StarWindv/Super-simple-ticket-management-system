# 使用 Cython 编写一个模块

## 前提条件

- 安装了 Cython。
- 熟悉基本的 Python 和 C 语言编程。

## 步骤

1. **创建 `setup.py` 文件**：用于定义编译设置，包括模块的名称、源文件等。

```python
from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("sample.pyx")
)
```

2. **编写 `.pyx` 文件**：这是包含 Cython 代码的地方。例如：

```cython
def add(int x, int y):
    return x + y
```

3. **编译模块**：
   在命令行中运行以下命令：

```bash
python setup.py build_ext --inplace
```

   这将生成一个扩展模块（通常是 `.so` 文件），可以在 Python 中导入。

4. **在 Python 中使用模块**：

```python
import sample

result = sample.add(1, 2)
print(result)  # 输出：3
```

## 说明

- Cython 代码通过注释和类型声明的方式，允许在 Python 中编写高效的 C/C++ 扩展。
- `setup.py` 文件中，`cythonize` 函数用于将 `.pyx` 文件转换为扩展模块。

## 注意事项

- 在编写 Cython 模块时，应尽量减少全局变量的使用，以避免潜在的线程安全问题。
- 对于性能敏感的部分，可以使用类型注释来优化代码执行效率。
- 编译过程中可能会遇到一些问题，如缺少依赖项或编译错误，需要根据具体情况进行调试和解决。

通过这些步骤，你可以开始使用 Cython 来编写高效的 Python 扩展模块。
