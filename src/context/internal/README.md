[source][stanford-x86-64]

```
| Register | Conventional use                | Low 32-bits | Low 16-bits | Low 8-bits |
|----------|---------------------------------|-------------|-------------|------------|
| %rax     | Return value, caller-saved      | %eax        | %ax         | %al        |
| %rdi     | 1st argument, caller-saved      | %edi        | %di         | %dil       |
| %rsi     | 2nd argument, caller-saved      | %esi        | %si         | %sil       |
| %rdx     | 3rd argument, caller-saved      | %edx        | %dx         | %dl        |
| %rcx     | 4th argument, caller-saved      | %ecx        | %cx         | %cl        |
| %r8      | 5th argument, caller-saved      | %r8d        | %r8w        | %r8b       |
| %r9      | 6th argument, caller-saved      | %r9d        | %r9w        | %r9b       |
| %r10     | Scratch/temporary, caller-saved | %r10d       | %r10w       | %r10b      |
| %r11     | Scratch/temporary, caller-saved | %r11d       | %r11w       | %r11b      |
| %rsp     | Stack pointer, callee-saved     | %esp        | %sp         | %spl       |
| %rbx     | Local variable, callee-saved    | %ebx        | %bx         | %bl        |
| %rbp     | Local variable, callee-saved    | %ebp        | %bp         | %bpl       |
| %r12     | Local variable, callee-saved    | %r12d       | %r12w       | %r12b      |
| %r13     | Local variable, callee-saved    | %r13d       | %r13w       | %r13b      |
| %r14     | Local variable, callee-saved    | %r14d       | %r14w       | %r14b      |
| %r15     | Local variable, callee-saved    | %r15d       | %r15w       | %r15b      |
| %rip     | Instruction pointer             |             |             |            |
| %rflags  | Status/condition code bits      |             |             |            |
```

---

[stanford-x86-64]: https://web.stanford.edu/class/archive/cs/cs107/cs107.1174/guide_x86-64.html