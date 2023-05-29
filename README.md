Це лабораторна робота з теми процесор. \
Метою цієї роботи було: зробити емуляцію процесора і простого асембелера, які схожі до https://youtu.be/Zt0JfmV7CyI \
Також зробити можливість імпортувати файли і запускати в інтерактивному режимі(як оболонка пітона).

Запуск:
`lab.exe path` - це запуск файлу \
`lab.exe -i`, `lab.exe --interactive` - це запуск в інтерактивному режимі

Регістри процесора:
1. `IP` - instruction pointer
2. `SP` - stack pointer
3. `A` - register(temp)
4. `B` - register(main)
5. `C` - register(temp2, save)

Інструкції процесора:

| Instruction       | Description                            |
|-------------------|----------------------------------------|
| `READ`            | A <- mem[A]                            |
| `WRITE`           | mem[A] <- B                            |
| `SWAP`            | A <-> B                                |
| `SWAP_C`          | A <-> C                                |
| `SET`             | A <- mem[IP + 1] <br> IP = IP + 2      |
| `JUMP_ZERO` `Arg` | if A == 0 then IP = `Arg`              |
| `READ_IP`         | A <- IP + 1 (next instruction address) |
| `WRITE_IP`        | IP <- A                                |
| `READ_SP`         | A <- SP                                |
| `WRITE_SP`        | SP <- A                                |
| `ADD`             | A += B                                 |
| `SUB`             | A -= B                                 |
| `MULT`            | A *= B (unsigned overflow)             |
| `DIV`             | A /= B (floor div)                     |
| `MOD`             | A %= B                                 |
| `SHIFT_L`         | A <<= B                                |
| `SHIFT_R`         | A >>= B                                |
| `AND`             | A &= B                                 |
| `OR`              | A \| = B                               |
| `XOR`             | A ^= B                                 |
| `NOT`             | A = ~A                                 |
| `LNOT`            | A = !A                                 |
| `DUMP`            | dumps processor state(debug purpose)   |

Інструкції асемблера = інструкції процесора + доповнення:

| Instruction            | Description                                                       | Side effect(registers)  |
|------------------------|-------------------------------------------------------------------|-------------------------|
| `SAVE_B`               | save B to C                                                       | A, B, C                 |
| `LOAD_B`               | load B from C                                                     | A, B, C                 |
| `SWAP_BC`              | swap B and C                                                      |                         |
| `JUMP Arg`             | jump to `Arg`                                                     | A                       |
| `JUMP_NOT_ZERO` `Arg`  | jump to `Arg` if not zero                                         | A                       |
| `STOP`                 | stop execution                                                    | :)                      |
| `COPY_B`               | copy B to A                                                       | A <- B                  |
| `COPY_A`               | copy A to B                                                       | B <- A                  |
| `PRINT`                | print B as char                                                   | A                       |
| `INPUT`                | read char to A                                                    | A                       |
| `READ_RAM`             | read from RAM(0-indexed)                                          | A                       |
| `WRITE_RAM`            | write to RAM(0-indexed)                                           | A                       |
| `READ_STACK`           | read from stack top                                               | A                       |
| `WRITE_STACK`          | write to stack top                                                | A                       |
| `EXTEND_STACK`         | extend stack size                                                 | A, C                    |
| `PUSH_STACK`           | push B on stack                                                   | A, C                    |
| `POP_STACK`            | pop stack                                                         | A, C                    |
| `PREPARE_ARGS`         | use before pushing args                                           | A, C                    |
| `CALL` `address` `cnt` | call function located at `address`<br>`cnt` - number of arguments | A, B(now args count), C |
| `RET`                  | return from function                                              | A, C                    |

Також в асемблері можна позначити `:label` - це мітка, яка зберігає адресу наступної команди,
може  використовуватися для `JUMP` і `CALL`

Також можна позначити `import "file"` - що імпортує файл, але не буде виконувати його,
використовується для імпорту функцій

Також є файли з прикладами:
1. `code.txt` - показує `import`, виклик функції
2. `print_num.txt` - показує `label` і що їх можна "експортувати"
3. `echo.txt` - показує просте IO