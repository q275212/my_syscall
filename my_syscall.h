/*
 * ==============================================================
 *  作者：simple
 *  日期：2024-11-16
 *  描述：ARM64 系统调用宏定义，提供简洁高效的系统调用接口，增加HOOK难度。
 * ==============================================================
 */


/* 定义汇编输入约束，用于内联汇编中的输入操作数 */
#define ASM_INPUTS_0 "r"(_x8)
#define ASM_INPUTS_1 ASM_INPUTS_0, "r"(_x0)
#define ASM_INPUTS_2 ASM_INPUTS_1, "r"(_x1)
#define ASM_INPUTS_3 ASM_INPUTS_2, "r"(_x2)
#define ASM_INPUTS_4 ASM_INPUTS_3, "r"(_x3)
#define ASM_INPUTS_5 ASM_INPUTS_4, "r"(_x4)
#define ASM_INPUTS_6 ASM_INPUTS_5, "r"(_x5)

#define DECL_REGS_0() register uint64_t _x0 asm("x0") = 0;
#define DECL_REGS_1(arg0)  register uint64_t _x0 asm("x0") = (uint64_t)(arg0);
#define DECL_REGS_2(arg0, arg1)  DECL_REGS_1(arg0)  register uint64_t _x1 asm("x1") = (uint64_t)(arg1);
#define DECL_REGS_3(arg0, arg1, arg2)  DECL_REGS_2(arg0, arg1)  register uint64_t _x2 asm("x2") = (uint64_t)(arg2);
#define DECL_REGS_4(arg0, arg1, arg2, arg3)  DECL_REGS_3(arg0, arg1, arg2)  register uint64_t _x3 asm("x3") = (uint64_t)(arg3);
#define DECL_REGS_5(arg0, arg1, arg2, arg3, arg4)  DECL_REGS_4(arg0, arg1, arg2, arg3)  register uint64_t _x4 asm("x4") = (uint64_t)(arg4);
#define DECL_REGS_6(arg0, arg1, arg2, arg3, arg4, arg5)  DECL_REGS_5(arg0, arg1, arg2, arg3, arg4)  register uint64_t _x5 asm("x5") = (uint64_t)(arg5);


/* 辅助宏：根据参数数量选择正确的宏 */
#define GET_MACRO(_0, _1, _2, _3, _4, _5, _6, NAME, ...) NAME

/* 根据传递的参数数量，选择对应的寄存器声明宏 */
#define DECL_REGS(...) \
    GET_MACRO(_, ##__VA_ARGS__, DECL_REGS_6, DECL_REGS_5, DECL_REGS_4, DECL_REGS_3, DECL_REGS_2, DECL_REGS_1, DECL_REGS_0)(__VA_ARGS__)

/* 根据传递的参数数量，选择对应的汇编输入约束宏 */
#define ASM_INPUTS(...) \
    GET_MACRO(_, ##__VA_ARGS__, ASM_INPUTS_6, ASM_INPUTS_5, ASM_INPUTS_4, ASM_INPUTS_3, ASM_INPUTS_2, ASM_INPUTS_1, ASM_INPUTS_0)

/* ==============================================================
 * 系统调用宏定义
 * --------------------------------------------------------------
 *  参数：
 *      num     - 系统调用号
 *      ...     - 可变参数列表，最多支持 6 个参数
 *  返回值：
 *      系统调用的返回值，类型为 uint64_t
 *  用法示例：
 * 
    int pid = my_syscall(__NR_getpid);
    printf("PID: %d\n", pid);
    const char *msg = "Hello, World!\n";
    size_t len = 16; // 字符串长度
    ssize_t ret = my_syscall(__NR_write, STDOUT_FILENO, msg, len,0x1000000000LL);

 *  MOV             W8, #0xAC ; __NR_getpid
    SVC             0

    MOV             X1, X0
    ADRL            X0, aPidD ; "PID: %d\n"
    BL              .printf

    MOV             W0, #1
    ADRP            X1, #aHelloWorld@PAGE ; "Hello, World!\n"    
    ADD             X1, X1, #aHelloWorld@PAGEOFF ; "Hello, World!\n"
    MOV             W8, #0x40 ; __NR_write
    MOV             W2, #0x10
    MOV             X3, #0x1000000000
    SVC             0
 * ==============================================================
 */
#define my_syscall(num, ...) ({                                  \
    /* 声明并初始化 x8 寄存器为系统调用号 num */                    \
    register uint64_t _x8 asm("x8") = (uint64_t)(num);                      \
    /* 根据参数数量声明并初始化相应的寄存器 */                      \
    DECL_REGS(__VA_ARGS__)                                          \
    /* 执行系统调用 */                                              \
    asm volatile(                                                   \
        "svc 0\n"                                                   \
        : "=r"(_x0) /* 输出操作数，获取系统调用的返回值 */           \
        : ASM_INPUTS(__VA_ARGS__) /* 输入操作数，系统调用号和参数 */ \
        : "memory"); /* 通知编译器可能影响内存 */                    \
    _x0; /* 返回系统调用的返回值 */                                 \
})

