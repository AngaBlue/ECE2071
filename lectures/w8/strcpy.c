void strcpy(char dest[], char source[]) {
    int i = 0;
    while ((dest[i] = source[i]) != '\0')
        ++i;    
}
