  .align 2
  .thumb_func
  .globl dispatch_entry

dispatch_entry:
  mov r0, #1
  msr primask, r0
  ldr r1, = disp_running
  str r0, [r1]

  push {r4-r7}
  mov r0, r8
  mov r1, r9
  mov r2, r10
  mov r3, r11
  push {r0-r3}

  ldr r0, =cur_task // r0 = *cur_task
  ldr r1, [r0] // r1 = cur_task
  cmp r1, #0
  beq disp_010 // no task to run

  mov r2, sp // r2 = sp
  str r2, [r1] // *cur_task = sp(r2)
disp_010:
  ldr r1, =sche_task // r1 = *sche_task
  ldr r2, [r1] // r2 = sche_task
  cmp r2, #0
  bne disp_030 // sche_task is not null

  str r2, [r0] // *cur_task = sche_task
disp_020: // idle task
  mov r3, #0
  msr primask, r3
  mov r3, #1
  msr primask, r3

  ldr r2, [r1] // r2 = sche_task
  cmp r2, #0
  beq disp_020 // sche_task is null

disp_030:
  // change task: next_task -> cur_task
  str r2, [r0] // cur_task = sche_task
  ldr r0, [r2] // r0 = sche_task
  mov sp, r0 // sp = sche_task

  // {r0-r3} is auto matically saved by hardware, so we need to put it to {r8-r11}
  pop {r0-r3}
  mov r8, r0
  mov r9, r1
  mov r10, r2
  mov r11, r3
  pop {r4-r7}

  // disp_running = 0, enable interrupt
  ldr r0, =disp_running
  mov r1, #0
  str r1, [r0]
  msr primask, r1
  bx lr

