#ifndef _TASK_MANAGER_H
#define _TASK_MANAGER_H
#include "interrupts.h"
#include "types.h"

#define MAX_TASK_NUM 32

typedef enum{
    TaskRunning,
    TaskPaused,
    TaskFrozen
} TaskState;

typedef volatile struct{
    CPUState savedCpuState;
    char name[128];
    char filename[128];
    uint32 executable_length;
    TaskState state;
    uint8 RESV[252];
    uint8 stack[1048576];
    void* executable;
} Task;

Task** tasks;
uint16 numTasks;
uint32 tasksRunning;
uint16 contextSwitchInerval;
uint16 contextSwitchCounter;
int16 currentTask;
uint8 taskManagerRunning;

CPUState schedule(CPUState currentState);
void setupTaskManager();
int16 addTask(void* codeBase, uint32 codeLength, uint32 startOffset, char* name, char* filename);

#endif