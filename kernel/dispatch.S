  .align 2
  .thumb_func
  .globl dispatch_entry

dispatch_entry:
  push {r4-r7}
  mov r0, r8
  mov r1, r9
  mov r2, r10
  mov r3, r11
  push {r0-r3}
  ldr r0, =cur_task
  ldr r1, [r0]
  cmp r1, #0
  beq disp_010
  ldr r0, =ctx_tbl
  // because task id 0 is not task, so we need to minus 1 
  sub r1, #1
  // calculate index of array
  lsl r1, r1, #2
  mov r2, sp
  str r2, [r0, r1]
disp_010:
  // change task: next_task -> cur_task
  ldr r0, =next_task
  ldr r1, [r0]
  ldr r0, =cur_task
  str r1, [r0]

  // ctx_tbl[cur_task - 1] has sp of cur_task
  ldr r0, =ctx_tbl
  sub r1, #1
  lsl r1, r1, #2
  ldr r2, [r0, r1]
  mov sp, r2
  // {r0-r3} is auto matically saved by hardware, so we need to put it to {r8-r11}
  pop {r0-r3}
  mov r8, r0
  mov r9, r1
  mov r10, r2
  mov r11, r3
  pop {r4-r7}
  bx lr
