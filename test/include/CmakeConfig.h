#define hello_world_GitTag "test build"

char* hello_world_buildDate(char* buffer) {
    sprintf(buffer, "%s %s", __DATE__, __TIME__); 
    return buffer;
}