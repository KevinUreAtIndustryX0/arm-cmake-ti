#pragma once

#ifdef TESTING
#define MOCKABLE __attribute__((weak))
#else
#define MOCKABLE
#endif