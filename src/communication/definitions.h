#ifndef K_DEFINITIONS_H
#define	K_DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define BUFFER_LENGTH 2600
extern char *endMsg;
extern char *infoMsg;
extern char *tahMsg;
extern char *okMsg;

void printError(char *str);

#ifdef	__cplusplus
}
#endif

#endif	/* K_DEFINITIONS_H */

