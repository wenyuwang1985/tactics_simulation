# 项目编码规范（Claude 必须严格遵守）

## 核心编码原则
- **方法长度限制**：单个方法（包括 `main` 方法）**最多不超过 40 行**（不含空行和注释）。  
  如果超过，必须立即重构，封装成多个子方法。每个子方法职责单一、名称清晰。
- **代码可读性优先**：优先使用清晰的变量/方法名，而不是写很长的单个方法。

## 命名规范（必须严格遵守）
- **类名**：PascalCase（大驼峰），例如 `EntityAction`、`UserService`、`OrderController`
- **方法名**：camelCase（小驼峰），例如 `getHelloWorld()`、`calculateTotalPrice()`、`fetchUserById()`
- **变量名**：snake_case（全小写 + 下划线），例如 `user_id`、`total_price`、`current_status`
- **常量 / 枚举值 / 静态常量**：UPPER_SNAKE_CASE（全大写 + 下划线），例如 `MAX_RETRY_COUNT`、`STATUS_ACTIVE`、`ERROR_CODE_NOT_FOUND`
- **枚举类**：类名用 PascalCase，枚举常量用 UPPER_SNAKE_CASE

## 其他强制规则
- 禁止使用魔法数字、魔法字符串，必须提取为常量。
- 变量声明时必须使用有意义的名字，避免 `temp`、`data`、`result` 等过于泛化的名称。
- 每次生成或修改代码时，先检查是否违反以上规范，违反则必须立即重构。

## 工作流程提醒
- 当我要求你写新代码或修改代码时，**自动**应用以上所有规范，无需我再次提醒。
- 如果发现代码违反规范，请在修改时主动重构，并说明修改原因。