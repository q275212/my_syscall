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
