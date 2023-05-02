static void writetofile(char* filename, char* data){
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("ERROR OPENING %s.", filename);
        exit(1);
    }
    int charWritten = fprintf(file, data);
    if (charWritten <= 0) {
        printf("ERROR WRITING DATA");
        exit(1);
    }
    fclose(file);
}