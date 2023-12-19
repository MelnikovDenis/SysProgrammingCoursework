void createFifo(const char *path);
int openWriteOnlyFifo(char *path);
int openReadOnlyFifo(const char *path);
void writeString(int fd, char *str);
char* readString(int fd, char *str);