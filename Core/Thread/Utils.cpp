#include "Utils.h"
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

bool bindThisThreadToCore(int coreId) {
	int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (coreId < 0 || coreId >= num_cores)
		return false;

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(coreId, &cpuset);

	pthread_t currentThread = pthread_self();
	return pthread_setaffinity_np(currentThread, sizeof(cpu_set_t), &cpuset) == 0;
}
