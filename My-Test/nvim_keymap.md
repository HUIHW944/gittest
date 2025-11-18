# 📚 Neovim 快捷键速查表（Leader = `,`）

>  **编写日期：2025.11.13**  
>  **版本：V1.0**  
>  **适用环境**：Neovim 原生（非 VS Code）  
>  **Leader 键**：英文逗号 `,`  
>  **模式说明**：`n`=普通, `i`=插入, `v`=可视, `x`=选中, `t`=终端

---

## 🔑 一、核心编辑与导航（高频）

| 模式   | 快捷键        | 功能                                | 描述                          |
| ------ | --------      | ------                              | ------                        |
| `i`    | `jk`          | 退出插入模式                        | `zz:escape`                   |
| `n`    | `<Space>`     | 跳转到行尾                          | `zz:GoToEndOfLine` (`$`)      |
| `n`    | `z;`          | 保存文件                            | `zz:WriteBuffer` (`:w`)       |
| `n`    | `<Backspace>` | 清除搜索高亮                        | `zz:RemoveHighlight` (`:noh`) |
| `n`    | `gh`          | 显示 LSP 诊断悬浮                   | `zz:ShowDiagnosticHover`      |
| `n`    | `s`           | **Lightspeed 跳转**（覆盖原生 `s`） | `zz:LightSpeed:omni_s`        |
| `n/v`  | `,s`          | Hop：跳转到任意字符                 | `zz:hop:hint_char1`           |
| `n/v`  | `,w` / `,b`   | Hop：跳转到后/前单词                | `zz:hop:hint_words`           |
| `n/v`  | `,j` / `,k`   | Hop：跳转到下/上行                  | `zz:hop:hint_lines`           |
| `n/v`  | `,J` / `,K`   | Hop：垂直任意位置跳转               | `zz:hop:hint_vertical`        |
| `n/v`  | `,x` / `,X`   | Hop：跳转并复制/粘贴                | `zz:hop:hopyank`              |

---

## 🖥️ 二、窗口与标签页管理

| 模式    | 快捷键                                | 功能                    | 描述                  |
| ------  | --------                              | ------                  | ------                |
| `n/t/i` | `<C-h/j/k/l>` 或 `<A-; / /> / [ / '>` | 窗口切换（左/下/上/右） | `zz:N/TI.Goto*Win`    |
| `n`     | `,en`                                 | 新建标签页              | `zz:N.TabNew`         |
| `n`     | `,ej` / `,ek`                         | 下一个/上一个标签页     | `zz:N.TabNext/Pre`    |
| `n`     | `,ec`                                 | 关闭当前标签页          | `zz:N.TabClose`       |
| `n`     | `,eo`                                 | 仅保留当前标签页        | `zz:N.TabOnly`        |
| `n`     | `,et`                                 | 跳转到第 N 个标签页     | 支持计数（如 `3,et`） |

---

## 📋 三、剪贴板与系统集成

| 模式    | 快捷键        | 功能                           | 描述               |
| ------  | --------      | ------                         | ------             |
| `n/v/x` | `,y`          | 复制到系统剪贴板               | `"+y`              |
| `n/v/x` | `,p` / `,P`   | 粘贴（后/前）                  | `"+p` / `"+P`      |
| `n/v/x` | `,]p` / `,[p` | 智能缩进粘贴                   | `"+]p` / `"+[p`    |
| `n`     | `,<Tab>`      | 在系统文件管理器中打开当前文件 | Windows `explorer` |

---

## 🔍 四、文件与项目导航（Telescope + GrugFar）

| 模式   | 快捷键        | 功能                    | 描述                    |
| ------ | --------      | ------                  | ------                  |
| `n`    | `,ff`         | 查找文件                | `Telescope find_files`  |
| `n`    | `,fs`         | 实时全文搜索            | `Telescope live_grep`   |
| `n`    | `,fg`         | Git 文件搜索            | `Telescope git_files`   |
| `n`    | `,fb`         | 缓冲区列表              | `Telescope buffers`     |
| `n/v`  | `,[` / `,]`   | 在当前文件搜索（词/空） | `GrugFar`               |
| `n/v`  | `, {` / `,}`  | 在项目中搜索（词/空）   | `GrugFar`               |
| `n`    | `,fd`         | 查看诊断                | `Telescope diagnostics` |
| `n`    | `,fk` / `,fl` | 文档/工作区符号         | LSP                     |
| `n`    | `,fr`         | 查找引用                | LSP                     |

---

## 🧠 五、AI / LLM 辅助编程

| 模式   | 快捷键            | 功能                | 描述                         |
| ------ | --------          | ------              | ------                       |
| `n`    | `,ac`             | 切换 LLM 聊天窗口   | `Toggle LLM Chat`            |
| `v/n`  | `,ak`             | 向 LLM 提问（单次） | `Ask LLM`                    |
| `v/n`  | `,aa`             | 多轮对话            | `Ask LLM (multi-turn)`       |
| `v`    | `,ae`             | 解释代码            | `Explain the Code`           |
| `v`    | `,ad`             | 生成文档字符串      | `Generate a Docstring`       |
| `x`    | `,ao`             | 优化并对比代码      | `Optimize the Code`          |
| `n`    | `,ag`             | 生成 Commit 信息    | `Generate AI Commit Message` |
| `x`    | `,tc`             | 生成测试用例        | `Generate Test Cases`        |
| `i`    | `<A-a>`           | 接受 LLM 虚拟建议   | 需开启虚拟文本               |
| `i`    | `<A-n>` / `<A-p>` | 下一个/上一个建议   |                              |

---

## 📂 六、文件树与大纲（NeoTree + Aerial）

