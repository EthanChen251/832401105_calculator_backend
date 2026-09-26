832401105_calculator_backend/
│
├── CMakeLists.txt             # CMake 构建配置
├── README.md                  # 后端项目说明
├── codestyle.md               # C++ 代码规范
├── .gitignore
│
├── src/
│   │
│   ├── main.cpp               # 后端程序入口
│   │
│   ├── api/
│   │   ├── ApiServer.h
│   │   └── ApiServer.cpp      # HTTP API 与路由处理
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
│   └── common/
│       ├── Error.h            # 统一错误类型
│       └── Result.h           # 统一处理结果类型
│
├── tests/
│   ├── parser_test.cpp
│   ├── calculator_test.cpp
│   └── database_test.cpp
│
├── data/
│   └── calculator.db          # SQLite 数据库文件（运行时生成）
│
├── docs/
│   ├── 后端设计文档_v1.md
│   └── 计算模块设计_v1.md
│
└── third_party/
    ├── httplib.h              # cpp-httplib
    └── json.hpp               # nlohmann/json