#pragma once

#ifdef DEBUG
#define error(msg) printf("\033[31m %s \n\033[0m", msg)
#else
#define error(msg) 
#endif