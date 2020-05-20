


#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"


#define STACK_SIZE 128

struct task_params {
	const char * task_name;
	const uint32_t interval_ticks;
	TaskHandle_t handle;
};


void task_loop(void* pvParameters) {
	struct task_params* params = (struct task_params*)pvParameters;
	uint32_t a = 0;
	printf("Enter %s\n", params->task_name);
	for(;;) {
		printf("Hello from %s. counter=%d\n", params->task_name, a++);
		vTaskDelay(params->interval_ticks);
	}
}

int main() {

	struct task_params tasks[] = {{
		.task_name = "Task One",
		.interval_ticks = 250
	},{
		.task_name = "Task Two",
		.interval_ticks = 333
	}};

	const size_t num_tasks = sizeof(tasks) / sizeof(struct task_params);

	for(size_t i = 0; i < num_tasks; i++) {
		struct task_params* params = &tasks[i];
		xTaskCreate(task_loop, params->task_name, STACK_SIZE, params, tskIDLE_PRIORITY, &params->handle);
	}

	printf("Starting scheduler\n");
	vTaskStartScheduler();
	printf("Something got really bad\n");

}

