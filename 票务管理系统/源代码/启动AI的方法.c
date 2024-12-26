#include <Python.h>
#include <stdio.h>
int ai() {
    // 初始化 Python 解释器
    Py_Initialize();


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
    Py_Finalize();
}

int main()
{

    ai();
}