| 模式   | 快捷键        | 功能                           | 描述       |
| ------ | --------      | ------                         | ------     |
| `n`    | `,vn`         | 切换左侧文件树（定位当前文件） | `NeoTree`  |
| `n`    | `,vd`         | 聚焦文件树侧边栏               | `NeoTree`  |
| `n`    | `,vf` / `,vg` | 切换 Buffers / Git 状态视图    | `NeoTree`  |
| `n`    | `,v.`         | 切换 Markdown 预览             | `Markview` |
| `n`    | `,vo`         | 切换代码大纲                   | `Aerial`   |
| `n`    | `{` / `}`     | 上一个/下一个大纲项            | `Aerial`   |

---

## 🐞 七、调试（DAP）

| 模式   | 快捷键         | 功能         | 描述                  |
| ------ | --------       | ------       | ------                |
| `n`    | `<F5>`         | 开始调试     | `DapNew`              |
| `n`    | `<F7>`         | 继续         | `DapContinue`         |
| `n`    | `<F8>`         | 单步跳过     | `DapStepOver`         |
| `n`    | `<F10>`        | 单步进入     | `DapStepInto`         |
| `n`    | `<F11>`        | 单步跳出     | `DapStepOut`          |
| `n`    | `<F9>`         | 切换断点     | `DapToggleBreakpoint` |
| `n`    | `<S-F5>`       | 终止调试     | `DapTerminate`        |
| `n`    | `,va` / `<F3>` | 切换 DAP UI  | `dapui:toggle`        |
| `n`    | `,vt`          | 打开浮动终端 | `dapui:FlotTerm`      |

---

## 🛠️ 八、构建与编译（CMake）

| 模式   | 快捷键        | 功能                 | 描述             |
| ------ | --------      | ------               | ------           |
| `n`    | `,cb`         | 构建项目             | `CMakeBuild`     |
| `n`    | `,cr`         | 运行项目             | `CMakeRun`       |
| `n`    | `,cc`         | 清理构建             | `CMakeClean`     |
| `n`    | `,cg`         | 生成构建系统         | `CMakeGenerate`  |
| `n`    | `,cB` / `,cR` | 构建/运行当前文件    |                  |
| `n`    | `,cs*`        | 选择预设/目录/目标等 | 如 `,cst` 选目标 |

---

## 📜 九、Git 与版本控制

| 模式   | 快捷键        | 功能                | 描述                  |
| ------ | --------      | ------              | ------                |
| `n`    | `,do`         | 打开 DiffView       | 对比变更              |
| `n`    | `,dh`         | 查看文件历史        | `DiffviewFileHistory` |
| `n`    | `,dc` / `,dt` | 关闭/切换 Diff 面板 |                       |
| `x`    | `,ts`         | 翻译选中单词        | LLM 辅助              |

---

## 🧩 十、缓冲区与会话管理

| 模式   | 快捷键           | 功能                       | 描述             |
| ------ | --------         | ------                     | ------           |
| `n`    | `,bo` / `,bc`    | 挑选并跳转/关闭 buffer     | `BufferLinePick` |
| `n`    | `,1`–`,9` / `,0` | 跳转到第 1–9 / 最后 buffer |                  |
| `n`    | `z'`             | 保存会话（带输入）         | `MiniSessions`   |

---

## 🧰 十一、高级编辑功能

| 模式   | 快捷键   | 功能                     | 描述             |
| ------ | -------- | ------                   | ------           |
| `n`    | `,'`     | 切换 virtual edit 模式   | 光标可移至行尾外 |
| `n`    | `,.`     | 切换折叠栏显示           | `foldcolumn`     |
| `n`    | `,``     | 切换 Quickfix 窗口       | 垂直底部，宽 70  |
| `n`    | `,;`     | 切换自动换行             | `:set wrap!`     |
| `x`    | `S`      | 为选中文本添加包围符     | `MiniSurround`   |
| `n`    | `yss`    | 为当前行添加包围符       | `MiniSurround`   |
| `n`    | `gxc`    | 取消交换操作             | `Mini.Operators` |

---

## 🧪 十二、终端与 REPL

| 模式   | 快捷键   | 功能                     | 描述             |
| ------ | -------- | ------                   | ------           |
| `n`    | `,vm`    | 切换浮动终端（Git 目录） | `ToggleTerm`     |
| `t`    | `<C-q>`  | 退出终端模式             |                  |
| `t`    | `<C-y>`  | 从寄存器粘贴             | 输入寄存器名     |

---

## 🔧 十三、配置与重载

| 模式   | 快捷键   | 功能                           | 描述               |
| ------ | -------- | ------                         | ------             |
| `n`    | `,\e`    | 编辑 Neovim 配置               | `:vsplit $myvimrc` |
| `n`    | `,\s`    | 重新加载配置                   | `:source $myvimrc` |
| `n`    | `,\S`    | 重载 Lazy 插件（从寄存器 `l`） |                    |

---

## 🚪 十四、退出

| 模式   | 快捷键   | 功能                           | 描述               |
| ------ | -------- | ------                         | ------             |
| `n`    | `,q`     | 关闭当前窗口                   | `:q`               |
| `n`    | `,Q`     | 关闭所有窗口                   | `:qa`              |

---

> 💡 **提示**：  
> - **`s` 键已被 Lightspeed 覆盖**，不再是“删除字符并插入”，如需原生功能可用 `cl`。  
> - **Hop 与 Lightspeed 共存**：Hop 用 `,s`，Lightspeed 用 `s`，互不冲突。  
> - **NeoTree 与 NvimTree 冲突**：快捷键 `,vn`/`,vf` 实际由 NeoTree 生效。  
> - **VS Code 用户**：以上快捷键仅在 Neovim 原生环境生效；VS Code 环境有独立快捷键（见前文）。

---