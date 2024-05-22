#define ASM_SYMBOL(name) name

.global ASM_SYMBOL(PrepareContextInBuffer)
        .global ASM_SYMBOL(SwitchBufferContext)

            ASM_SYMBOL(PrepareContextInBuffer)
    :
#rdi, rsi, rdx, rcx, r8, r9
# 1. % rdi - buffer
# 2. % rsi - CallRunner
# 3. % rdx - runner

#Switch to stack from buffer
      movq %
    rsp,
% r11 #save rsp to stack movq % rdi, % rsp #load buffer to rsp

                                         subq $64,
% rsp #Just to be safe

#buffer stack must be 16 - byte aligned
        andq $
    - 16,
% rsp addq $8, %
                   rsp

# 3 - thd arg(runner)
#mov to 7 - th argument in buffer
                   movq
                   % rdx,
8(% rsp)

#Prepare for context switching

        pushq
    %
    rsi #push CallRunner

#Callee - saved registers
    pushq $0 #r15 pushq $0 #r14 pushq $0 #r13 pushq $0 #r12 pushq $0
    #rbp pushq $0 #rbx

#Return value for PrepareContextInBuffer
#rsp to buffer
    movq
    % rsp,
%
    rax

#Restore callstack
    movq
    % r11,
% rsp #rsp : = tmp retq

             ASM_SYMBOL(SwitchBufferContext)
    :