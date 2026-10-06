# Calculator Backend

一个使用 **C++17** 编写的轻量级计算器后端，为前后端分离的计算器系统提供表达式求值和计算历史管理功能。

项目使用 `cpp-httplib` 提供 HTTP 服务，使用 `nlohmann/json` 处理 JSON，并通过 SQLite 持久化计算历史。第三方依赖已经包含在仓库中，无需额外下载包管理器依赖。

## 功能特性

- 支持 `+`、`-`、`*`、`/` 四则运算
- 支持小数、多层括号以及一元正负号
- 检测非法字符、错误数字格式、语法错误和括号不匹配
- 检测除零错误
- 提供 HTTP JSON API
- 使用 SQLite 保存、查询和删除计算历史
- 支持浏览器跨域请求（CORS）

## 处理流程

```text
HTTP Request
     ↓
JSON Validation
     ↓
Tokenizer
     ↓
Expression Parser
     ↓
Infix → Postfix
     ↓
Calculator
     ↓
SQLite History
     ↓
HTTP JSON Response
```

表达式首先被拆分为 Token，随后经过语法校验和一元运算符识别。解析器使用调度场算法将中缀表达式转换为后缀表达式，计算器再通过栈完成求值。只有成功计算的表达式才会写入数据库。

## 项目结构

```text
832401105_calculator_backend/
│
├── CMakeLists.txt             # CMake 构建配置
├── README.md                  # 后端项目说明
├── codestyle.md               # C++ 代码规范
├── .gitignore
│
├── src/
│   │
│   ├── main.cpp               # 程序入口、HTTP 路由和 CORS
│   │
│   ├── api/
│   │   ├── ApiServer.h         # 预留的 API 层文件
│   │   └── ApiServer.cpp
│   │
│   ├── parser/
│   │   ├── Token.h            # Token 类型及数据结构
│   │   ├── Tokenizer.h
│   │   ├── Tokenizer.cpp      # 字符串 → Token
│   │   ├── ExpressionParser.h
│   │   └── ExpressionParser.cpp
│   │                          # 语法检查、一元运算符识别、
│   │                          # 中缀表达式 → 后缀表达式
│   │
│   ├── calculator/
│   │   ├── Calculator.h
│   │   └── Calculator.cpp     # 后缀表达式求值
│   │
│   ├── database/
│   │   ├── Database.h
│   │   └── Database.cpp       # SQLite 数据库访问
│   │
│   ├── common/
│   │   ├── Error.h            # 预留的统一错误类型
│   │   └── Result.h           # 预留的统一结果类型
│   │
│   └── test/
│       ├── tokenizer_test.cpp
│       ├── parser_test.cpp
│       ├── calculator_test.cpp
│       └── database_test.cpp
│
├── data/
│   └── calculator.db          # SQLite 数据库文件（运行时生成）
│
├── docs/
│   └── 后端设计文档_v1.md
│
└── third_party/
    ├── httplib.h              # cpp-httplib
    ├── json.hpp               # nlohmann/json
    └── sqlite/
        ├── sqlite3.c
        └── sqlite3.h
```

> `calculator.db` 不会提交到 Git。首次运行前需要确保项目根目录下存在 `data/` 目录。

## 技术栈

| 类别 | 技术 |
| --- | --- |
| 编程语言 | C++17、C |
| 构建系统 | CMake 3.16+ |
| HTTP 服务 | cpp-httplib 0.58.0 |
| JSON | nlohmann/json 3.12.0 |
| 数据库 | SQLite 3.53.4 |
| 核心算法 | 状态机、调度场算法、栈求值 |

## 构建与运行

### 环境要求

- 支持 C++17 的编译器，例如 GCC、Clang 或 MSVC
- CMake 3.16 或更高版本
- 可选：Ninja

HTTP、JSON 和 SQLite 源码均已包含在项目中，不需要额外安装这些依赖。

### 1. 获取项目

```bash
git clone https://github.com/EthanChen251/832401105_calculator_backend.git
cd 832401105_calculator_backend
```

### 2. 创建数据目录

```bash
cmake -E make_directory data
```

### 3. 配置并构建

使用当前系统的默认 CMake 生成器：

```bash
cmake -S . -B build
cmake --build build
```

如果已经安装 Ninja，也可以使用：

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

### 4. 启动服务

请在项目根目录下运行程序，以保证相对数据库路径 `data/calculator.db` 正确。

Windows：

```powershell
.\build\calculator_backend.exe
```

Linux / macOS：

```bash
./build/calculator_backend
```

启动成功后，服务监听：

```text
http://127.0.0.1:8080
```

## API 文档

### 计算表达式

```http
POST /api/calculate
Content-Type: application/json
```

请求示例：

```json
{
  "expression": "(1+2)*3"
}
```

成功响应：

```json
{
  "success": true,
  "result": 9.0
}
```

错误响应示例：

```json
{
  "success": false,
  "error": "Cannot divide by zero!"
}
```

curl 示例：

```bash
curl -X POST http://127.0.0.1:8080/api/calculate \
  -H "Content-Type: application/json" \
  -d '{"expression":"(1+2)*3"}'
```

### 查询历史记录

```http
GET /api/history
```

响应示例：

```json
[
  {
    "id": 1,
    "expression": "(1+2)*3",
    "result": 9.0,
    "created_at": "2026-10-04 10:30:00"
  }
]
```

### 删除历史记录

```http
DELETE /api/history/{id}
```

例如：

```bash
curl -X DELETE http://127.0.0.1:8080/api/history/1
```

成功响应：

```json
{
  "success": true,
  "deleted_id": 1
}
```

## 支持的表达式

| 表达式 | 说明 |
| --- | --- |
| `1 + 2` | 加法 |
| `10 - 3 * 2` | 运算符优先级 |
| `(1 + 2) * 3` | 括号 |
| `12.5 / 2` | 小数 |
| `-5 + 8` | 一元负号 |
| `3 * -2` | 运算符后的负数 |
| `--5` | 连续一元运算符 |

当前版本不支持幂运算、数学函数、科学计数法以及 `2(3+4)` 形式的隐式乘法。

## 数据库

程序首次启动时会自动创建 `calculation_history` 表：

```sql
CREATE TABLE IF NOT EXISTS calculation_history (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    expression TEXT NOT NULL,
    result REAL NOT NULL,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);
```

历史记录按 ID 倒序返回。

## 测试说明

`src/test/` 中包含分词器、解析器、计算器和数据库的开发测试程序。目前 CMake 默认只构建 `database_test`，这些程序属于手动验证工具，尚未接入自动化测试框架或 CTest。

> `database_test` 会操作 `data/calculator.db`，运行前请注意备份需要保留的历史数据。

## 相关文档

- [后端设计文档](docs/后端设计文档_v1.md)
- [代码规范](codestyle.md)

## License

本项目当前未声明开源许可证。如需复用或分发代码，请先联系项目作者。
